#pragma once

#include <utl/File.h>

namespace maf3
{
	struct SDSVersionIndependentHeader
	{
		char sdsMagic[4];
		uint32_t unk;
		char platformName[4];
		uint32_t fnvChecksum;
	};

	struct SDSVersionSpecificHeader
	{
		char pad[56];
	};

	static_assert(sizeof(SDSVersionIndependentHeader) == 16);
	static_assert(sizeof(SDSVersionSpecificHeader) == 56);

	class SDSFile
	{
	public:

		bool load(utl::File&);
	};
}