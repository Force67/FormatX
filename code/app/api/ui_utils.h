#pragma once

#ifdef BUILDING_FORMATX
#define FX_API __declspec(dllexport)
#else
#define FX_API __declspec(dllimport)
#endif

namespace fx {}