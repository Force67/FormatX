
#include "SDS.h"

namespace maf3
{
	bool SDSFile::load(utl::File& file)
	{
		SDSVersionIndependentHeader header{};
		file.Read(header);

		if (std::strncmp(header.sdsMagic, "SDS", 3))
			return false;

		// old gen bug check
		if (std::strncmp(header.platformName, "X360", 4) ||
			std::strncmp(header.platformName, "PS3", 3)) {
			std::puts("SDSFile:Load: Unsupported platform");
			return false;
		}

		SDSVersionSpecificHeader specific{};
		file.Read(specific);
	}
}