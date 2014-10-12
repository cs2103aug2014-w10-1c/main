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
			local.deadline = boost::posix_time::ptime(
				boost::gregorian::date(1970, boost::gregorian::Jan, 1),
				boost::posix_time::hours(0));

			std::wostringstream stream;
			stream << local;
			Assert::AreEqual(
				std::wstring(L"Edit task #1 (Description => the lols, "
					L"Deadline => 1970-Jan-01 00:00:00)"),
				stream.str());
		}

		{  // NOLINT(whitespace/braces)
			EDIT_QUERY local = DUMMY;
			local.priority = TaskPriority::HIGH;

			std::wostringstream stream;
			stream << local;
			Assert::AreEqual(
				std::wstring(L"Edit task #1 (Description => the lols, "
				L"Priority => 1)"),
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
	}

	TEST_METHOD(comparesInequality) {
		EDIT_QUERY local {
			DUMMY.taskID,
			DUMMY.description
		};

		Assert::AreEqual(DUMMY, local);

		EDIT_QUERY local2 = local;
		local2.taskID++;
		Assert::AreNotEqual(DUMMY, local2);

		local2 = local;
		local2.description = std::wstring(L"");
		Assert::AreNotEqual(DUMMY, local2);

		local2 = local;
		local2.deadline = boost::posix_time::ptime(
			boost::gregorian::date(1970, 1, 1),
			boost::posix_time::hours(1));
		Assert::AreNotEqual(local, local2);

		local2 = local;
		local2.priority = TaskPriority::HIGH;
		Assert::AreNotEqual(local, local2);

		local2 = local;
		local2.complete = true;
		Assert::AreNotEqual(local, local2);
	}

private:
	/// A dummy object.
	static const EDIT_QUERY DUMMY;
};

const EDIT_QUERY EditQueryTests::DUMMY {
	1,
	L"the lols"
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You
