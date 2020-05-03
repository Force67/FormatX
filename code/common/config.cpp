
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "config.h"

#include <utl/file.h>
#include <utl/path.h>

#include "utl/logger/logger.h"

// TODO: this shit needs serious fixing

namespace config {

bool load() {
    auto path = utl::make_app_path(utl::app_path::home, "config.yml");

    // no config? write one
    if (!utl::exists(path))
        return save();

    try {
        auto doc = YAML::LoadFile(path);

        // go through static opt list
        for (optBase* i = optBase::root(); i; i = i->next) {
            if (!i->name) {
                LOG_WARNING("Ignoring unnamed opt {}", fmt::ptr(i));
                continue;
            }

            std::string str = i->name;

            if (str.empty())
                continue;

            const auto pos = str.find_first_of('.');
            YAML::Node node;

            // no sub families
            if (pos == -1)
                node = doc[str];
            else
                node = doc[str.substr(0, pos)][&str[pos + 1]];

            if (node)
                i->decode(node);
            else
                LOG_WARNING("Unable to decode opt {}", i->name);
        }

    } catch (std::exception& ex) {
        LOG_WARNING("Exception while parsing config: {}", ex.what());
    }

    // optBase::root() = nullptr;
    return true;
}

bool save() {
    auto path = utl::make_app_path(utl::app_path::home, "config.yml");

    try {
        utl::File out(path, utl::fileMode::write);
        if (!out.IsOpen())
            return false;

        YAML::Node doc, val;
        for (optBase* i = optBase::root(); i; i = i->next) {
            if (!i->name) {
                LOG_WARNING("Ignoring unnamed opt {}", fmt::ptr(i));
                continue;
            }

            std::string str = i->name;
            if (str.empty()) continue;
            
            i->encode(val);

            const auto pos = str.find_first_of('.');

            // no sub families
            if (pos == -1)
                doc[str] = val;
            else 
                doc[str.substr(0, pos)][&str[pos + 1]] = val;
        }

        YAML::Emitter emit;
        emit << doc;

        out.Write(emit.c_str(), emit.size());
    } catch (const std::runtime_error& ex) {
        LOG_WARNING("Exception while writing config: {}", ex.what());
        return false;
    }

    return true;
}
} // namespace config