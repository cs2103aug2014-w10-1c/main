#include <string>
#include "CppUnitTest.h"
#include <QtWidgets>
#include "..\You-GUI\you_main_gui.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MyUnitTests {
	TEST_CLASS(MyTestClass) {
	public:
		TEST_METHOD(MyTestFunction) {
			char* argv[1];
			int c = 1;
			char** v = &argv[0];
			QApplication a(c, v);
			YouMainGUI w;
		}
	};
} // namespace MyUnitTests
