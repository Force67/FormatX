
// Copyright (C) 2019 Force67

#include <plugintraits.h>
#include <utl/File.h>

#include "ArcFile.h"

namespace plugin 
{
	static u32 accept(utl::File& file)
	{
		if (file.GetSize() > sizeof(ArcHeader)) {
			char magicName[7];
			if (std::strncmp(magicName, "ARCH000", 7) != 0)
				return 1;
		}

		return -1;
	}

	static bool init(utl::File& file, u32 type)
	{
		if (type == 1) {
			ArcFile arc;
			arc.Deserialize(file);
		}

		return true;
	}

	static void shutdown()
	{

	}
}

EXPORT pluginDesc PLUGIN = {
	PluginVersion::V_1_0,
	"tboiplugin",
	"The Binding of Isaac Plugin",
	plugin::accept,
	plugin::init,
	plugin::shutdown,
};