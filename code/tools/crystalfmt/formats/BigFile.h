#pragma once

// Copyright (C) Force67 2019

#include <utl/File.h>

struct TigerHeader
{
	uint32_t magic;
	uint32_t version; // rise is v4
	uint32_t unk;
	uint32_t numEntries;
	uint32_t unk2;
	char platformName[32];
};

struct TigerEntry
{
	uint32_t nameHash;
	uint32_t language;
	uint32_t size;
	uint32_t sizeCompressed;
	uint32_t offset;
	uint16_t flags1;
	uint16_t flags2;
};

static_assert(sizeof(TigerEntry) == 24, "Bad Entry size");

class BigFile
{
	utl::File& file;
	TigerHeader header{};
	std::vector<TigerEntry> filelist{};

	static constexpr uint32_t tigerMagic = 0x53464154;

public:

	BigFile(utl::File&);
	bool ExtractFile();

	void DebugPrintEntries();
};