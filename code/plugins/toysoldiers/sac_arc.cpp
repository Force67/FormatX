// Copyright (C) Greavesy 2019
#include <filesystem>
#include <iostream>
#include <sstream>
#include <zlib.h>

#include "sac_arc.h"

namespace fs = std::filesystem;

std::string SACFile::getEntryName(utl::File& file, SACEntry& entry)
{
	auto offset = (entry.nameOffset & 0x7FFFFFFF) - 4;
	file.Seek(offset, utl::seekMode::seek_set);
	std::string name;
	name.resize(entry.nameSize);
	file.Read(name.data(), entry.nameSize);
	return name;
}

void recurseDirectories(std::vector<std::string>& files, const fs::directory_entry& entry)
{
	if (entry.is_directory()) {
		for (const auto& entry : fs::directory_iterator(entry))
		{
			if (entry.is_regular_file()) files.push_back(entry.path().string());
			recurseDirectories(files, entry);
		}
	}
}

void SACFile::SetName(std::string& name)
{
	this->name = name;
}

FileResult SACFile::Deserialize(utl::File& file)
{
	if (file.IsOpen()) {
		file.Read(header);
		file.Seek(((header.dictionaryOffset) & 0x7FFFFFFF) + 4, utl::seekMode::seek_set);
		// read in all entries
		filelist.resize(header.numEntries);
		file.Read(filelist);
	}

	return FileResult::success;
}

FileResult SACFile::Serialize(utl::File& file)
{
	std::vector<std::string> files = std::vector<std::string>();
	std::string dirPath = name + "\\";
	for (const auto& entry : fs::directory_iterator(dirPath))
	{
		if (entry.is_regular_file()) files.push_back(entry.path().string());
		recurseDirectories(files, entry);
	}

	for (auto& entry : files)
	{
		this->filelist.push_back(SACEntry());
	}

	this->header.sacMagic0 = sacMagic;
	this->header.sigMagic0 = sigMagic;
	this->header.sigVersion = 0x22142;
	this->header.sacMagic1 = sacMagic;
	this->header.dictionaryOffset = sizeof(SACHeader)+4;
	this->header.numEntries = this->filelist.size();

	file.Write(this->header);
	file.Write((uint32_t)0); //padding i think
	file.Write(this->filelist);

	return FileResult::success;
}

FileResult SACFile::ExtractAll(utl::File& file)
{
	if (file.IsOpen())
	{
		for (auto& e : filelist) {
			//get entry name and then print into console, store name too.
			auto entryName = this->getEntryName(file, e);
			std::printf("%s\n", entryName.c_str());
			std::istringstream ns;
			ns.str(entryName);

			//build directory path.
			std::string temp = name;
			std::string dirPath = temp.erase(this->name.size()-5) + "\\";
			for (std::string line; std::getline(ns, line, '\\'); ) {
				if (!strchr(line.c_str(), '.'))
					dirPath += (line + "\\");
			}
			if (!fs::exists(dirPath)) fs::create_directories(dirPath);

			//do preliminary temp storage.
			file.Seek(e.dataOffset, utl::seekMode::seek_set);
			std::vector<uint8_t> data(e.uncompressedSize);

			if ((e.isCompressed == 0) || (e.compressedSize == e.uncompressedSize)) //just incase isCompressed flag not set.
			{
				file.Read(data);
			}
			else
			{
				std::vector<uint8_t> compressedData(e.compressedSize);
				file.Read(compressedData);

				//lets inflate compressed data
				int err = 0;
				z_stream stream;
				stream.zalloc = Z_NULL;
				stream.zfree = Z_NULL;
				stream.opaque = Z_NULL;
				stream.avail_in = e.compressedSize;
				stream.next_in = compressedData.data();
				err = inflateInit(&stream);
				if (err != Z_OK) __debugbreak();
				stream.next_out = data.data();
				stream.avail_out = e.uncompressedSize;
				err = inflate(&stream, Z_NO_FLUSH);
				if (err != Z_STREAM_END) __debugbreak();
				err = inflateEnd(&stream);
				if (err != Z_OK) __debugbreak();
			}
			utl::File out(temp + "\\" + entryName, utl::fileMode::write);
			out.Write(data);
			out.Close();
		}
		return FileResult::success;
	} else return FileResult::fail;
}
