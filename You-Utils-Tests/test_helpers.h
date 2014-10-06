//@author A0097630B
#pragma once
#ifndef YOU_UTILS_TESTS_TEST_HELPERS_H_
#define YOU_UTILS_TESTS_TEST_HELPERS_H_

#include <string>
#include <boost/variant/static_visitor.hpp>

namespace You {
namespace NLP {
namespace UnitTests {

/// Applies ToString to the element in the container.
struct ToStringVisitor : public boost::static_visitor<std::wstring> {
	/// Applies ToString to the given element in the variant.
	template<typename T>
	std::wstring operator()(const T& value) {
		return ToString(value);
	}
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You

namespace boost {

template<typename T1, typename T2, typename T3>
std::wstring ToString(const variant<T1, T2, T3>& t) {
	You::NLP::UnitTests::ToStringVisitor visitor;
	return apply_visitor(visitor, t);
}

inline std::wstring ToString(const boost::gregorian::date& date) {
	return boost::lexical_cast<std::wstring>(date);
}

inline std::wstring ToString(const boost::posix_time::ptime& time) {
	return boost::lexical_cast<std::wstring>(time);
}

}  // namespace boost

namespace Microsoft {
namespace VisualStudio {
namespace CppUnitTestFramework {

/// Missing overloads.
/// @{
// NOLINTNEXTLINE(runtime/int)
template<> static std::wstring ToString<long long>(const long long& t) {
	RETURN_WIDE_STRING(t);
}
// NOLINTNEXTLINE(runtime/int)
template<> static std::wstring ToString<long long>(const long long* t) {
	RETURN_WIDE_STRING(t);
}
// NOLINTNEXTLINE(runtime/int)
template<> static std::wstring ToString<long long>(long long* t) {
	RETURN_WIDE_STRING(t);
}
/// }

}  // namespace CppUnitTestFramework
}  // namespace VisualStudio
}  // namespace Microsoft

#endif  // YOU_UTILS_TESTS_TEST_HELPERS_H_
