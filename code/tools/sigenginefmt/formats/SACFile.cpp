// Copyright (C) Greavesy 2019
#include <filesystem>
#include <iostream>
#include <sstream>

#include "SACFile.h"

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

		if (header.sacMagic0 != sacMagic || 
			header.sigMagic0 != sigMagic ||
			header.sacMagic1 != sacMagic)
			return false;
		
		std::printf("[+] Opening Signal Archive with %d entries\n", header.numEntries);
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
			auto entryName = this->getEntryName(e);
			std::printf("%s\n", entryName.c_str());
			if (e.isCompressed == 0)
			{
				std::vector<uint8_t> data(e.uncompressedSize);
				file.Seek(e.dataOffset, utl::seekMode::seek_set);
				file.Read(data);
				std::istringstream ns;
				ns.str(entryName);

				std::string dirPath;
				for (std::string line; std::getline(ns, line, '\\'); ) {
					if (!strchr(line.c_str(), '.'))
						dirPath += (line + "\\");
				}
				if (!fs::exists(dirPath)) fs::create_directories(dirPath);
				utl::File out(entryName, utl::fileMode::create);
				out.Write(data);
				out.Close();
			}
			else std::printf("Skipped compressed entry\n");
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