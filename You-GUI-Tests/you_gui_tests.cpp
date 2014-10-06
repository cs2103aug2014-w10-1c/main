#include "stdafx.h"
#include "..\You-GUI\you_main_gui.h"
using Logger = Microsoft::VisualStudio::CppUnitTestFramework::Logger;
using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace MyUnitTests {
	TEST_CLASS(MyTestClass) {
	public:
		TEST_METHOD(MyTestFunction) {
			char* argv[1];
			int c = 1;
			char** v = &argv[0];
			// This returns a Result object
			// To do
			QApplication a(c, v);
			YouMainGUI w;
			Assert::AreEqual("abc", "abc", L"abc");
		}
	};
}  // namespace MyUnitTests
