//@author A0097630B
#pragma once
#ifndef YOU_UTILS_ALGORITHMS_H_
#define YOU_UTILS_ALGORITHMS_H_

#include <algorithm>
#include <string>

namespace You {
namespace Utils {

/// Implements a functor which compares the similarity of two objects.
///
/// By default this implements equality.
///
/// \tparam T The type of the operands.
template<typename T>
struct similar_to : public std::equal_to<T> {
};

template<typename CharT>
struct similar_to<std::basic_string<CharT>> {
	typedef std::basic_string<CharT> StringType;

	/// Compares two strings for similarity.
	///
	/// This will tokenise rhs and lhs, and see if any of the tokens in rhs are
	/// substrings of lhs.
	///
	/// \return True if rhs matches anything in lhs.
	bool operator()(const StringType& lhs, const StringType& rhs);

private:
	/// Checks the needle is in any haystack.
	///
	/// \param[in] needle The needle to search for.
	/// \param[in] haystacks The haystacks to search.
	/// \return True if the needle is in any haystack. If the haystacks are
	///         empty, returns false.
	static bool isAnyNeedleInHaystacks(const StringType& needle,
		const std::vector<StringType>& haystacks);

	/// Checks if the given needle is in the given haystack.
	///
	/// \param[in] needle The needle to search for
	/// \param[in] haystack The haystack to search.
	/// \return True if any needle is in the haystack. If the haystack is empty,
	///         returns false.
	static bool isNeedleInHaystack(const StringType& needle,
		const StringType& haystack);
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_ALGORITHMS_H_
