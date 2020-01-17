#pragma once

// Copyright (C) 2019-2020 Greavesy

#include <utl/File.h>
#include <fileformat.h>
#include "types.h"

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
	FileIdentifiers type;
	uint32_t isCompressed;
	uint32_t dataOffset;
	uint32_t compressedSize;
	uint32_t uncompressedSize;
	uint32_t unk0;
	uint32_t unk1;
	uint32_t unk2;
	uint32_t unk3;
	uint32_t unk4;
	uint32_t unk5;
	uint32_t unk6;
};

static_assert(sizeof(SACEntry) == 56, "Bad Entry size");

class SACFile : public IFileFormat
{
	std::string name;
	SACHeader header{};
	std::vector<SACEntry> filelist{};

	static constexpr uint32_t sigVersion = 2;

	std::string getEntryName(utl::File& file, SACEntry& entry);

public:

	void SetName(std::string& name);
	FileResult Deserialize(utl::File&) override;
	FileResult Serialize(utl::File&) override;
	FileResult ExtractAll(utl::File&) override;
};