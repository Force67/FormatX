#pragma once

// Copyright (C) 2019 Force67

#include <cstdint>

namespace utl {
	class File;
}

class IFileFormat
{
public:

	virtual ~IFileFormat() = default;
	virtual void Serialize(utl::File&) = 0;
	virtual void Deserialize(const utl::File&) = 0;
};