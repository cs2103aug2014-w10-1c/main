// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef YOU_QUERYENGINE_STDAFX_H_
#define YOU_QUERYENGINE_STDAFX_H_

#include <SDKDDKVer.h>

#include <cstdint>
#include <functional>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>
#include <unordered_set>

#pragma warning(push)
#pragma warning(disable: 4510 4512 4555 4610)
#include <boost/tokenizer.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/variant.hpp>
#pragma warning(pop)

#endif  // YOU_QUERYENGINE_STDAFX_H_
