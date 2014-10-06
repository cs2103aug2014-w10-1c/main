#include "stdafx.h"
#include "..\You-GUI\you_main_gui.h"
#include "..\You-QueryEngine\api.h"

using Logger = Microsoft::VisualStudio::CppUnitTestFramework::Logger;

namespace MyUnitTests {
	TEST_CLASS(MyTestClass) {
	public:
		TEST_METHOD(MyTestFunction) {
			char* argv[1];
			int c = 1;
			char** v = &argv[0];
			// This returns a Result object
			// To do
			/*
			You::NLP::Result result = You::QueryEngine::executeQuery(
				You::QueryEngine::AddTask(L"abc",
				You::QueryEngine::Task::DEFAULT_DEADLINE,
				You::QueryEngine::Task::DEFAULT_PRIORITY,
				You::QueryEngine::Task::DEFAULT_DEPENDENCIES));
			boost::get<You::NLP::TaskList>(result);
			*/
			Logger::WriteMessage("lol");
			QApplication a(c, v);
			YouMainGUI w;
		}
	};
}  // namespace MyUnitTests
