#include "stdafx.h"
#include "You-Controller/controller.h"
#include "You-Controller/result.h"
#include "You-Controller/controller_context.h"
#include "You-QueryEngine/internal/task_builder.h"
#include "You-GUI/you_main_gui.h"
using Logger = Microsoft::VisualStudio::CppUnitTestFramework::Logger;
using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;
typedef You::Controller::Task Task;
typedef You::Controller::TaskList TaskList;

typedef You::Controller::Result Result;

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
			TaskList tl;
			Task newTask =
				You::QueryEngine::Internal::TaskBuilder::get().description(L"LOL");
			tl.push_back(newTask);
			w.addTasks(tl);
			Assert::AreEqual("abc", "abc", L"abc");
		}
	};
}  // namespace MyUnitTests
