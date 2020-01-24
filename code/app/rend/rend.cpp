
#include "rend.h"
#include "vk/vk_rend.h"

static rendBackend g_backendType{ rendBackend::none };

std::unique_ptr<rendInterface> createRend(QWindow* parent, rendBackend type)
{
	std::unique_ptr<rendInterface> ptr;

	switch (type) {
	case rendBackend::vulkan:
	{
		ptr = std::make_unique<vkBackend>(parent);
		break;
	}
	case rendBackend::none:
	case rendBackend::dx11:
	case rendBackend::opengl:
	{
		__debugbreak();
		return nullptr;
	}
	}

	if (!ptr->create())
		return nullptr;

	g_backendType = type;

	return std::move(ptr);
}

rendBackend getBackendType() {
	return g_backendType;
}