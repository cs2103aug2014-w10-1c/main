#pragma once
#ifndef YOU_NLP_TESTS_INTERNAL_TEST_HELPERS_H_
#define YOU_NLP_TESTS_INTERNAL_TEST_HELPERS_H_

#include <string>
#include <boost/variant/static_visitor.hpp>

namespace You {
namespace NLP {
namespace UnitTests {

struct ToStringVisitor : public boost::static_visitor<std::wstring> {
	template<typename T>
	std::wstring operator()(const T& value) {
		return ToString(value);
	}
};

}  // namespace UnitTests
}
}

namespace boost {

template<typename T1>
std::wstring ToString(const variant<T1>& t) {
	You::NLP::UnitTests::ToStringVisitor visitor;
	return apply_visitor(visitor, t);
}

}  // namespace boost

#endif  // YOU_NLP_TESTS_INTERNAL_TEST_HELPERS_H_
