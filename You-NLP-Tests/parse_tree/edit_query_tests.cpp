//@author A0097630B
#include "stdafx.h"
#include "query_parser.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace UnitTests {

using You::NLP::EDIT_QUERY;

TEST_CLASS(EditQueryTests) {
public:
	TEST_METHOD(convertsToStream) {
		{
			std::wostringstream stream;
			stream << DUMMY;
			Assert::AreEqual(
				std::wstring(L"Edit task #1 (Description => the lols)"),
				stream.str());
		}

		{  // NOLINT(whitespace/braces)
			EDIT_QUERY local = DUMMY;
			local.fieldsToChange = EDIT_QUERY::FIELDS::DESCRIPTION |
				EDIT_QUERY::FIELDS::DUE;

			std::wostringstream stream;
			stream << local;
			Assert::AreEqual(
				std::wstring(L"Edit task #1 (Description => the lols, "
					L"Due => 1970-Jan-01 00:00:00)"),
				stream.str());
		}
	}

	TEST_METHOD(convertsToString) {
		Assert::AreEqual(
			std::wstring(L"Edit task #1 (Description => the lols)"),
			boost::lexical_cast<std::wstring>(DUMMY));
	}

	TEST_METHOD(comparesEquality) {
		EDIT_QUERY local = DUMMY;

		Assert::AreEqual(DUMMY, local);

		local.due = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1),
			boost::posix_time::hours(1));
		Assert::AreEqual(DUMMY, local, L"Changing fields not specified in the "
			L"modified mask should not cause inequality.");
	}

	TEST_METHOD(comparesInequality) {
		EDIT_QUERY local {
			1,
			EDIT_QUERY::FIELDS::DESCRIPTION,
			L""
		};

		Assert::AreNotEqual(DUMMY, local);

		local.taskID++;
		local.description = DUMMY.description;
		Assert::AreNotEqual(DUMMY, local);

		local.taskID = DUMMY.taskID;
		local.fieldsToChange = local.fieldsToChange | EDIT_QUERY::FIELDS::DUE;
		EDIT_QUERY local2 = local;
		local2.due = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1),
			boost::posix_time::hours(1));
		Assert::AreNotEqual(local, local2);
	}

private:
	/// A dummy object.
	static const EDIT_QUERY DUMMY;
};

const EDIT_QUERY EditQueryTests::DUMMY {
	1,
	EDIT_QUERY::FIELDS::DESCRIPTION,
	L"the lols",
	boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1),
		boost::posix_time::hours(0))
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You
