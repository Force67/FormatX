
// Copyright (C) 2019 Force67

#include <filesystem>
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
		std::string arcdir(argv[1]);

		RmdpArc arc;
		auto arcroot = arcdir.substr(0, arcdir.length() - 4);
		{
			auto xmlDesc = arcroot + "xml";
			if (std::filesystem::exists(xmlDesc)) {
				arc.ParseDescriptor(xmlDesc);
			}
		}

		auto res = arc.Deserialize(file);
		if (res != FileResult::success) {
			std::puts("unable to deserialize file");
			return -2;
		}
	}

	getchar();
	return 0;
}