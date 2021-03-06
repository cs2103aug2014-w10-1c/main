//@author A0114171W
#pragma once
#ifndef YOU_DATASTORE_TASK_TYPEDEFS_H_
#define YOU_DATASTORE_TASK_TYPEDEFS_H_

#include <cstdint>
#include <string>
#include <unordered_map>

namespace You {
namespace DataStore {

/// typedef for Task ID
typedef int64_t TaskId;

/// \name Typedefs for serializing to xml
/// @{
typedef std::wstring Key;
typedef std::wstring Value;
typedef std::pair<Key, Value> KeyValuePair;
typedef std::unordered_map<Key, Value> KeyValuePairs;
/// @}

}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_TASK_TYPEDEFS_H_
