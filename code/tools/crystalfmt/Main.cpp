
// Copyright (C) 2019 Force67

#include "formats/BigFile.h"

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::puts("Usage: CrystalFMT <filename>");
		return 0;
	}

	utl::File file(argv[1]);
	if (!file.IsOpen()) {
		std::puts("Failed to open file!");
		return -1;
	}

	if (std::strstr(argv[1], ".tiger") && file.GetSize() > sizeof(TigerHeader)) {
		BigFile bfile(file);
		if (bfile.ExtractFile()) {
			bfile.DebugPrintEntries();
		}
	}

	getchar();
	return 0;
}