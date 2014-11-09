//@author A0097630B
#include "stdafx.h"
#include "algorithms.h"

namespace You {
namespace Utils {

template<typename CharT>
bool similar_to<std::basic_string<CharT>>::operator()(
	const StringType& lhs,
	const StringType& rhs) {
	std::vector<StringType> needles;
	boost::split(needles, rhs, boost::is_space(), boost::token_compress_on);

	std::vector<StringType> haystacks;
	boost::split(haystacks, lhs, boost::is_space(), boost::token_compress_on);

	return std::all_of(begin(needles), end(needles),
		std::bind(&isAnyNeedleInHaystacks, std::placeholders::_1, haystacks));
}

template<typename CharT>
bool similar_to<std::basic_string<CharT>>::isAnyNeedleInHaystacks(
	const StringType& needle,
	const std::vector<StringType>& haystacks) {
	assert(!haystacks.empty());
	return std::any_of(begin(haystacks), end(haystacks),
		std::bind(&isNeedleInHaystack, needle, std::placeholders::_1));
}

template<typename CharT>
bool similar_to<std::basic_string<CharT>>::isNeedleInHaystack(
	const StringType& needle,
	const StringType& haystack) {
	if (needle.empty()) {
		return false;
	} else {
		return haystack.find(needle) != StringType::npos;
	}
}

template struct similar_to<std::wstring>;
template struct similar_to<std::string>;

}  // namespace Utils
}  // namespace You
