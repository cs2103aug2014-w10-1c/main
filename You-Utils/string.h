//@author A0097630B
#pragma once
#ifndef YOU_UTILS_STRING_H_
#define YOU_UTILS_STRING_H_

#include <iosfwd>
#include <string>

namespace You {
namespace Utils {

/// Helper to convert char strings to wchar_t strings.
///
/// \param[in] string The string to convert.
/// \param[in] count The number of characters to convert.
std::wstring toWString(const char* string, size_t count);

}  // namespace Utils
}  // namespace You

/// Overload to convert narrow to wide strings.
std::wostream& operator<<(std::wostream& stream, const std::string& string);

#endif  // YOU_UTILS_STRING_H_
