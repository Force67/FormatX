
// Copyright (C) 2019 Force67

#include <plugintraits.h>

#include "TigerArc.h"

namespace plugin 
{
	static u32 accept(utl::File& file)
	{
		if (file.GetSize() > sizeof(TigerHeader)) {
			uint32_t magic = 0;

			file.Read(magic);
			if (magic == tigerMagic)
				return 1;
		}

		return -1;
	}

	static bool init(utl::File& file, u32 type)
	{
		if (type == 1) {
			auto arcReader = CreateTigerFMT(file);
			arcReader->ExtractAll(file);
			return true;
		}

		return false;
	}

	static void shutdown()
	{

	}
}

EXPORT pluginDesc PLUGIN = {
	PluginVersion::V_1_0,
	"trplugin",
	"Tomb Raider Plugin",
	plugin::accept,
	plugin::init,
	plugin::shutdown,
};