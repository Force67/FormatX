#include <filesystem>

// Copyright (C) 2019 Greavesy

#include "formats/SACFile.h"
namespace fs = std::filesystem;

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::puts("Usage: SigEngineFMT <filename>");
		return 0;
	}

	if (std::strstr(argv[1], ".sacb")) {
		SACFile sac;
		sac.SetName(std::string(argv[1]));
		utl::File file(argv[1]);

		if (file.IsOpen() && file.GetSize() > sizeof(SACHeader)) {
			if (sac.Deserialize(file) == FileResult::success) {
				sac.ExtractAll(file);
			}
			else {
				std::printf("Failed to deserialize %s!", argv[1]);
			}
		}
		else {
			std::puts("Failed to open file!");
			return -1;
		}
	}
	else if (fs::is_directory(argv[1]))
	{
		SACFile sac;
		sac.SetName(std::string(argv[1]));
		std::string packName(argv[1]);
		packName += "1.sacb";
		utl::File file(packName, utl::fileMode::write);
		sac.Serialize(file);
	}

	getchar();
	return 0;
}