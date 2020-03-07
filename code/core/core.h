#pragma once

/*
 * FormatX : Video core
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <vector>
#include <base.h>
#include <plugin_traits.h>
#include <utl/logger/logger.h>

#ifdef _WIN32 
#ifdef COMPILING_CORE
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#endif

// forward decl vcore
// so we dont include it everywhere
namespace video_core {
class renderWindow;
class renderInterface;
}

namespace core {
enum class result {
    success,
    failed,
};

class fxcore {
public:
    CORE_API fxcore();
    CORE_API ~fxcore();
    CORE_API result init(video_core::renderWindow&);

    auto& getPlugins() const { return pluginList; }

private:
    UniquePtr<video_core::renderInterface> renderer;
    std::vector<pluginLoader*> pluginList;
};
}