
// Copyright (C) 2019 Force67

#include "formats/RmdpFile.h"

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::puts("Usage: NorthlightFMT <filename>");
		return 0;
	}

	utl::File file(argv[1]);
	if (!file.IsOpen()) {
		std::puts("Failed to open file!");
		return -1;
	}

	if (std::strstr(argv[1], ".rmdp") && file.GetSize() > sizeof(RmdpHeader)) {
		RmdpFile rfile(file);
		if (rfile.ExtractFile()) {
		}
	}

	getchar();
	return 0;
}