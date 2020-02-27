#pragma once

// Copyright (C) Force67 2019

#include <fileformat.h>
#include <utl/File.h>

static constexpr uint32_t rmdpMagic = 0x44424553; // SEBD

struct RmdpHeader {
    uint32_t magic;
    uint32_t unk1;
    uint32_t unk2;
    uint32_t unk4;
    char platformName[4];
    uint32_t unk5;
};

namespace pugi {
class xml_node;
}

class RmdpArc final : public IFileFormat {
    RmdpHeader hdr{};

    const std::string& path;
    std::string extractBase;

    // custom
    struct XmlEntry {
        std::string pathNameFull;
        uint32_t offset;
        uint64_t size;
    };
    std::vector<XmlEntry> entries;

    void TraverseDirectory(pugi::xml_node&);
    bool ParseDescriptor(const std::string&);

public:
    RmdpArc(const std::string&);

    FileResult Deserialize(utl::File&) override;
    FileResult ExtractAll(utl::File&) override;
};