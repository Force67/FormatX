
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

		auto fmt = CreateTigerFMT(file);
		if (fmt) {
			auto res = fmt->Deserialize(file);

			const char* possibleErr{ nullptr };
			switch (res) {
			case FileResult::badplatform: possibleErr = "Bad Tiger Platform"; break;
			case FileResult::badversion: possibleErr = "Bad Tiger Version"; break;
			case FileResult::success:
			default:
			{
				fmt->ExtractAll(file);
			}
			}

			if (possibleErr) {
				std::printf("Tiger Extraction Err %s\n", possibleErr);
			}
		}
		else {
			std::puts("[!] Unknown tiger file version");
		}
	}

	getchar();
	return 0;
}