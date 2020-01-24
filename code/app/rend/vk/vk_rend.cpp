
// Copyright (C) 2019-2020 Force67, Greavesy

#include "vk_rend.h"

#include <QString>
#include <QFile>
#include <QVulkanFunctions>

//https://code.woboq.org/qt5/qtbase/examples/vulkan/hellovulkancubes/vulkanwindow.cpp.html

// Note that the vertex data and the projection matrix assume OpenGL. With
// Vulkan Y is negated in clip space and the near/far plane is at 0/1 instead
// of -1/1. These will be corrected for by an extra transformation when
// calculating the modelview-projection matrix.
static float vertexData[] = { // Y up, front = CCW
	 0.0f,   0.5f,   1.0f, 0.0f, 0.0f,
	-0.5f,  -0.5f,   0.0f, 1.0f, 0.0f,
	 0.5f,  -0.5f,   0.0f, 0.0f, 1.0f
};

static const int UNIFORM_DATA_SIZE = 16 * sizeof(float);

static inline constexpr VkDeviceSize aligned(VkDeviceSize v, VkDeviceSize byteAlign)
{
	return (v + byteAlign - 1) & ~(byteAlign - 1);
}

class vkRend : public QVulkanWindowRenderer
{
public:
	vkRend(vkBackend *b) : be(b)
	{}

	void initResources() override;
	void initSwapChainResources() override;
	void startNextFrame() override;
	void releaseSwapChainResources() override;
	void releaseResources() override;

private:
	vkBackend* be;
	QVulkanDeviceFunctions* devFuncs;
	VkDeviceMemory m_bufMem = VK_NULL_HANDLE;
	VkBuffer m_buf = VK_NULL_HANDLE;
	VkDescriptorBufferInfo m_uniformBufInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];
	VkDescriptorPool m_descPool = VK_NULL_HANDLE;
	VkDescriptorSetLayout m_descSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet m_descSet[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];
	VkPipelineCache m_pipelineCache = VK_NULL_HANDLE;
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	VkPipeline m_pipeline = VK_NULL_HANDLE;

	QMatrix4x4 m_Projection;

	VkShaderModule createShaderModule(const QString& name);
};

void vkRend::initResources()
{
	VkResult result = VK_RESULT_MAX_ENUM;
	auto &inst = be->vkInstance();
	VkDevice device = be->device();
	devFuncs = inst.deviceFunctions(device);

	//create the info for the triangle's vertex buffer and then create a buffer
	//based on the info appiled in 'bufferInfo'
	//TODO: create function for building vertex buffers.
	const int concurrentFrameCount = be->concurrentFrameCount();
	const VkPhysicalDeviceLimits* pdevLimits = &be->physicalDeviceProperties()->limits;
	const VkDeviceSize uniAlign = pdevLimits->minUniformBufferOffsetAlignment;

	auto alignedSize = aligned(aligned(sizeof(vertexData), uniAlign) * concurrentFrameCount + 1, uniAlign);
		
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	bufferInfo.size = alignedSize;

	result = devFuncs->vkCreateBuffer(device, &bufferInfo, nullptr, &m_buf);
	if (result != VkResult::VK_SUCCESS) {
		printf("Failed to create Vertex Buffer!\n");
		__debugbreak();
	}

	//The buffer should be created at this point does not have memory allocated. 
	//We fetch the size required from the buffer and allocate new memory.
	//TODO: Seperate from here.
	VkMemoryRequirements memRequirements;
	devFuncs->vkGetBufferMemoryRequirements(device, m_buf, &memRequirements);

	VkMemoryAllocateInfo allocation{};
	memset(&allocation, 0, sizeof(allocation));
	allocation.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocation.allocationSize = memRequirements.size;
	allocation.memoryTypeIndex = be->hostVisibleMemoryIndex();

	result = devFuncs->vkAllocateMemory(device, &allocation, nullptr, &m_bufMem);
	if (result != VkResult::VK_SUCCESS) {
		printf("Failed to allocate memory for Vertex Buffer!\n");
		__debugbreak();
	}
	result = devFuncs->vkBindBufferMemory(device, m_buf, m_bufMem, 0);
	if (result != VkResult::VK_SUCCESS) {
		printf("Failed to bind buffer!\n");
		__debugbreak();
	}

	//Now we have memory allocated and a buffer, we can fill the buffer:
	//TODO: Seperate from here.
	quint8* data;	
	result = devFuncs->vkMapMemory(device, m_bufMem, 0, memRequirements.size, 0, reinterpret_cast<void**>(&data));
	if (result != VkResult::VK_SUCCESS) {
		printf("Failed to bind buffer!\n");
	}

	const VkDeviceSize vertexAllocSize = aligned(sizeof(vertexData), uniAlign);
	const VkDeviceSize uniformAllocSize = aligned(UNIFORM_DATA_SIZE, uniAlign);

	memcpy(data, vertexData, sizeof(vertexData));
	QMatrix4x4 ident;
	memset(m_uniformBufInfo, 0, sizeof(m_uniformBufInfo));
	for (int i = 0; i < concurrentFrameCount; i++) {
		const VkDeviceSize offset = vertexAllocSize + i * uniformAllocSize;
		//const auto offset = i * sizeof(QMatrix4x4);
		memcpy(data + offset, ident.constData(), sizeof(QMatrix4x4));
		m_uniformBufInfo[i].buffer = m_buf;
		m_uniformBufInfo[i].offset = offset;
		m_uniformBufInfo[i].range = uniformAllocSize;
	}
	devFuncs->vkUnmapMemory(device, m_bufMem);

	//next, we create vertex attributes and bind them for the pipeline.
	//This can be vastly improved, especially when we have a vertex struct later.
	//TODO: Vertex struct, add Z component to positions.
	VkVertexInputBindingDescription bindingDescription;
	memset(&bindingDescription, 0, sizeof(bindingDescription));
	bindingDescription.binding = 0;
	bindingDescription.stride = 5 * sizeof(float);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	VkVertexInputAttributeDescription attributeDescription[] = {
			{ // position
				0, // location
				0, // binding
				VK_FORMAT_R32G32_SFLOAT,
				0
			},
			{ // color
				1,
				0,
				VK_FORMAT_R32G32B32_SFLOAT,
				2 * sizeof(float)
			}
	};


	//declare number of attributes and layout for the vertex data for the pipeline.
	VkPipelineVertexInputStateCreateInfo inputInfo;
	memset(&inputInfo, 0, sizeof(inputInfo));
	inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	inputInfo.pNext = nullptr;
	inputInfo.flags = 0;
	inputInfo.vertexBindingDescriptionCount = 1;
	inputInfo.pVertexBindingDescriptions = &bindingDescription;
	inputInfo.vertexAttributeDescriptionCount = 2;
	inputInfo.pVertexAttributeDescriptions = attributeDescription;

	// Set up descriptor set and its layout.
	VkDescriptorPoolSize descPoolSizes = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uint32_t(be->concurrentFrameCount()) };
	VkDescriptorPoolCreateInfo descPoolInfo;
	memset(&descPoolInfo, 0, sizeof(descPoolInfo));
	descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descPoolInfo.maxSets = be->concurrentFrameCount();
	descPoolInfo.poolSizeCount = 1;
	descPoolInfo.pPoolSizes = &descPoolSizes;
	result = devFuncs->vkCreateDescriptorPool(device, &descPoolInfo, nullptr, &m_descPool);
	if (result != VK_SUCCESS)
		printf("Failed to create descriptor pool!");

	VkDescriptorSetLayoutBinding layoutBinding = {
		0, // binding
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		1,
		VK_SHADER_STAGE_VERTEX_BIT,
		nullptr
	};
	VkDescriptorSetLayoutCreateInfo descLayoutInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		nullptr,
		0,
		1,
		&layoutBinding
	};
	result = devFuncs->vkCreateDescriptorSetLayout(device, &descLayoutInfo, nullptr, &m_descSetLayout);
	if (result != VK_SUCCESS)
		printf("Failed to create descriptor set layout!");

	for (int i = 0; i < be->concurrentFrameCount(); ++i) {
		VkDescriptorSetAllocateInfo descSetAllocInfo = {
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			nullptr,
			m_descPool,
			1,
			&m_descSetLayout
		};
		result = devFuncs->vkAllocateDescriptorSets(device, &descSetAllocInfo, &m_descSet[i]);
		if (result != VK_SUCCESS)
			printf("Failed to allocate descriptor set!");

		VkWriteDescriptorSet descWrite;
		memset(&descWrite, 0, sizeof(descWrite));
		descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWrite.dstSet = m_descSet[i];
		descWrite.descriptorCount = 1;
		descWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descWrite.pBufferInfo = &m_uniformBufInfo[i];
		devFuncs->vkUpdateDescriptorSets(device, 1, &descWrite, 0, nullptr);
	}

	// Pipeline cache
	VkPipelineCacheCreateInfo pipelineCacheInfo;
	memset(&pipelineCacheInfo, 0, sizeof(pipelineCacheInfo));
	pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	result = devFuncs->vkCreatePipelineCache(device, &pipelineCacheInfo, nullptr, &m_pipelineCache);
	if (result != VK_SUCCESS)
		printf("Failed to create pipeline cache!");

	//Pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &m_descSetLayout;
	result = devFuncs->vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);
	if (result != VK_SUCCESS)
		printf("Failed to create pipeline layout!");

	//import shaders
	VkShaderModule vertModule = createShaderModule(QStringLiteral("color_vert.spv"));
	VkShaderModule fragModule = createShaderModule(QStringLiteral("color_frag.spv"));

	//begin the creation of the graphics pipeline:
	//insert these shaders into the render pipeline.
	//we create a stage for each shader going to be used; 
	//in this instance, we are only using a vertex and fragment shader.
	VkGraphicsPipelineCreateInfo pipeInfo;
	memset(&pipeInfo, 0, sizeof(pipeInfo));
	pipeInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeInfo.stageCount = 2;

	VkPipelineShaderStageCreateInfo stages[2] = {
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			0,
			VK_SHADER_STAGE_VERTEX_BIT,
			vertModule,
			"main",
			nullptr
		},
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			0,
			VK_SHADER_STAGE_FRAGMENT_BIT,
			fragModule,
			"main",
			nullptr
		}
	};

	pipeInfo.pStages = stages;
	pipeInfo.pVertexInputState = &inputInfo;

	VkPipelineInputAssemblyStateCreateInfo assemblyState;
	memset(&assemblyState, 0, sizeof(assemblyState));
	assemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	assemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	pipeInfo.pInputAssemblyState = &assemblyState;

	//create viewport data for the pipeline. 
	//This can be set later, allowing us to resize the window.
	VkPipelineViewportStateCreateInfo pipeVP;
	memset(&pipeVP, 0, sizeof(pipeVP));
	pipeVP.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	pipeVP.viewportCount = pipeVP.scissorCount = 1;
	pipeInfo.pViewportState = &pipeVP;

	//this declares what we need to set in the rasterizer;
	//this controls culling, polygon types, eg. wireframe,
	//and which face is considered the front.
	VkPipelineRasterizationStateCreateInfo rs;
	memset(&rs, 0, sizeof(rs));
	rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rs.polygonMode = VK_POLYGON_MODE_FILL;
	rs.cullMode = VK_CULL_MODE_NONE;
	rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rs.lineWidth = 1.0f;
	pipeInfo.pRasterizationState = &rs;

	//used for alpha. (i think).
	VkPipelineMultisampleStateCreateInfo ms;
	memset(&ms, 0, sizeof(ms));
	ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	ms.rasterizationSamples = be->sampleCountFlagBits();
	pipeInfo.pMultisampleState = &ms;

	VkPipelineDepthStencilStateCreateInfo ds;
	memset(&ds, 0, sizeof(ds));
	ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	ds.depthTestEnable = VK_TRUE;
	ds.depthWriteEnable = VK_TRUE;
	ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	pipeInfo.pDepthStencilState = &ds;

	VkPipelineColorBlendStateCreateInfo cb;
	memset(&cb, 0, sizeof(cb));
	cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

	VkPipelineColorBlendAttachmentState att;
	memset(&att, 0, sizeof(att));
	att.colorWriteMask = 0xF;
	cb.attachmentCount = 1;
	cb.pAttachments = &att;
	pipeInfo.pColorBlendState = &cb;

	VkDynamicState dynEnable[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dyn;
	memset(&dyn, 0, sizeof(dyn));
	dyn.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dyn.dynamicStateCount = sizeof(dynEnable) / sizeof(VkDynamicState);
	dyn.pDynamicStates = dynEnable;
	pipeInfo.pDynamicState = &dyn;

	pipeInfo.layout = m_pipelineLayout;
	pipeInfo.renderPass = be->defaultRenderPass();

	//we can finally create the pipeline now the data has been specified above:
	result = devFuncs->vkCreateGraphicsPipelines(device, m_pipelineCache, 1, &pipeInfo, nullptr, &m_pipeline);
	if (result != VkResult::VK_SUCCESS) {
		printf("Failed to create graphics pipeline!\n");
	}

	//now that the shaders have been loaded into the pipeline, 
	//we can finally destroy the unused shader modules.
	if (vertModule) {
		devFuncs->vkDestroyShaderModule(device, vertModule, nullptr);
	}
	if (fragModule) {
		devFuncs->vkDestroyShaderModule(device, fragModule, nullptr);
	}
}

void vkRend::initSwapChainResources()
{
	const QSize size = be->swapChainImageSize();

	//this is the camera; it controls FOV, aspect ratio, clip spaces and transforms.
	m_Projection = be->clipCorrectionMatrix();
	m_Projection.perspective(45.0f, size.width() / (float)size.height(), 0.01f, 100.0f);
	m_Projection.translate(0, 0, -4);
}

void vkRend::startNextFrame()
{
	VkClearColorValue clearColor = { {0.0f, 0.0f, 0.0f, 1.0f} };
	VkClearDepthStencilValue clearDepthStencil = { 1.0f, 0.0f };
	VkClearValue clearValues[3];
	memset(clearValues, 0, sizeof(clearValues));
	clearValues[0].color = clearValues[2].color = clearColor;
	clearValues[1].depthStencil = clearDepthStencil;

	VkRenderPassBeginInfo renderPassInfo;
	memset(&renderPassInfo, 0, sizeof(renderPassInfo));
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = be->defaultRenderPass();
	renderPassInfo.framebuffer = be->currentFramebuffer();
	const QSize imgSize = be->swapChainImageSize();
	renderPassInfo.renderArea.extent.width = imgSize.width();
	renderPassInfo.renderArea.extent.height = imgSize.height();
	renderPassInfo.clearValueCount = be->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
	renderPassInfo.pClearValues = clearValues;
	VkCommandBuffer cmd = be->currentCommandBuffer();
	devFuncs->vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	//we now need to render the vertex buffer we created earlier. 
	devFuncs->vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
	devFuncs->vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descSet[be->currentFrame()], 0, nullptr);
	VkDeviceSize vbOffset = 0;
	devFuncs->vkCmdBindVertexBuffers(cmd, 0, 1, &m_buf, &vbOffset);

	//before fully drawing, we need to update viewports and scissors.
	//Maybe we can do this at the start rather than in the update. 
	//TODO: Consider the above ^.
	VkViewport vp;
	vp.x = vp.y = 0;
	vp.width = imgSize.width();
	vp.height = imgSize.height();
	vp.minDepth = 0;
	vp.maxDepth = 1;
	devFuncs->vkCmdSetViewport(cmd, 0, 1, &vp);

	VkRect2D scissor;
	scissor.offset.x = scissor.offset.y = 0;
	scissor.extent.width = vp.width;
	scissor.extent.height = vp.height;
	devFuncs->vkCmdSetScissor(cmd, 0, 1, &scissor);

	devFuncs->vkCmdDraw(cmd, 3, 1, 0, 0);
	devFuncs->vkCmdEndRenderPass(cmd);

	be->frameReady();
	be->requestUpdate();
}

void vkRend::releaseSwapChainResources()
{
	//TODO:: Whenever we need to release swap chain data
}

void vkRend::releaseResources()
{
	VkDevice device = be->device();

	if (m_pipeline)
	{
		devFuncs->vkDestroyPipeline(device, m_pipeline, nullptr);
		m_pipeline = VK_NULL_HANDLE;
	}
	if (m_pipelineLayout)
	{
		devFuncs->vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
		m_pipelineLayout = VK_NULL_HANDLE;
	}
	if (m_pipelineCache)
	{
		devFuncs->vkDestroyPipelineCache(device, m_pipelineCache, nullptr);
		m_pipelineCache = VK_NULL_HANDLE;
	}
	if (m_descSetLayout)
	{
		devFuncs->vkDestroyDescriptorSetLayout(device, m_descSetLayout, nullptr);
		m_descSetLayout = VK_NULL_HANDLE;
	}
	if (m_descPool) {
		devFuncs->vkDestroyDescriptorPool(device, m_descPool, nullptr);
		m_descPool = VK_NULL_HANDLE;
	}
	if (m_buf) {
		devFuncs->vkDestroyBuffer(device, m_buf, nullptr);
		m_buf = VK_NULL_HANDLE;
	}
	if (m_bufMem) {
		devFuncs->vkFreeMemory(device, m_bufMem, nullptr);
		m_bufMem = VK_NULL_HANDLE;
	}
}

VkShaderModule vkRend::createShaderModule(const QString& name)
{
	//open the file and read all the data.
	if (!QFile::exists(name)) {
		printf("File %s does not exist!", name.data());
		return VK_NULL_HANDLE;
	}
	QFile file(name);
	if (!file.open(QIODevice::ReadOnly)) {
		printf("Failed to open shader file!");
		return VK_NULL_HANDLE;
	}

	QByteArray data = file.readAll();
	file.close();

	//when we have the data, we can create the shader module.
	VkShaderModuleCreateInfo shaderInfo;
	memset(&shaderInfo, 0, sizeof(shaderInfo));
	shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderInfo.codeSize = data.size();
	shaderInfo.pCode = reinterpret_cast<const uint32_t*>(data.constData());
	VkShaderModule shaderModule;
	VkResult result = devFuncs->vkCreateShaderModule(be->device(), &shaderInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS) {
		printf("Failed to create shader module: %d", result);
		return VK_NULL_HANDLE;
	}

	return shaderModule;
}

bool vkBackend::create()
{
	QVulkanWindow::create();
#ifdef _DEBUG
	// debug validation layers
	vkInst.setLayers(QByteArrayList()
		<< "VK_LAYER_GOOGLE_threading"
		<< "VK_LAYER_LUNARG_parameter_validation"
		<< "VK_LAYER_LUNARG_object_tracker"
		<< "VK_LAYER_LUNARG_core_validation"
		<< "VK_LAYER_LUNARG_image"
		<< "VK_LAYER_LUNARG_swapchain"
		<< "VK_LAYER_GOOGLE_unique_objects");
#endif

	if (!vkInst.create())
		return false;
	
	setVulkanInstance(&vkInst);
	return true;
}

QVulkanWindowRenderer* vkBackend::createRenderer() {
	return new vkRend(this);
}