// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef YOU_UTILS_STDAFX_H_
#define YOU_UTILS_STDAFX_H_

#include <SDKDDKVer.h>

#include <memory>
#include <cassert>
#include <string>
#include <vector>
#include <functional>

#pragma warning(push)
#include <boost/algorithm/string.hpp>
#pragma warning(disable: 4555)
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/variant/variant.hpp>
#pragma warning(pop)

#include <windows.h>
#undef ERROR

#endif  // YOU_UTILS_STDAFX_H_
