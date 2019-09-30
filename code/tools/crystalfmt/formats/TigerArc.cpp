
// Copyright (C) Force67 2019

#include <filesystem>
#include "TigerArc.h"

int ValidateTiger(utl::File& file)
{
	uint32_t magic = 0;
	uint32_t version = 0;

	file.Read(magic);
	file.Read(version);

	// 'TAFS'
	if (magic != tigerMagic)
		return 0;

	// todo: check platform

	//for now...
	if (version < 3 || version > 5)
		return 0;

	return version;
}

TR9Tiger::TR9Tiger(utl::File& f) :
	file(f)
{
	file.Seek(0, utl::seekMode::seek_set);
	file.Read(hdr);

	entries.resize(hdr.numEntries);
	file.Read(entries);
}

void TR9Tiger::ExtractAll()
{
	std::filesystem::create_directory("unp");

	int ec = 0;
	int lv = 0;
	for (auto& tre : entries) {
		file.Seek(tre.offset, utl::seekMode::seek_set);

		std::vector<uint8_t> data(tre.size);
		file.Read(data);

		utl::File writer("unp//" + std::to_string(tre.nameHash), utl::fileMode::write);
		if (writer.IsOpen()) {
			writer.Write(data);
		}

		const int percent = (ec / (hdr.numEntries / 100));
		if (lv != percent) {
			std::printf("Progress %d%% (%d/%d)\n", percent, ec, hdr.numEntries);
			lv = percent;
		}

		ec++;
	}
}