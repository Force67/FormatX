#pragma once

#include <memory>

enum class rendBackend
{
	vulkan,
	dx11,
	opengl
};

class rendInterface
{
public:
	virtual bool create() = 0;
};

std::unique_ptr<rendInterface> createRend(rendBackend);