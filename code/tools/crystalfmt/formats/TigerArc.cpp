
// Copyright (C) Force67 2019

#include "TigerArc.h"

TigerArc::TigerArc(utl::File& f) :
	file(f)
{}

bool TigerArc::Validate()
{
	if (file.IsOpen()) {
		file.Read(header);

		if (header.magic != tigerMagic)
			return false;

		if (header.version != 4) {
			std::puts("[!] Bad Tiger version");
			return false;
		}

		if (std::strcmp(header.platformName, "orbis-w") == 0) {
			std::printf("[+] Opening PS4-Bigfile with %d entries\n", header.numEntries);
		}
		else if (std::strcmp(header.platformName, "pcx64-w") == 0) {
			std::printf("[+] Opening PC-Bigfile with %d entries\n", header.numEntries);
		}
		else return false;

		// read in all entries
		filelist.resize(header.numEntries);
		file.Read(filelist);
	}

	return true;
}

void TigerArc::ExtractFile(const TigerEntry& entry)
{
	auto prev = file.Tell();
	file.Seek(entry.offset, utl::seekMode::seek_set);

	std::vector<uint8_t> data(entry.size);
	file.Read(data);


}

void TigerArc::ExtractAll()
{
	for (auto& e : filelist)
	{
		if (e.sizeCompressed != 0) {
			std::printf("[!]: We cant handle compressed tiger entry yet (%x)\n", e.nameHash);
			continue;
		}

		ExtractFile(e);
	}
}

void TigerArc::DebugPrintEntries()
{
	for (auto& e : filelist) {
		std::printf("Entry: Hash: %x | Lang: %x | Size: (Z: %d, U :%d) | Offset %x | Flags (1: %x, 2: %x)\n",
			e.nameHash, e.language, e.sizeCompressed, e.size, e.offset, e.flags1, e.flags2);
	}
}