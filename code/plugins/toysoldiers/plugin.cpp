
// Copyright (C) 2019-2020 Force67

#include <plugintraits.h>
#include <utl/File.h>

#include "sac_arc.h"

namespace
{
	constexpr u64 ksacsigMagic = 0xF28E6F5E67695321;

	enum fileType
	{
		ARC,
	};

	bool accept(utl::File& file, fileDesc& out)
	{
		if (file.GetSize() > sizeof(SACHeader)) {
			u64 sacsigMagic = 0;

			/*in reality two dwords*/
			file.Read(sacsigMagic);
			if (sacsigMagic == ksacsigMagic) {
				out = { ARC, "Toy Soldiers SAC Archive" };
				return true;
			}
		}

		return false;
	}

	bool init(utl::File& file, const fileDesc& in)
	{
		if (in.type == ARC) {
			SACFile arc(file);
			if (arc.Validate()) {
				arc.ExportFiles();
				return true;
			}
		}

		return false;
	}
}

EXPORT pluginLoader PLUGIN = {
	PluginVersion::V_1_0,
	"toysldplugin",
	"Toy Soldiers Plugin",
	::accept,
	::init,
	nullptr,
};