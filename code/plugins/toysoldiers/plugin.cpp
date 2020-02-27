
// Copyright (C) 2019-2020 Force67, Greavesy

#include <plugintraits.h>
#include <utl/File.h>
#include "sac_arc.h"
#include "types.h"

namespace {
enum fileType {
    SACB,
};

bool accept(utl::File& file, fileDesc& out) {
    if (file.GetSize() > sizeof(SACHeader)) {
        u32 fileID;
        char sigMagic[6];
        u16 version;

        /*in reality two dwords*/
        auto pos = file.Tell();
        file.Read(fileID);
        file.Read(sigMagic);
        file.Read(version);
        if ((std::strncmp(sigMagic, "!SigB!", 6) == 0) && (version == 2)) {
            if (fileID == FileIdentifiers::SAC) {
                out = {SACB, "Toy Soldiers SACB Archive"};
                return true;
            }
        }
    }

    return false;
}

bool init(utl::File& file, const fileDesc& in) {
    if (in.type == SACB) {
        SACFile arc;
        arc.Deserialize(file);
        return true;
    }

    return false;
}
}

EXPORT pluginLoader PLUGIN = {
    PluginVersion::V_1_0, "toysldplugin", "Toy Soldiers Plugin", ::accept, ::init, nullptr,
};