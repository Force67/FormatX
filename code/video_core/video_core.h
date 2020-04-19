#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <utl/logger/logger.h>

#ifdef BUILDING_VCORE
#define VCORE_API EXPORT
#else
#define VCORE_API IMPORT
#endif

namespace video_core {

class TextureFactory;
class ShaderFactory;

struct ViewportConfig {
    bool fullScreen = false;
    i32 height = 0;
    i32 width = 0;
};

struct RenderConfig {
    bool clearBackground = false;
};

// render window base
// must be implemented by user
class renderWindow {
public:
    virtual void* getHandle() = 0;

    inline auto& getConfig() {
        return config;
    }

protected:
    ViewportConfig config;
};

class renderInterface {
public:
    inline explicit renderInterface(renderWindow& window) :
        window(window) {}

    // controller
    virtual bool init() = 0;
    virtual void shutdown() = 0;
    virtual void present() = 0;

protected:
    renderWindow& window;

public:
    // stats updated by renderer
    float currentFps = 0.f;
    i32 currentFrame = 0;

    // set by renderer
    TextureFactory* texFactory = nullptr;
    ShaderFactory* shaderFactory = nullptr;
};

// which api should be used in the
// background
enum class RenderApi { 
    null,
    opengl, 
    vulkan,
    dx11,
    dx12, 
};
// describe the render interface instance
enum RenderInstanceFlags : u32 {
    enable_validation
};

struct RenderCallbacks {
    void*(__cdecl *malloc)(size_t);
    void(__cdecl * free)(void*);
};

struct alignas(8) RenderInstanceDesc {
    RenderApi apiType;
    RenderInstanceFlags flags;
    const char* appName;
    RenderCallbacks callbacks;
};

// create a renderer
UniquePtr<renderInterface> VCORE_API createRenderer(renderWindow&, const RenderInstanceDesc&);

// video api allocator
// (uses default cxx allocators, if
// no custom ones are supplied via rendercallbacks)
VCORE_API void* v_malloc(size_t);
VCORE_API void v_free(void*);

// construct c++ object with placement new
template <typename T, typename ... TArgs>
inline T* VAlloc(TArgs... args) { 
    void* mem = v_malloc(sizeof(T));
    return new (mem) T(args...);
}

// destroy c++ object
template <typename T>
void VFree(T* ptr) {
    ptr->~T();
    v_free(static_cast<void*>(ptr));
}
}