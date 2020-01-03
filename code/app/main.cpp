
// Copyright (C) 2019-2020 Force67

#include <cstdint>
#include <cstdio>
#include <filesystem>

#include <utl/path.h>
#include <utl/File.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <plugintraits.h>

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::puts("Usage: formatx <filename>");
		return 0;
	}

	if (!fs::exists(argv[1])) {
		std::puts("file does not exist");
		return 0;
	}

	utl::File file(argv[1]);

	std::vector<void*> handles;

	for (auto& p : fs::directory_iterator(utl::make_abs_path(L"plugins"))) {
		if (p.is_regular_file() && p.path().extension() == L".dll") {
			auto *hlib = LoadLibraryW(p.path().c_str());
			if (hlib) {
				auto* info = reinterpret_cast<pluginDesc*>(GetProcAddress(hlib, "PLUGIN"));
				if (info) {
					std::printf("registered %s\n", info->prettyName);
					handles.push_back(hlib);		

					/* don't unload it here yet */
					file.Seek(0, utl::seekMode::seek_set);
					u32 ret = info->accept(file);
					if (ret == -1)
						continue;
					else
						info->init(file, ret);
				}
				else
					FreeLibrary(hlib);
			}
		}
	}

	for (auto h : handles)
		FreeLibrary(static_cast<HMODULE>(h));

	return 0;
}