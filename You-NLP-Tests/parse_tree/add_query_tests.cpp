//@author A0097630B
#include "stdafx.h"
#include "parse_tree.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace UnitTests {

using You::NLP::ADD_QUERY;

TEST_CLASS(AddQueryTests) {
public:
	TEST_METHOD(convertsToStream) {
		std::wostringstream stream;
		stream << DUMMY;
		Assert::AreEqual(
			(boost::wformat(L"%1% (start %2%, deadline %3%, normal priority, "
				L"1 subtasks, 1 dependents)") % DESCRIPTION % START %
				DEADLINE).str(),
			stream.str());
	}

	TEST_METHOD(convertsToString) {
		Assert::AreEqual(
			(boost::wformat(L"%1% (start %2%, deadline %3%, normal priority, "
				L"1 subtasks, 1 dependents)") % DESCRIPTION % START %
				DEADLINE).str(),
			boost::lexical_cast<std::wstring>(DUMMY));
	}

	TEST_METHOD(comparesEquality) {
		ADD_QUERY local {
			DESCRIPTION,
			TaskPriority::NORMAL,
			START,
			DEADLINE,
			{ ADD_QUERY { DESCRIPTION } },
			std::shared_ptr<ADD_QUERY>(new ADD_QUERY { DESCRIPTION + L"3" })
		};

		Assert::AreEqual(DUMMY, local);
	}

	TEST_METHOD(comparesInequality) {
		ADD_QUERY local {
			L"x" + DESCRIPTION,
			TaskPriority::NORMAL,
			DEADLINE
		};

		Assert::AreNotEqual(DUMMY, local);

		local.description = DUMMY.description;
		local.priority = TaskPriority::HIGH;
		Assert::AreNotEqual(DUMMY, local);

		local.priority = DUMMY.priority;
		local.deadline = DUMMY.deadline.get() + boost::posix_time::hours(1);
		Assert::AreNotEqual(DUMMY, local);
	}

private:
	/// The dummy description.
	static const std::wstring DESCRIPTION;

	/// The dummy start time.
	static const boost::posix_time::ptime START;

	/// The dummy deadline.
	static const boost::posix_time::ptime DEADLINE;

	/// A dummy object.
	static const ADD_QUERY DUMMY;
};

const std::wstring AddQueryTests::DESCRIPTION(L"Hello world");
const boost::posix_time::ptime AddQueryTests::START(
	boost::gregorian::date(2010, boost::gregorian::Dec, 3),
	boost::posix_time::hours(0));
const boost::posix_time::ptime AddQueryTests::DEADLINE(
	boost::gregorian::date(2010, boost::gregorian::Dec, 4),
	boost::posix_time::hours(0));
const ADD_QUERY AddQueryTests::DUMMY {
	DESCRIPTION,
	TaskPriority::NORMAL,
	START,
	DEADLINE,
	{ ADD_QUERY { DESCRIPTION } },
	std::shared_ptr<ADD_QUERY>(new ADD_QUERY { DESCRIPTION + L"3" })
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You
