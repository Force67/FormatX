
// Copyright (C) Force67 2019

#include "RmdpFIle.h"

RmdpFile::RmdpFile(utl::File& f) :
	file(f)
{}

bool RmdpFile::ExtractFile()
{
	if (file.IsOpen()) {
		file.Read(header);

		if (header.magic != rmdpMagic)
			return false;

		if (std::strcmp(header.platformName, "W_64") != 0)
			return false;
	}

	return true;
}