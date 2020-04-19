#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

// you MUST include this file before anything else

#include <base.h>

#ifdef _WIN32
#include <Windows.h>
#include "glad/vulkan.h"
#endif

namespace video_core {
constexpr u32 VCORE_MIN_VK_VERSION = VK_API_VERSION_1_1;
}