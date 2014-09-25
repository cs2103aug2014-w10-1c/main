#include "stdafx.h"
#include "internal/query_parser.h"
#include "exception.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

#pragma region boost::variant visitor helpers

namespace {

struct ToStringVisitor : public boost::static_visitor<std::wstring> {
	template<typename T>
	std::wstring operator()(const T& value) {
		return ToString(value);
	}
};

}

namespace boost {

template<typename T1>
std::wstring ToString(const variant<T1>& t) {
	ToStringVisitor visitor;
	return apply_visitor(visitor, t);
}

}

#pragma endregion

namespace You {
namespace NLP {
namespace Internal {
	template <typename Q> static std::wstring ToString(const Q& q) {
		return q.toString();
	}
}

namespace UnitTests {

using You::NLP::ParserException;

using You::NLP::Internal::QueryParser;
using You::NLP::Internal::ADD_QUERY;
using You::NLP::Internal::QUERY;

TEST_CLASS(ParserTests) {
public:
	TEST_METHOD(throwsExceptionWhenParseFails) {
		Assert::ExpectException<ParserException>([]() {
				// "throw" is currently not defined, so this should work.
				QueryParser::parse(L"/throw");
			}, L"Throws exception on syntax error");
	}

	TEST_METHOD(parsesStringAsTask) {
		QUERY q = QueryParser::parse(L"win");

		Assert::AreEqual(QUERY(ADD_QUERY{
			L"win",
			std::wstring()
		}), q);
	}
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You
