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
	/// Checks if any needle is in the given haystack.
	///
	/// \param[in] needles The needles to search for.
	/// \param[in] haystack The haystack to search.
	/// \return True if any needle is in the haystack. If the haystack is empty,
	///         returns false.
	static bool needlesInHaystack(const std::vector<StringType>& needles,
		const StringType& haystack);

	/// Checks if the given needle is in the given haystack.
	///
	/// \param[in] needle The needle to search for
	/// \param[in] haystack The haystack to search.
	/// \return True if any needle is in the haystack. If the haystack is empty,
	///         returns false.
	static bool needleInHaystack(const StringType& needle,
		const StringType& haystack);
};

template struct similar_to<std::wstring>;
template struct similar_to<std::string>;

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_ALGORITHMS_H_
