#pragma once
#ifndef YOU_GUI_NLP_MANAGER_H_
#define YOU_GUI_NLP_MANAGER_H_
#include <QApplication>
#include <QWidget>
#include "You-NLP/controller.h"
#include "You-NLP/result.h"
#include "You-NLP/controller_context.h"
#include "base_manager.h"

/// The component that handles all session-related data of the application. It
/// handles basics such as the window state of the application and task sets
/// previously displayed, saving the state on program exit, and restoring them
/// when the program is started again. It inherits from the BaseManager class.
class YouMainGUI::NLPManager : public YouMainGUI::BaseManager{
	Q_OBJECT
	friend class YouMainGUI;
public:
	explicit NLPManager(YouMainGUI * const parentGUI)
		: BaseManager(parentGUI) {}
	~NLPManager();

private:
	/// Generates a Task from a QTreeWidgetItem object.

	/// Generates a TaskList object from all tasks currently visible in
	/// QTreeWidget.
	///		Questions: Can I simply build a flat list, despite the tasks being
	///		a hierarchical structure? The tree structure is after all encoded
	///		within the tasks themselves.
	You::NLP::TaskList generateTaskList(QTreeWidget* widget);
	/// Generates a Context object from a TaskList object
	///		Questions: How do I build a Context object? It appears to be
	///		inaccessible to me. Should I simply pass a list of task IDs + their
	///		corresponding view order back, instead of building the TaskList?

	/// Queries the NLP engine. Converts the current view into a context,
	/// passes the Context and wstring input into the NLP engine, and gets a
	/// Result object. Called by user's confirmation to send entry in
	/// commandInputBox, via signal/slots.
	You::NLP::Result queryNLP();

	/// Interprets the Result object (typedef from Response)
	void setup();

	private slots:
	/// QT's signal/slot mechanism for input enter button.
	void commandEnterButtonClicked();
};

#endif  // YOU_GUI_NLP_MANAGER_H_
