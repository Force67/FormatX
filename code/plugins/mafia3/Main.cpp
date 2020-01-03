
// Copyright (C) 2019 Force67

#include <filesystem>
#include "formats/SDS.h"

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::puts("Usage: M3Format <filename>");
		return 0;
	}

	utl::File file(argv[1]);
	if (!file.IsOpen()) {
		std::puts("Failed to open file!");
		return -1;
	}


	getchar();
	return 0;
}