#include "stdafx.h"
#include "youmaingui.h"
#include "TaskModel.h"
#include <QApplication>

YouMainGUI::YouMainGUI(QWidget *parent)
	: QMainWindow(parent) {
	ui.setupUi(this);
	tm = new QStandardItemModel(0);
	tm->insertColumn(0);
	tm->insertRow(0);
	ui.taskTreePanel->setModel(tm);
}

YouMainGUI::~YouMainGUI() {
}

void YouMainGUI::on_commandEnterButton_clicked() {
	ui.commandInputBox->setText("Signal Slot Test");
	int clickCount = tm->rowCount();
	hiddenRoot = new QStandardItem(clickCount);
	tm->insertRow(clickCount, hiddenRoot);
}


