#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "constants.h"
#include "you_main_gui.h"

YouMainGUI::YouMainGUI(QWidget *parent)
	: QMainWindow(parent) {
	ui.setupUi(this);
	taskPanelSetup();
	populateTaskPanel();
}

YouMainGUI::~YouMainGUI() {
}

void YouMainGUI::on_commandEnterButton_clicked() {
	//Fire off command to NLP!
}

void YouMainGUI::taskPanelSetup() {
	ui.taskTreePanel->setColumnCount(CONSTANTS_YOUGUI::TASK_PANEL_COLUMN_COUNT);
	std::vector<std::wstring> headerStrings;
	headerStrings.push_back(CONSTANTS_YOUGUI::TASK_PANEL_COLUMN_1);
	headerStrings.push_back(CONSTANTS_YOUGUI::TASK_PANEL_COLUMN_2);
	headerStrings.push_back(CONSTANTS_YOUGUI::TASK_PANEL_COLUMN_3);
	headerStrings.push_back(CONSTANTS_YOUGUI::TASK_PANEL_COLUMN_4);
	ui.taskTreePanel->setHeaderItem(createItem(headerStrings));
	ui.taskTreePanel->setColumnHidden(3, true);
}

//A function to demonstrate how to add data
void YouMainGUI::populateTaskPanel() {
	//Create a vector of strings representing the data for each column for a single entry
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

void YouMainGUI::addSubtask(QTreeWidgetItem* parent, std::vector<std::wstring> rowStrings) {
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
