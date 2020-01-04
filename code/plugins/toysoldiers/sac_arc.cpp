// Copyright (C) Greavesy 2019
#include <filesystem>
#include <iostream>
#include <sstream>
#include <zlib.h>

#include "sac_arc.h"

namespace fs = std::filesystem;

std::string SACFile::getEntryName(SACEntry entry)
{
	auto offset = (entry.nameOffset & 0x7FFFFFFF) - 4;
	file.Seek(offset, utl::seekMode::seek_set);
	std::string name;
	name.resize(entry.nameSize);
	file.Read(name.data(), entry.nameSize);
	return name;
}

SACFile::SACFile(utl::File& f) :
	file(f)
{}

bool SACFile::Validate()
{
	if (file.IsOpen()) {
		file.Read(header);
		file.Seek(((header.dictionaryOffset) & 0x7FFFFFFF) + 4, utl::seekMode::seek_set);
		// read in all entries
		filelist.resize(header.numEntries);
		file.Read(filelist);
	}

	return true;
}

void SACFile::ExportFiles()
{
	if (file.IsOpen())
	{
		for (auto& e : filelist) {
			//get entry name and then print into console, store name too.
			auto entryName = this->getEntryName(e);
			std::printf("%s\n", entryName.c_str());
			std::istringstream ns;
			ns.str(entryName);

			//build directory path.
			std::string dirPath;
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

			//option 1
			utl::File out(entryName, utl::fileMode::write);
			out.Write(data);
			out.Close();

			//option 2
			//FILE* out2;
			//fopen_s(&out2, entryName.c_str(), "wb");
			//if (out2 != NULL)
			//{
			//	fwrite(data.data(), sizeof(uint8_t), data.size(), out2);
			//	fclose(out2);
			//}
		}
	}
}

void SACFile::DebugPrintEntries()
{
	if (file.IsOpen())
	{
		for (auto& e : filelist) {
			std::string& name = this->getEntryName(e);
			std::printf("Entry: Name %s \n", name.c_str());
			//std::printf("Entry: Hash: %x | Lang: %x | Size: (Z: %d, U :%d) | Offset %x | Flags (1: %x, 2: %x)\n",
			//	e.nameHash, e.language, e.sizeCompressed, e.size, e.offset, e.flags1, e.flags2);
		}
	}

}