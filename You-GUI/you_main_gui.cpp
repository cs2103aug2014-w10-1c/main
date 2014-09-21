#include "stdafx.h"
#include <QApplication>
#include <QList>
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
	itemModel.setRowCount(4);
	itemModel.setColumnCount(4);
	ui.taskTreePanel->setModel(&itemModel);

	//Creates a hidden root item, and set the tree view to show only its children
	itemModel.setItem(0, 0, &hiddenRoot);
	ui.taskTreePanel->setRootIndex(itemModel.index(0, 0, QModelIndex()));
}

//A function to demonstrate how to add data
void YouMainGUI::populateTaskPanel() {
	//Create a vector of strings representing the data for each column for a single entry
	std::vector<std::wstring> rowStrings;
	rowStrings.push_back(L"abc");
	rowStrings.push_back(L"xyz");
	//Adds the task to the task panel. The first parameter indicates the parent
	//Therefore tasks created with parents other than hiddenRoot are subtasks
	createTask(&hiddenRoot, rowStrings);
}

//A helper function to build a QList from a vector of strings
QList<QStandardItem*> YouMainGUI::buildRow(std::vector<std::wstring> rowStrings) {
	QList<QStandardItem*> rowItems;
	for (auto it = rowStrings.begin(); it < rowStrings.end(); it++) {
		rowItems.append(new QStandardItem(QString::fromStdWString(*it)));
	}
	return rowItems;
}

//Creates the task and appends it to indicated parent
void YouMainGUI::createTask(QStandardItem *parent, std::vector<std::wstring> rowStrings) {
	QList<QStandardItem*> strList = buildRow(rowStrings);
	parent->appendRow(strList);
}
