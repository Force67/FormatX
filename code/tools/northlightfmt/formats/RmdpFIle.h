#pragma once

// Copyright (C) Force67 2019

#include <utl/File.h>

struct RmdpHeader
{
	uint32_t magic;
	uint32_t unk1;
	uint32_t unk2;
	uint32_t unk4;
	char platformName[4];
	uint32_t unk5;
};

class RmdpFile
{
	utl::File& file;
	RmdpHeader header{};
	
	static constexpr uint32_t rmdpMagic = 0x44424553; // SEBD

public:

	RmdpFile(utl::File&);
	bool ExtractFile();
};