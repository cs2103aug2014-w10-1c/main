#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "you_main_gui.h"

YouMainGUI::YouMainGUI(QWidget *parent)
	: QMainWindow(parent) {
	ui.setupUi(this);
	loadSession();
	taskPanelSetup();
	populateTaskPanel();
}

YouMainGUI::~YouMainGUI() {
}

void YouMainGUI::closeEvent(QCloseEvent *event) {
	saveSession();
}

void YouMainGUI::on_commandEnterButton_clicked() {
	YouMainGUI::queryNLP();
}

void YouMainGUI::taskPanelSetup() {
	ui.taskTreePanel->setColumnCount(YouMainGUI::TASK_COLUMN_COUNT);
	const std::vector<std::wstring> headerStrings = {
		YouMainGUI::TASK_COLUMN_1, YouMainGUI::TASK_COLUMN_2,
		YouMainGUI::TASK_COLUMN_3, YouMainGUI::TASK_COLUMN_4,
		YouMainGUI::TASK_COLUMN_5 };
	ui.taskTreePanel->setHeaderItem(createItem(headerStrings));
	ui.taskTreePanel->setColumnHidden(0, true);
}

void YouMainGUI::populateTaskPanel() {
	// Create a vector of strings representing the data for each column for a
	// single entry
	for (int i = 0; i < 10; i++) {
		std::vector<std::wstring> rowStrings;
		rowStrings.push_back(L"abc");
		rowStrings.push_back(L"xyz");
		rowStrings.push_back(L"xyzz");
		addTask(rowStrings);
	}
}

void YouMainGUI::addTask(std::vector<std::wstring> rowStrings) {
	QTreeWidgetItem* item = createItem(rowStrings);
	ui.taskTreePanel->addTopLevelItem(item);
}

void YouMainGUI::addSubtask(QTreeWidgetItem* parent,
							std::vector<std::wstring> rowStrings) {
	QTreeWidgetItem* item = createItem(rowStrings);
	parent->addChild(item);
}

QTreeWidgetItem* YouMainGUI::createItem(std::vector<std::wstring> rowStrings) {
	QStringList tempList;
	std::vector<std::wstring>::iterator it;
	for (auto it = rowStrings.begin(); it != rowStrings.end(); it++) {
		tempList << QString::fromStdWString(*it);
	}
	QTreeWidgetItem *item = new QTreeWidgetItem(tempList);
	return item;
}

void YouMainGUI::deleteTask(QTreeWidgetItem* task) {
	delete task;
}

void YouMainGUI::loadSession() {
	QSettings settings("You", "You");
	settings.beginGroup("MainWindow");
	resize(settings.value("size", QSize(400, 400)).toSize());
	move(settings.value("pos", QPoint(200, 200)).toPoint());
	settings.endGroup();
}

void YouMainGUI::saveSession() {
	QSettings settings("You", "You");
	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.endGroup();
}

void YouMainGUI::queryNLP() {
	/// Init a task list
	/// Convert GUI state into a context
	/// Feed query and context into NLP engine
	You::NLP::TaskList tl;
	QString inputString = ui.commandInputBox->text();
	/// Get Result from controller.query()
}
