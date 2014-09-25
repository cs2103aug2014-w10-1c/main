#include "stdafx.h"
#include <boost/format.hpp>

#include "internal/query_parser.h"
#include "exception.h"
#include "../../internal/test_helpers.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace Internal {
namespace UnitTests {

using You::NLP::Internal::ADD_QUERY;

TEST_CLASS(AddQueryTests) {
public:
	TEST_METHOD(convertsToStream) {
		std::wostringstream stream;
		stream << DUMMY;
		Assert::AreEqual(
			(boost::wformat(L"%1% (due %2%)") % DESCRIPTION % DUE).str(),
			stream.str());
	}

	TEST_METHOD(comparesEquality) {
		ADD_QUERY local {
			DESCRIPTION,
			DUE
		};

		Assert::AreEqual(DUMMY, local);
	}

	TEST_METHOD(comparesInequality) {
		ADD_QUERY local {
			DUE,
			DESCRIPTION
		};

		Assert::AreNotEqual(DUMMY, local);
	}

private:
	/// The dummy description.
	static const std::wstring DESCRIPTION;

	/// The dummy deadline.
	static const std::wstring DUE;

	/// A dummy object.
	static const ADD_QUERY DUMMY;
};

const std::wstring AddQueryTests::DESCRIPTION(L"Hello world");
const std::wstring AddQueryTests::DUE(L"23 May");
const ADD_QUERY AddQueryTests::DUMMY { DESCRIPTION, DUE };

}  // namespace UnitTests
}  // namespace Internal
}  // namespace NLP
}  // namespace You
