
/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <algorithm>

#include "path.h"
#include "string_util.h"

#include <Windows.h>
#include <shlobj.h> // for SHGetFolderPath

namespace utl {

std::string make_abs_path(optional_path relative) {
    static std::string filePath;
    if (filePath.empty()) {
        wchar_t buf[2048]{};

        GetModuleFileNameW(nullptr, buf, 2048);
        wchar_t* dirPtr = std::wcsrchr(buf, L'\\');
        dirPtr[1] = L'\0';

        filePath = utl::utf16_to_utf8(buf);
    }

    auto newPath = !relative ? filePath : filePath + *relative;

    // sanitize the path (this is a windows thing, on linux & osx we
    // need to do it the other way around)
    std::replace(newPath.begin(), newPath.end(), '/', '\\');

    return newPath;
}

std::string get_home_dir() {
    wchar_t* localPath = nullptr;

    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &localPath))) {
        auto newPath = utl::utf16_to_utf8(localPath);
        CoTaskMemFree(localPath);;
        return newPath;
    }

    BUGCHECK();
    return "";
}

bool exists(std::string_view rel) {
    if (rel.empty())
        return false;

    auto newPath = utl::utf8_to_utf16_w(std::string(rel));
    return GetFileAttributesW(newPath.c_str()) != INVALID_FILE_ATTRIBUTES;
}

bool make_dir(std::string_view rel) {
    if (rel.empty())
        return false;

    auto newPath = utl::utf8_to_utf16_w(std::string(rel));

    // allows for recursive directory creation
    return SHCreateDirectoryEx(nullptr, newPath.c_str(), nullptr);
}
} // namespace utl