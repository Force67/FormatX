#pragma once

// Copyright (C) Force67 2019

#include <utl/File.h>

static constexpr uint32_t tigerMagic = 0x53464154;

// same on v3 and v4
struct TigerHeader
{
	uint32_t magic;
	uint32_t version; //3,4
	uint32_t partsCount;
	uint32_t numEntries;
	uint32_t unk2; // 1
	char platformName[32];
};

// new header for Shadow
struct TigerHeaderV5
{
	uint32_t magic;
	uint32_t version; //5
	uint32_t partsCount;
	uint32_t numEntries;
	uint32_t unk2; // 1
	uint32_t unk3;
	char platformName[32];
};

// TR2013
/*struct TigerEntryV3
{
	uint32_t nameHash;
	uint32_t language;
	uint32_t size;
	uint32_t sizeCompressed;
	uint32_t flags;
	uint32_t offset;
};*/

struct TigerEntryV3
{
	uint32_t nameHash;
	uint32_t locale;
	uint32_t size;
	uint32_t offset;
};

// RISE
struct TigerEntryV4
{
	uint32_t crcNameHash;
	uint32_t language;
	uint32_t size;
	uint32_t sizeCompressed;
	uint32_t offset;
	uint32_t flags;
};

// SHADOW
struct TigerEntryV5
{
	uint64_t fnvNameHash;
	uint64_t language;
	uint32_t size;
	uint32_t sizeCompressed;
	uint32_t unk;
	uint32_t offset;
};

static_assert(sizeof(TigerEntryV3) == 16, "Bad V3 Entry size");
static_assert(sizeof(TigerEntryV4) == 24, "Bad V4 Entry size");
static_assert(sizeof(TigerEntryV5) == 32, "Bad V5 Entry size");

//TODO: IFile trait, for (De)Serialization

// known Tiger Arc?
int ValidateTiger(utl::File&);

class TR9Tiger
{
	utl::File& file;

	TigerHeader hdr{};
	std::vector<TigerEntryV3> entries;

public:

	TR9Tiger(utl::File&);
	void ExtractAll();
};
