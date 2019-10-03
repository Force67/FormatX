
// Copyright (C) 2019 Force67

#include "formats/ArcFile.h"

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::puts("Usage: TboiFmt <filename>");
		return 0;
	}

	utl::File file(argv[1]);
	if (!file.IsOpen()) {
		std::puts("Failed to open file!");
		return -1;
	}

	if (std::strstr(argv[1], ".a") && file.GetSize() > 0) {
		ArcFile arc;
		if (arc.Deserialize(file) == FileResult::success) {
			arc.ExtractAll(file);
		}
		else {
			std::puts("Failed to extract .a!");
		}
	}

	getchar();
	return 0;
}