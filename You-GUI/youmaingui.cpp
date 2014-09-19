#include "stdafx.h"
#include "youmaingui.h"
#include "TaskModel.h"
#include <QApplication>

YouMainGUI::YouMainGUI(QWidget *parent)
	: QMainWindow(parent) {
	ui.setupUi(this);
}

YouMainGUI::~YouMainGUI() {
}

void YouMainGUI::on_commandEnterButton_clicked() {
	ui.commandInputBox->setText("Signal Slot Test");
}


