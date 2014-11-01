//@author A0097630B
#include "stdafx.h"

namespace You {
namespace Utils {

std::wstring toWString(const char* string, size_t count) {
	std::wstring result;
	result.resize(count + 1);

	size_t resultCount = result.size();
	mbstowcs_s(&resultCount, &result.front(), resultCount, string, _TRUNCATE);

	result.erase(result.begin() + resultCount - 1, result.end());
	return result;
}

}  // namespace Utils
}  // namespace You

std::wostream& operator<<(std::wostream& stream, const std::string& string) {
	return stream << You::Utils::toWString(string.c_str(), string.length());
}
