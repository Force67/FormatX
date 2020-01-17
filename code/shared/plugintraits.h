#pragma once

// Copyright (C) 2019 Force67

#include <cstdint>

#define EXPORT extern "C" __declspec(dllexport)

using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

enum class PluginVersion
{
	V_1_0,
};

namespace utl {
	class File;
}

#pragma pack(push, 1)

/*desc may be expanded in future*/
struct fileDesc
{
	int type; /*types are local to your plugin*/
	const char* name; 
};

struct pluginLoader
{
	PluginVersion version;
	const char* name;
	const char* prettyName;
	bool(*accept)(utl::File&, fileDesc& out);
	bool(*init)(utl::File&, const fileDesc& in);
	void(*shutdown)(); // <optional
};

#pragma pack (pop)