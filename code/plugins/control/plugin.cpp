
// Copyright (C) 2019 Force67

#include <plugintraits.h>
#include "RmdpFIle.h"

namespace plugin 
{
	static u32 accept(utl::File& file)
	{
		if (file.GetSize() > sizeof(RmdpHeader)) {
			uint32_t magic = 0;
			file.Read(magic);

			/*todo: verify file versioN!!!*/
			if (magic == rmdpMagic)
				return 1;
		}

		return -1;
	}

	static bool init(utl::File& file, u32 type)
	{
		if (type == 1) {
			RmdpArc arc("");
			arc.Deserialize(file);
		}

		return true;
	}

	static bool init(const char* name)
	{
		return true;
	}

	static void shutdown()
	{

	}
}

EXPORT pluginDesc PLUGIN = {
	PluginVersion::V_1_0,
	"controlPlugin",
	"Control Plugin",
	plugin::accept,
	plugin::init,
	plugin::shutdown,
};