#pragma once

// Copyright (C) 2019 Force67

#include <fileformat.h>
#include <utl/File.h>

#pragma pack(push, 1)
struct ArcHeader {
    char magicName[7];
    uint8_t tableIdx;
    uint32_t startOffset;
    uint16_t entryCount;
};

struct ArcEntry {
    uint32_t hashOne;
    uint32_t hashTwo;
    uint32_t offset;
    uint32_t length;
    uint32_t checksum;
};
#pragma pack(pop)

static_assert(sizeof(ArcHeader) == 14, "bad ArcHeader size");
static_assert(sizeof(ArcEntry) == 20, "bad ArcEntry size");

class ArcFile final : public IFileFormat {
    ArcHeader header{};
    std::vector<ArcEntry> fileList{};

public:
    FileResult Deserialize(utl::File&) override;
    FileResult ExtractAll(utl::File&) override;
};