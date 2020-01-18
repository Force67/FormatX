
#include "rend.h"
#include "vk/vk_rend.h"

std::unique_ptr<rendInterface> createRend(rendBackend backend)
{
	std::unique_ptr<rendInterface> ptr;

	switch (backend) {
	case rendBackend::vulkan:
	{
		ptr = std::make_unique<vkBackend>();
		break;
	}
	case rendBackend::dx11:
	case rendBackend::opengl:
	{
		__debugbreak();
		return nullptr;
	}
	}

	if (!ptr->create())
		return nullptr;

	return std::move(ptr);
}