#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "you_main_gui.h"

YouMainGUI::YouMainGUI(QWidget *parent)
	: QMainWindow(parent), sm(new YouMainGUI::SessionManager(this)),
		stm(new YouMainGUI::SystemTrayManager(this)),
		tpm(new YouMainGUI::TaskPanelManager(this)) {
	columnHeaders = { TASK_COLUMN_1,
		TASK_COLUMN_2, TASK_COLUMN_3, TASK_COLUMN_4, TASK_COLUMN_5 };
	ui.setupUi(this);
	createActions();
	stm->setIcon();
	sm->loadSession();
	tpm->taskPanelSetup();
	populateTaskPanel();
}

YouMainGUI::~YouMainGUI() {
}

void YouMainGUI::closeEvent(QCloseEvent *event) {
	if (stm->trayIcon.isVisible()) {
		QMessageBox::information(this, tr("Systray"),
			tr("The program will keep running in the "
			"system tray. To terminate the program, "
			"choose <b>Quit</b> in the context menu "
			"of the system tray entry."));
		hide();
		event->ignore();
	}
}

void YouMainGUI::on_commandEnterButton_clicked() {
	YouMainGUI::queryNLP();
}

void YouMainGUI::populateTaskPanel() {
	// Create a vector of strings representing the data for each column for a
	// single entry
	for (int i = 0; i < 10; i++) {
		std::vector<std::wstring> rowStrings;
		rowStrings.push_back(L"abc");
		rowStrings.push_back(L"xyz");
		rowStrings.push_back(L"xyzz");
		tpm->addTask(rowStrings);
	}
}

You::NLP::Result YouMainGUI::queryNLP() {
	/// Init a task list
	/// Convert GUI state into a context
	/// Feed query and context into NLP engine
	You::NLP::TaskList tl;
	std::wstring inputString = ui.commandInputBox->text().toStdWString();
	/// Get Result from controller.query()
	You::NLP::Result result;
	return result;
}

YouMainGUI::BaseManager::BaseManager(YouMainGUI* parentGUI)
	: parentGUI(parentGUI) {}

void YouMainGUI::createActions()
{
	this->stm->minimizeAction = new QAction(tr("Mi&nimize"), this);
	connect(this->stm->minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

	this->stm->maximizeAction = new QAction(tr("Ma&ximize"), this);
	connect(this->stm->maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

	this->stm->restoreAction = new QAction(tr("&Restore"), this);
	connect(this->stm->restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	this->stm->quitAction = new QAction(tr("&Quit"), this);
	connect(this->stm->quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void YouMainGUI::setVisible(bool visible){
	this->stm->minimizeAction->setEnabled(visible);
	this->stm->maximizeAction->setEnabled(!isMaximized());
	this->stm->restoreAction->setEnabled(isMaximized() || !visible);
	QWidget::setVisible(visible);
}
