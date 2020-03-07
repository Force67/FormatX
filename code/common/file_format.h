#pragma once

// Copyright (C) 2019 Force67

#include <cstdint>

namespace utl {
class File;
}

enum class FileResult {
    success,
    badmagic,
    badversion,
    badplatform,
    fail,
};

class IFileFormat {
public:
    virtual ~IFileFormat() = default;
    virtual FileResult Serialize(utl::File&) {
        return FileResult::success;
    }
    virtual FileResult Deserialize(utl::File&) = 0;
    virtual FileResult ExtractAll(utl::File&) {
        return FileResult::success;
    }
};