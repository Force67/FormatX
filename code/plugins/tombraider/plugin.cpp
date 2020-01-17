
// Copyright (C) 2019-2020 Force67

#include <plugintraits.h>
#include <utl/File.h>

namespace 
{
	constexpr u32 tigerMagic = 0x53464154;

	enum fileType
	{
		TIGER_TR9,
		TIGER_TR10,
		TIGER_TR11,
	};

	bool accept(utl::File& file, fileDesc &out)
	{
		if (file.GetSize() > 52) {
			u32 magic = 0;
			u32 version = 0;

			file.Read(magic);
			if (magic == tigerMagic) {
				file.Read(version);

				switch (version) {
				case 3:
					out = { TIGER_TR9, "Tomb Raider 2013 Archive" };
					return true;
				case 4:
					out = { TIGER_TR10, "Rise of the Tomb Raider Archive" };
					return true;
				case 5:
					out = { TIGER_TR11, "Shadow of the Tomb Raider Archive" };
					return true;
				default:
					out = {};
					return false;
				}
			}
		}

		return false;
	}

	bool init(utl::File& file, const fileDesc &in)
	{
		switch (in.type) {
		case TIGER_TR9: {
			__debugbreak();
			return true;
		}
		}

		/*if (type == 1) {
			auto arcReader = CreateTigerFMT(file);
			arcReader->ExtractAll(file);
			return true;
		}*/

		return false;
	}
}

EXPORT pluginLoader PLUGIN = {
	PluginVersion::V_1_0,
	"trplugin",
	"Tomb Raider Plugin",
	::accept,
	::init,
	nullptr,
};