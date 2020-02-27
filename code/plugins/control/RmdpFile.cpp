
// Copyright (C) Force67 2019

#include <cstdio>
#include <filesystem>
#include <pugixml.hpp>
#include <utl/path.h>
#include "RmdpFile.h"

RmdpArc::RmdpArc(const std::string& path) : path(path) {
    size_t pos = path.find_last_of('\\');
    if (pos != std::string::npos) {
        extractBase = "northlight_extract\\" + path.substr(pos + 1, path.length() - 6 - pos);
    } else {
        extractBase = "northlight_extract\\unknown";
    }
}

FileResult RmdpArc::Deserialize(utl::File& file) {
    file.Read(hdr);

    if (hdr.magic == rmdpMagic) {
        if (std::strcmp(hdr.platformName, "W_64") != 0)
            return FileResult::badplatform;
    }

    // see if we have the descriptor available
    auto arcroot = path.substr(0, path.length() - 4);
    {
        auto xmlDesc = arcroot + "xml";
        if (std::filesystem::exists(xmlDesc)) {
            ParseDescriptor(xmlDesc);
        } else
            return FileResult::badmagic;
    }

    return FileResult::success;
}

FileResult RmdpArc::ExtractAll(utl::File& file) {
    std::printf("Progress: [");

    int ec = 0;
    int lv = 0;

    for (auto& e : entries) {

        // if we registered valid data...
        if (e.offset) {
            file.Seek(e.offset, utl::seekMode::seek_set);

            std::vector<uint8_t> data(e.size);
            file.Read(data);

            auto namefull = utl::make_abs_path(e.pathNameFull);
            utl::File out(namefull, utl::fileMode::write);
            if (out.IsOpen()) {
                // std::printf("[+] Extracting file %s\n", namefull.c_str());
                out.Write(data);
            } else {
                std::printf("[!] Unable to extract file to %s\n", namefull.c_str());
            }
        }

        const int percent = (ec / (entries.size() / 100));
        if (lv != percent) {
            if (percent == 100)
                std::printf("#]\n");
            else
                std::printf("#");

            lv = percent;
        }

        ec++;
    }

    return FileResult::success;
}

void RmdpArc::TraverseDirectory(pugi::xml_node& node) {
    if (std::strcmp(node.name(), "directory") == 0) {
        // and for all of our subdirectories...
        for (pugi::xml_node& e : node) {
            if (std::strcmp(e.name(), "directory") == 0) {
                TraverseDirectory(e);
            } else {
                XmlEntry entry{};
                entry.offset = e.attribute("offset").as_ullong();
                entry.size = e.attribute("size").as_ullong();

                auto target = e.attribute("originalPath").as_string();
                if (target) {
                    std::string tasstr(target);
                    size_t pos = tasstr.find("runtimedata");
                    if (pos != std::string::npos) {
                        pos += 11;

                        auto outFile = extractBase + tasstr.substr(pos, std::strlen(target) - pos);

                        // another horrible hack
                        auto outPath = outFile.substr(0, outFile.find_last_of('\\'));
                        std::filesystem::create_directories(outPath);

                        XmlEntry entry{};
                        entry.pathNameFull = outFile;
                        entry.offset = e.attribute("offset").as_ullong();
                        entry.size = e.attribute("size").as_ullong();

                        entries.emplace_back(entry);
                    }
                }
            }
        }
    }
}

bool RmdpArc::ParseDescriptor(const std::string& path) {
    pugi::xml_document doc;
    auto res = doc.load_file(path.c_str());
    if (!res)
        return false;

    auto info = doc.child("PackFileDirectory");
    int32_t dc = info.attribute("directoryCount").as_int();
    int32_t fc = info.attribute("fileCount").as_int();
    if (fc == 0) {
        return false;
    }

    std::filesystem::create_directories(extractBase);

    entries.resize(fc);

    for (auto& tool : info.child("directory").children("directory")) {
        TraverseDirectory(tool);
    }

    return true;
}