
// Copyright (C) 2019-2020 Force67

#include "vk_rend.h"

#include <QString>
#include <QVulkanFunctions>

//https://code.woboq.org/qt5/qtbase/examples/vulkan/hellovulkancubes/vulkanwindow.cpp.html

class vkRend : public QVulkanWindowRenderer
{
public:
	vkRend(vkBackend *b) :
		be(b)
	{}

	void initResources() override;
	void startNextFrame() override;

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
};

void vkRend::initResources()
{
	auto &inst = be->vkInstance();
	devFuncs = inst.deviceFunctions(be->device());

	QString info;
	info += QString::asprintf("Number of physical devices: %d\n", be->availablePhysicalDevices().count());
	QVulkanFunctions* f = inst.functions();
	VkPhysicalDeviceProperties props;
	f->vkGetPhysicalDeviceProperties(be->physicalDevice(), &props);
	info += QString::asprintf("Active physical device name: '%s' version %d.%d.%d\nAPI version %d.%d.%d\n",
		props.deviceName,
		VK_VERSION_MAJOR(props.driverVersion), VK_VERSION_MINOR(props.driverVersion),
		VK_VERSION_PATCH(props.driverVersion),
		VK_VERSION_MAJOR(props.apiVersion), VK_VERSION_MINOR(props.apiVersion),
		VK_VERSION_PATCH(props.apiVersion));
	info += QStringLiteral("Supported instance layers:\n");
	for (const QVulkanLayer& layer : inst.supportedLayers())
		info += QString::asprintf("    %s v%u\n", layer.name.constData(), layer.version);
	info += QStringLiteral("Enabled instance layers:\n");
	for (const QByteArray& layer : inst.layers())
		info += QString::asprintf("    %s\n", layer.constData());
	info += QStringLiteral("Supported instance extensions:\n");
	for (const QVulkanExtension& ext : inst.supportedExtensions())
		info += QString::asprintf("    %s v%u\n", ext.name.constData(), ext.version);
	info += QStringLiteral("Enabled instance extensions:\n");
	for (const QByteArray& ext : inst.extensions())
		info += QString::asprintf("    %s\n", ext.constData());
	info += QString::asprintf("Color format: %u\nDepth-stencil format: %u\n",
		be->colorFormat(), be->depthStencilFormat());
	info += QStringLiteral("Supported sample counts:");
	const QVector<int> sampleCounts = be->supportedSampleCounts();
	for (int count : sampleCounts)
		info += QLatin1Char(' ') + QString::number(count);
	info += QLatin1Char('\n');

	__debugbreak();
}

void vkRend::startNextFrame()
{

}

bool vkBackend::create()
{
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

	return false;
}

QVulkanWindowRenderer* vkBackend::createRenderer()
{
	auto* r = new vkRend(this);
	r->initResources();

	return r;
}