//@author A0094446X
#include "stdafx.h"
#include "windows.h"
#include "task_notification_handler.h"

namespace You {
namespace GUI {
namespace Thread {

TaskNotificationHandler::TaskNotificationHandler() {
	moveToThread(this);
}

void TaskNotificationHandler::run() {
	while (1) {
		Sleep(100);
	}
}

}  // namespace Thread
}  // namespace GUI
}  // namespace You
