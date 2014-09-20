#include "stdafx.h"
#include "youmaingui.h"
#include <QApplication>
#include <QList>

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
	itemModel = new QStandardItemModel(1, 4);
	ui.taskTreePanel->setModel(itemModel);

	//Creates a hidden root item, and set the tree view to show only its children
	hiddenRoot = new QStandardItem(0);
	itemModel->setItem(0, 0, hiddenRoot);
	ui.taskTreePanel->setRootIndex(itemModel->index(0, 0, QModelIndex()));
}

//A function to demonstrate how to add data
void YouMainGUI::populateTaskPanel() {
	//Create a vector of strings representing the data for each column for a single entry
	std::vector<std::string> rowStrings;
	rowStrings.push_back("abc");
	rowStrings.push_back("xyz");

	//Adds the task to the task panel. The first parameter indicates the parent
	//Therefore tasks created with parents other than hiddenRoot are subtasks
	createTask(hiddenRoot, rowStrings);
}

//A helper function to build a QList from a vector of strings
QList<QStandardItem*> YouMainGUI::buildRow(std::vector<std::string> rowStrings) {
	QList<QStandardItem*> rowItems;
	std::vector<std::string>::iterator it;
	for (it = rowStrings.begin(); it < rowStrings.end(); it++) {
		rowItems.append(new QStandardItem(QString::fromStdString(*it)));
	}
	return rowItems;
}

//Creates the task and appends it to indicated parent
void YouMainGUI::createTask(QStandardItem *parent, std::vector<std::string> rowStrings) {
	QList<QStandardItem*> strList = buildRow(rowStrings);
	parent->appendRow(strList);
}
