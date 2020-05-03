#pragma once

/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <string>
#include <optional>

namespace utl {

enum class app_path {
    home,
    self,
    count
};

using optional_path = std::optional<std::string>;

// creates a sanitized path relative to the current executable
std::string make_abs_path(optional_path = std::nullopt);

// creates a sanitized path relative to the specified link
std::string make_app_path(app_path, optional_path = std::nullopt);

bool exists(std::string_view rel);

bool make_dir(std::string_view rel);
}