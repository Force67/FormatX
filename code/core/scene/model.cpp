
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <logger/logger.h>

#include "model.h"

namespace scene {

Model::Model() {
}

bool Model::loadFromFile(const std::string& path) {
    Assimp::Importer imp;
    const auto* scene =
        imp.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_ERROR("Failed to load model {}", imp.GetErrorString());
        return false;
    }

    return true;
}
}