
// Copyright (C) 2019 Force67

#include <plugintraits.h>
#include <utl/File.h>

#include "ArcFile.h"

namespace {
enum fileType {
    ARC,
};

static bool accept(utl::File& file, fileDesc& out) {
    if (file.GetSize() > sizeof(ArcHeader)) {
        char magicName[7];
        file.Read(magicName);
        if (std::strncmp(magicName, "ARCH000", 7) == 0) {
            out = {ARC, "The Binding of Isaac Rebirth Archive"};
            return true;
        }
    }

    return false;
}

static bool init(utl::File& file, const fileDesc& in) {
    if (in.type == ARC) {
        ArcFile arc;
        arc.Deserialize(file);
    }

    return true;
}
}

EXPORT pluginLoader PLUGIN = {
    PluginVersion::V_1_0, "tboiplugin", "The Binding of Isaac Plugin", ::accept, ::init, nullptr,
};