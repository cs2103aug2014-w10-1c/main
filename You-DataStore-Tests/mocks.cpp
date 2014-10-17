#include "stdafx.h"
#include "mocks.h"

namespace You {
namespace DataStore {
namespace UnitTests {

const std::wstring TASK_ID = L"id";
const std::wstring DESCRIPTION = L"description";
const std::wstring DEADLINE = L"deadline";
const std::wstring PRIORITY = L"priority";
const std::wstring DEPENDENCIES = L"dependency";

SerializedTask task1() {
	SerializedTask st = {
		{ TASK_ID, L"0" },
		{ DESCRIPTION, L"bla bla" },
		{ DEADLINE, L"xxxxxx" },
		{ PRIORITY, L"urgent" },
		{ DEPENDENCIES, L"12345" }
	};
	return st;
}

SerializedTask task2() {
	SerializedTask st = {
		{ TASK_ID, L"1" },
		{ DESCRIPTION, L"yada yada" },
		{ DEADLINE, L"yyyyyy" },
		{ PRIORITY, L"normal" },
		{ DEPENDENCIES, L"4321" }
	};
	return st;
}

}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You
