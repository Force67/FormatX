
// Copyright (C) 2019 Force67

#include <filesystem>
#include "ArcFile.h"

FileResult ArcFile::Deserialize(utl::File& file) {
    file.Read(header);

    if (std::strncmp(header.magicName, "ARCH000", 7) != 0)
        return FileResult::badmagic;

    file.Seek(static_cast<uint64_t>(header.startOffset), utl::seekMode::seek_set);

    fileList.resize(header.entryCount);
    file.Read(fileList);

    return FileResult::success;
}

FileResult ArcFile::ExtractAll(utl::File& file) {
    std::filesystem::create_directories("tboi_extract\\file");

    for (auto& e : fileList) {
        file.Seek(e.offset, utl::seekMode::seek_set);

        std::vector<uint8_t> buf(e.length);
        file.Read(buf);

        utl::File out("tboi_extract\\file\\" + std::to_string(e.hashOne & 0x7FFF),
                      utl::fileMode::write);
        if (out.IsOpen()) {
            out.Write(buf);
        } else {
            std::puts("unable to open dat shit");
        }
    }

    return FileResult::success;
}