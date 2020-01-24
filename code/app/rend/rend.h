#pragma once

#include <QWindow>
#include <memory>

enum class rendBackend
{
	none,
	vulkan,
	dx11,
	opengl
};

class rendInterface
{
public:
	virtual bool create() = 0;
};

std::unique_ptr<rendInterface> createRend(QWindow*, rendBackend);
rendBackend getBackendType();