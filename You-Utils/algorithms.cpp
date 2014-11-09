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

	return std::any_of(begin(haystacks), end(haystacks),
		std::bind(&needlesInHaystack, needles, std::placeholders::_1));
}

template<typename CharT>
bool similar_to<std::basic_string<CharT>>::needlesInHaystack(
	const std::vector<StringType>& needles,
	const StringType& haystack) {
	if (haystack.empty()) {
		return false;
	} else {
		return std::any_of(begin(needles), end(needles),
			std::bind(&needleInHaystack, std::placeholders::_1, haystack));
	}
}

template<typename CharT>
bool similar_to<std::basic_string<CharT>>::needleInHaystack(
	const StringType& needle,
	const StringType& haystack) {
	if (needle.empty()) {
		return false;
	} else {
		return haystack.find(needle) != StringType::npos;
	}
}

}  // namespace Utils
}  // namespace You
