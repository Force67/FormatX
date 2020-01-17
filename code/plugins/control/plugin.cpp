
// Copyright (C) 2019 Force67

#include <plugintraits.h>
#include "RmdpFIle.h"

namespace 
{
	enum fileType
	{
		ARC,
	};

	static bool accept(utl::File& file, fileDesc &out)
	{
		if (file.GetSize() > sizeof(RmdpHeader)) {
			uint32_t magic = 0;
			file.Read(magic);

			/*todo: verify file versioN!!!*/
			if (magic == rmdpMagic) {
				out = { ARC, "Control Archive" };
				return true;
			}
		}

		return false;
	}

	static bool init(utl::File& file, const fileDesc &in)
	{
		if (in.type == ARC) {
			RmdpArc arc("");
			arc.Deserialize(file);
		}

		return true;
	}
}

EXPORT pluginLoader PLUGIN = {
	PluginVersion::V_1_0,
	"controlPlugin",
	"Control Plugin",
	::accept,
	::init,
	nullptr,
};