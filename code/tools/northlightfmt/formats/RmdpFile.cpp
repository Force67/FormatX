
// Copyright (C) Force67 2019

#include "RmdpFile.h"
#include <filesystem>
#include <pugixml.hpp>

FileResult RmdpArc::Deserialize(utl::File& file)
{
	file.Read(hdr);

/*	if (hdr.magic != rmdpMagic)
		return FileResult::badmagic;

	if (std::strcmp(hdr.platformName, "W_64") != 0)
		return FileResult::badplatform;
		*/
	for (auto& e : entries) {

		file.Seek(e.offset, utl::seekMode::seek_set);

		std::vector<uint8_t> data(e.size);
		file.Read(data);

		utl::File out("northlight_extract\\test\\" + e.name, utl::fileMode::write);
		if (out.IsOpen()) {
			out.Write(data);
		}
	}

	return FileResult::success;
}

bool RmdpArc::ParseDescriptor(const std::string &path)
{
	pugi::xml_document doc;
	auto res = doc.load_file(path.c_str());
	if (!res)
		return false;

	auto info = doc.child("PackFileDirectory");
	int32_t dc = info.attribute("directoryCount").as_int();
	int32_t fc = info.attribute("fileCount").as_int();
	//auto dir = info.attribute("storeFile").as_string();
	if (fc == 0) {
		return false;
	}

	std::filesystem::create_directories("northlight_extract\\test");

	entries.resize(fc);

	for (auto& tool : info.child("directory").children("directory")) {
		auto* nodeName = tool.attribute("name").as_string();

		std::printf("[+] Directory %s\n", nodeName);
		//std::filesystem::create_directory("")

		// test
		if (std::strcmp(nodeName, "ambiance_presets") == 0) {
			for (auto& file : tool) {

				auto& FE = entries.emplace_back();
				FE.name = file.attribute("name").as_string();
				FE.offset = file.attribute("offset").as_ullong();
				FE.hash = 0;
				FE.size = file.attribute("size").as_ullong();

				std::printf("name %s size %d\n", FE.name.c_str(), FE.size);
			}
		}
	}

	return true;
}