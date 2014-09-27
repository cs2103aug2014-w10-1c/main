#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "you_main_gui.h"

YouMainGUI::YouMainGUI(QWidget *parent)
	: QMainWindow(parent), sm(new YouMainGUI::SessionManager(*this)),
		stm(new YouMainGUI::SystemTrayManager(*this)),
		tpm(new YouMainGUI::TaskPanelManager(*this)),
		columnHeaders(std::vector<std::wstring>({ TASK_COLUMN_1,
		TASK_COLUMN_2, TASK_COLUMN_3, TASK_COLUMN_4, TASK_COLUMN_5 })) {
	ui.setupUi(this);
	stm->setIcon();
	sm->loadSession();
	tpm->taskPanelSetup();
	populateTaskPanel();
}

YouMainGUI::~YouMainGUI() {
}

void YouMainGUI::closeEvent(QCloseEvent *event) {
	sm->saveSession();
}

void YouMainGUI::on_commandEnterButton_clicked() {
	YouMainGUI::queryNLP();
}

void YouMainGUI::TaskPanelManager::taskPanelSetup() {
	parentGUI.ui.taskTreePanel->setColumnCount(4);
	parentGUI.ui.taskTreePanel->setHeaderItem(createItem(parentGUI.columnHeaders));
	parentGUI.ui.taskTreePanel->setColumnHidden(0, true);
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

YouMainGUI::TaskPanelManager::~TaskPanelManager() {
}

void YouMainGUI::TaskPanelManager::addTask(
		std::vector<std::wstring> rowStrings) {
	QTreeWidgetItem* item = createItem(rowStrings);
	parentGUI.ui.taskTreePanel->addTopLevelItem(item);
}

void YouMainGUI::TaskPanelManager::addSubtask(QTreeWidgetItem* parent,
		std::vector<std::wstring> rowStrings) {
	QTreeWidgetItem* item = createItem(rowStrings);
	parent->addChild(item);
}

QTreeWidgetItem* YouMainGUI::TaskPanelManager::createItem(
		std::vector<std::wstring> rowStrings) {
	QStringList tempList;
	std::vector<std::wstring>::iterator it;
	for (auto it = rowStrings.begin(); it != rowStrings.end(); it++) {
		tempList << QString::fromStdWString(*it);
	}
	QTreeWidgetItem *item = new QTreeWidgetItem(tempList);
	return item;
}

void YouMainGUI::TaskPanelManager::deleteTask(QTreeWidgetItem* task) {
	delete task;
}


YouMainGUI::SessionManager::~SessionManager() {
}

void YouMainGUI::SessionManager::loadSession() {
	QSettings settings("You", "You");
	settings.beginGroup("MainWindow");
	parentGUI.resize(settings.value("size", QSize(400, 400)).toSize());
	parentGUI.move(settings.value("pos", QPoint(200, 200)).toPoint());
	settings.endGroup();
}

void YouMainGUI::SessionManager::saveSession() {
	QSettings settings("You", "You");
	settings.beginGroup("MainWindow");
	settings.setValue("size", parentGUI.size());
	settings.setValue("pos", parentGUI.pos());
	settings.endGroup();
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

YouMainGUI::SystemTrayManager::~SystemTrayManager() {
}

void YouMainGUI::SystemTrayManager::setIcon() {
	QIcon icon("icon.png");
	trayIcon.setIcon(icon);
	parentGUI.setWindowIcon(icon);
	trayIcon.show();
}

void YouMainGUI::SystemTrayManager::iconActivated(
		QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		if (!parentGUI.YouMainGUI::isVisible()) {
			parentGUI.YouMainGUI::setVisible(false);
		} else {
			parentGUI.YouMainGUI::setVisible(true);
		}
	case QSystemTrayIcon::DoubleClick:
		if (!parentGUI.YouMainGUI::isVisible()) {
			parentGUI.YouMainGUI::setVisible(false);
		} else {
			parentGUI.YouMainGUI::setVisible(true);
		}
		break;
	default: {}
	}
}

void YouMainGUI::SessionManager::setup() {}
void YouMainGUI::TaskPanelManager::setup() {}
void YouMainGUI::SystemTrayManager::setup() {}
YouMainGUI::BaseManager::BaseManager(YouMainGUI& parentGUI)
	: parentGUI(parentGUI) {}
