//@author A0097630B
#include "stdafx.h"
#include "tests_stdafx.h"
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
		const std::wstring& expectedCategory = std::wstring(),
		const std::wstring& expectedMessage = std::wstring())
	: callCount(0),
	expectedSeverity(expectedSeverity),
	expectedCategory(expectedCategory),
	expectedMessage(expectedMessage) {
	}

	void onLog(LogSeverity severity,
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
	TEST_METHOD_INITIALIZE(setLogSeverity) {
		Log::setLogLevel(LogSeverity::DEBUG);
		Log::restoreSink();
	}

	TEST_METHOD(defaultLogSink) {
		Log::debug[L"category"] << L"test";
	}

	TEST_METHOD(basicLogging) {
		std::shared_ptr<LogTesterSink> tester(
			std::make_shared<LogTesterSink>(LogSeverity::DEBUG, L"", L"lol2"));
		Log::setSink(tester);
		Log::debug << std::wstring(L"lol2");
		Assert::AreEqual(1U, tester->getCallCount());

		Log::debug << L"lo" << L"l" << 2;
		Assert::AreEqual(2U, tester->getCallCount());
	}

	TEST_METHOD(lazyLogging) {
		Log::setLogLevel(LogSeverity::CRITICAL);
		std::shared_ptr<LogTesterSink> tester(
			std::make_shared<LogTesterSink>(LogSeverity::DEBUG));
		Log::setSink(tester);

		bool called = false;
		auto callerCheck = [&called]() {
			called = true; return L"";
		};
		Log::debug << callerCheck;
		Assert::IsFalse(called);
		Assert::AreEqual(0U, tester->getCallCount());

		Log::setLogLevel(LogSeverity::DEBUG);
		Log::debug << callerCheck;
		Assert::IsTrue(called);
		Assert::AreEqual(1U, tester->getCallCount());
	}

	TEST_METHOD(asciiLogging) {
		std::shared_ptr<LogTesterSink> tester(
			std::make_shared<LogTesterSink>(LogSeverity::DEBUG, L"", L"lol2"));
		Log::setSink(tester);
		Log::debug << "lol2";
		Assert::AreEqual(1U, tester->getCallCount());

		Log::debug << std::string("lol2");
		Assert::AreEqual(2U, tester->getCallCount());
	}

	TEST_METHOD(correctLogLevels) {
		Log::setSink(std::make_shared<LogTesterSink>(LogSeverity::DEBUG));
		Log::debug << L"";

		Log::setSink(std::make_shared<LogTesterSink>(LogSeverity::INFO));
		Log::info << L"";

		Log::setSink(std::make_shared<LogTesterSink>(LogSeverity::WARNING));
		Log::warning << L"";

		Log::setSink(std::make_shared<LogTesterSink>(LogSeverity::ERROR));
		Log::error << L"";

		Log::setSink(std::make_shared<LogTesterSink>(LogSeverity::CRITICAL));
		Log::critical << L"";
	}
};

}  // namespace UnitTests
}  // namespace Utils
}  // namespace You
