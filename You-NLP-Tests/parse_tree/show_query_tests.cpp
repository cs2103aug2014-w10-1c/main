//@author A0097630B
#include "stdafx.h"
#include "parse_tree/show_query.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace UnitTests {

TEST_CLASS(ShowQueryTests) {
	TEST_METHOD(convertsToStream) {
		std::wostringstream stream;
		stream << DUMMY;
		Assert::AreEqual(
			std::wstring(L"Show tasks (criteria Description=, sort by "
				L"Description ascending)"),
			stream.str());
	}

	TEST_METHOD(convertsToString) {
		Assert::AreEqual(
			std::wstring(L"Show tasks (criteria Description=, sort by "
				L"Description ascending)"),
			boost::lexical_cast<std::wstring>(DUMMY));
	}

	TEST_METHOD(comparesEquality) {
		SHOW_QUERY local = DUMMY;

		Assert::AreEqual(DUMMY, local);
	}

	TEST_METHOD(comparesInequality) {
		SHOW_QUERY local {
			{ { TaskField::DESCRIPTION, SHOW_QUERY::Predicate::EQ, L"" } },
			{
				{ TaskField::DESCRIPTION, SHOW_QUERY::Order::ASCENDING }
			}
		};

		Assert::AreEqual(DUMMY, local);

		local.order[0].field = TaskField::DEADLINE;
		Assert::AreNotEqual(DUMMY, local);

		local = DUMMY;
		local.order.push_back(SHOW_QUERY::FIELD_ORDER {
			TaskField::DESCRIPTION
		});
		Assert::AreNotEqual(DUMMY, local);

		local = DUMMY;
		local.predicates.emplace_back(SHOW_QUERY::FIELD_FILTER {
			TaskField::DESCRIPTION,
			SHOW_QUERY::Predicate::EQ,
			Utils::make_option<std::wstring>(L"")
		});
		Assert::AreNotEqual(DUMMY, local);

		SHOW_QUERY local2 = local;
		local2.predicates.push_back(local2.predicates[0]);
		Assert::AreNotEqual(local, local2);

		local2 = local;
		local2.predicates[0].field = TaskField::PRIORITY;
		Assert::AreNotEqual(local, local2);

		local2 = local;
		local2.predicates[0].predicate = SHOW_QUERY::Predicate::GREATER_THAN;
		Assert::AreNotEqual(local, local2);

		local2 = local;
		local2.predicates[0].value = std::wstring(L"not empty");
		Assert::AreNotEqual(local, local2);
	}

private:
	/// A dummy object.
	static const SHOW_QUERY DUMMY;
};

const SHOW_QUERY ShowQueryTests::DUMMY {
	{ { TaskField::DESCRIPTION, SHOW_QUERY::Predicate::EQ, L"" } },
	{ { TaskField::DESCRIPTION, SHOW_QUERY::Order::ASCENDING } }
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You
