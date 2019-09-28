#pragma once

// Copyright (C) 2019 Force67

#include <cstdint>

#define EXP extern "C" __declspec(dllexport)

enum class PluginVersion : uint8_t
{
	V_1_0,
};

#pragma pack(push, 1)
struct Plugin
{
	PluginVersion version;
	const char* name;
	bool (*init)(const char* file);
	void (*shutdown)(); // <optional
};
#pragma pack(pop)