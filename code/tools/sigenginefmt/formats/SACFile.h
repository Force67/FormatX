#pragma once

// Copyright (C) Greavesy 2019

#include <utl/File.h>

struct SACHeader
{
	uint32_t sacMagic0;
	uint32_t sigMagic0;
	uint32_t sigVersion;
	uint32_t sacMagic1;
	char padding[60];
	uint32_t dataOffset;
	uint32_t dictionaryOffset;
	uint32_t numEntries;
};

struct SACEntry
{
	uint32_t nameOffset;
	uint32_t nameSize;
	uint32_t unkOffset;
	uint32_t isCompressed;
	uint32_t dataOffset;
	uint32_t uncompressedSize;
	uint32_t compressedSize;
	uint32_t unk0;
	uint32_t unk1;
	uint32_t unk2;
	uint32_t unk3;
	uint32_t unk4;
	uint32_t unk5;
	uint32_t unk6;
};

static_assert(sizeof(SACEntry) == 56, "Bad Entry size");

class SACFile
{
	utl::File& file;
	SACHeader header{};
	std::vector<SACEntry> filelist{};

	static constexpr uint32_t sigVersion = 2;
	static constexpr uint32_t sigMagic = 0x67695321;
	static constexpr uint32_t sacMagic = 0xF28E6F5E;

	std::string getEntryName(SACEntry entry);

public:

	SACFile(utl::File&);
	bool Validate();
	void ExportFiles();
	void DebugPrintEntries();
};