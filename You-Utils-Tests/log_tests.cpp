//@author A0097630B
#include "stdafx.h"
#include <boost/tuple/tuple.hpp>
#include "log.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace Microsoft {
namespace VisualStudio {
namespace CppUnitTestFramework {

std::wstring ToString(You::Utils::LogSeverity severity) {
	return ToString(static_cast<int>(severity));
}

}  // namespace CppUnitTestFramework
}  // namespace VisualStudio
}  // namespace Microsoft

namespace You {
namespace Utils {
namespace UnitTests {

class LogTesterSink : public LogSink {
public:
	LogTesterSink(LogSeverity expectedSeverity,
		const std::wstring& expectedCategory,
		const std::wstring& expectedMessage)
	: callCount(0),
	expectedSeverity(expectedSeverity),
	expectedCategory(expectedCategory),
	expectedMessage(expectedMessage) {
	}

	virtual void onLog(LogSeverity severity,
		const std::wstring& category,
		const std::wstring& message) override {
		Assert::AreEqual(expectedSeverity, severity);
		Assert::AreEqual(expectedCategory, category);
		Assert::AreEqual(expectedMessage, message);
		++callCount;
	}

	inline size_t getCallCount() const {
		return callCount;
	}

private:
	size_t callCount;
	LogSeverity expectedSeverity;
	std::wstring expectedCategory;
	std::wstring expectedMessage;
};

TEST_CLASS(LogTests) {
	TEST_METHOD(debugLogMethod) {
		std::shared_ptr<LogTesterSink> tester(
			std::make_shared<LogTesterSink>(LogSeverity::DEBUG, L"", L"lol"));
		Log::setSink(tester);
		Log::debug << L"lol";
		Assert::AreEqual(1U, tester->getCallCount());

		Log::debug << L"lo" << L"l";
		Assert::AreEqual(2U, tester->getCallCount());
	}
};

}  // namespace UnitTests
}  // namespace Utils
}  // namespace You
