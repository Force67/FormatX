
// Copyright (C) 2019 Force67

#include "formats/TigerArc.h"

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

	if (std::strstr(argv[1], ".tiger") && file.GetSize() > 0) {

		// can we handle this tiger file?
		int v = ValidateTiger(file);
		if (v == 0) {
			std::puts("[!] Unknown tiger file!");
			return -2;
		}

		switch (v) {
		case 3:
		{
			TR9Tiger arc(file);
			arc.ExtractAll();
			break;
		} 
		}
	}

	getchar();
	return 0;
}