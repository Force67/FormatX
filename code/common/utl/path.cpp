
/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <algorithm>

#include "path.h"

#ifdef _WIN32
#include <Windows.h>
#include <shlobj.h> // for SHGetFolderPath
#endif

#include <base.h>

namespace utl {

std::string get_home_dir();

std::string make_app_path(app_path pathType, optional_path relative) {
    // clearly a bug
    if (pathType == app_path::count)
        BUGCHECK();

    static std::string s_defaultPaths[static_cast<int>(app_path::count)];
    auto& newPath = s_defaultPaths[static_cast<int>(pathType)];

    if (newPath.empty()) {
        switch (pathType) { 
            case app_path::home: {
                newPath = get_home_dir() + PATH_SEP PRJ_COMPANY PATH_SEP PRJ_NAME PATH_SEP;
            break;
            }
            case app_path::self:
            default:
                return make_abs_path(relative);
        }

        if (!exists(newPath))
            make_dir(newPath);
    }

    return !relative ? newPath : newPath + *relative;
}
} // namespace utl