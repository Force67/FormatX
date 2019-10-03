#pragma once

// Copyright (C) Force67 2019

#include <utl/File.h>
#include <fileformat.h>
#include <zlib.h>
#include <fstream>

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
	uint32_t size; //verified
	uint32_t sizeCompressed;
	uint32_t offset;
	uint16_t flags1;
	uint16_t flags2;
};

//4+4+4+4+2+2+4

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

static_assert(sizeof(TigerHeader) == 52, "Bad V3/V4 Header size");

std::unique_ptr<IFileFormat> CreateTigerFMT(utl::File&);