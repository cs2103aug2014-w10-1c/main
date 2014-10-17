//@author A0094446X
#pragma once
#ifndef YOU_GUI_TASK_NOTIFICATION_HANDLER_H_
#define YOU_GUI_TASK_NOTIFICATION_HANDLER_H_
#include <QThread>
#include "You-Controller\controller.h"

namespace You {
namespace GUI {
namespace Thread {
using Task = You::Controller::Task;
/// TaskNotificationHandler inherits from QThread, and runs on a separate
/// thread as a listener to check when to notify the user of upcoming tasks.
/// It maintains a list of IDs with notifications today and the next, and
/// signals when a notification is due.
class TaskNotificationHandler : public QThread {
	Q_OBJECT
public:
	/// Constructor.
	TaskNotificationHandler();

signals:
	/// Signals to MainWindow to notify user of upcoming task
	void notify(Task::ID);

protected:
	/// Loops in its own thread to check if it is time for notification
	void run();

private:
	/// Implement a vector of key-value pairs, with key being datetime and
	/// value being Task::ID
};

}  // namespace Thread
}  // namespace GUI
}  // namespace You

#endif  // YOU_GUI_TASK_NOTIFICATION_HANDLER_H_
