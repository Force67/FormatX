#pragma once

// Copyright (C) Force67 2019

#include <utl/File.h>
#include <fileformat.h>

static constexpr uint32_t rmdpMagic = 0x44424553; // SEBD

struct RmdpHeader
{
	uint32_t magic;
	uint32_t unk1;
	uint32_t unk2;
	uint32_t unk4;
	char platformName[4];
	uint32_t unk5;
};

class RmdpArc : public IFileFormat
{
	RmdpHeader hdr{};

	// custom
	struct ArcEntry
	{
		std::string name;
		uint32_t offset;
		uint32_t hash;
		uint64_t size;
	};
	std::vector<ArcEntry> entries;

public:

	FileResult Deserialize(utl::File&) override;

	// todo: find better way
	bool ParseDescriptor(const std::string&);
};