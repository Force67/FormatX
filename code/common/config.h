#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <variant>

#include <type_traits>
#include <yaml-cpp/yaml.h>

namespace config {

struct optBase {
    inline explicit optBase()
        : next(root()) {
        root() = this;
    }

    static inline optBase*& root() noexcept {
        static optBase* s_root{nullptr};
        return s_root;
    }

    virtual void decode(const YAML::Node& v) = 0;
    virtual void encode(YAML::Node& v) = 0;

    optBase* next = nullptr;

    const char* name;
    const char* desc;
};

template <typename DataType>
class opt : public optBase {
public:
    // non copyable
    opt(const opt&) = delete;
    opt& operator=(const opt&) = delete;

    constexpr inline explicit opt(const char* name, const char* desc, DataType default)
        : optBase() {
        optBase::name = name;
        optBase::desc = desc; 
        data = default;
    }

    // accessors
    DataType* operator->() const noexcept {
        return std::pointer_traits<DataType*>::pointer_to(*data);
    }

    DataType& operator&() const noexcept {
        return data;
    }

    operator DataType() const noexcept {
        return data;
    }

    // convert to
    void decode(const YAML::Node& v) override {
         data = v.as<DataType>(data);
    }

    // convert at
    void encode(YAML::Node& v) override {
         v = YAML::Node(data);
    }

private:
    DataType data;
};

bool load();
bool save();
} // namespace config