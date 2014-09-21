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
	/*
	QFile file("tt");
	if (file.open(QIODevice::WriteOnly)) {
	QDataStream stream(&file);
	qint32 n(itemModel.rowCount()), m(itemModel.columnCount());
	stream << n << m;

	for (int i = 0; i < n; ++i) {
	for (int j = 0; j < m; j++){
	QString test;
	test = itemModel.item(i, j)->text();
	stream << test;
	}
	}
	file.close();
	}*/
}

void YouMainGUI::taskPanelSetup() {
	ui.taskTreePanel->setColumnCount(2);
}

//A function to demonstrate how to add data
void YouMainGUI::populateTaskPanel() {
	//Create a vector of strings representing the data for each column for a single entry
	for (int i = 0; i < 10; i++){
		std::vector<std::wstring> rowStrings;
		rowStrings.push_back(L"abc");
		rowStrings.push_back(L"xyz");
		QTreeWidgetItem* item = createTask(rowStrings);
		ui.taskTreePanel->addTopLevelItem(item);
	}
}

QTreeWidgetItem* YouMainGUI::createTask(std::vector<std::wstring> rowStrings) {
	QStringList tempList;
	std::vector<std::wstring>::iterator it;
	for (auto it = rowStrings.begin(); it != rowStrings.end(); it++) {
		tempList << QString::fromStdWString(*it);
	}
	QTreeWidgetItem *item = new QTreeWidgetItem(tempList);
	return item;
}
