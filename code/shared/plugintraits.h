#pragma once

// Copyright (C) 2019 Force67

#include <cstdint>

#define EXPORT extern "C" __declspec(dllexport)

using u32 = uint32_t;

enum class PluginVersion : u32
{
	V_1_0,
};

namespace utl {
	class File;
}

struct pluginDesc
{
	PluginVersion version;
	const char* name;
	const char* prettyName;
	u32(*accept)(utl::File&); // <return your custom file index
	bool (*init)(utl::File&, u32 index);
	void (*shutdown)(); // <optional
};