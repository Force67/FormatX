
// Copyright (C) Force67 2019

#include "BigFile.h"

BigFile::BigFile(utl::File& f) : 
	file(f)
{}

bool BigFile::ExtractFile()
{
	if (file.IsOpen()) {
		file.Read(header);

		if (header.magic != tigerMagic)
			return false;

		if (std::strcmp(header.platformName, "orbis-w") != 0 && 
			header.version == 4) {
			return false;
		}
		
		std::printf("[+] Opening PS4-Bigfile with %d entries\n", header.numEntries);

		// read in all entries
		filelist.resize(header.numEntries);
		file.Read(filelist);
	}

	return true;
}

void BigFile::DebugPrintEntries()
{
	for (auto& e : filelist) {
		std::printf("Entry: Hash: %x | Lang: %x | Size: (Z: %d, U :%d) | Offset %x | Flags (1: %x, 2: %x)\n",
			e.nameHash, e.language, e.sizeCompressed, e.size, e.offset, e.flags1, e.flags2);
	}
}