#ifndef YOU_MAIN_GUI_YOUGUI_H_
#define YOU_MAIN_GUI_YOUGUI_H_

#include <QtWidgets/QMainWindow>
#include <QList>
#include "ui_yougui.h"
#include "session_manager.h"

class YouMainGUI : public QMainWindow {
	Q_OBJECT

public:
	explicit YouMainGUI(QWidget *parent = nullptr);
	~YouMainGUI();
	void populateTaskPanel();  // fills item model with example data

	QStandardItemModel itemModel;
private:
	void taskPanelSetup();  // initializes the item model and associates it with tree view
	QList<QStandardItem*> buildRow(std::vector<std::wstring> rowStrings);
	void createTask(QStandardItem *parent, std::vector<std::wstring> rowStrings);
	Ui::YouMainGUIClass ui;
	//QStandardItemModel itemModel(1, 4);
	QStandardItem hiddenRoot;
	SessionManager sessionManager;

	private slots:
	void on_commandEnterButton_clicked();
};

#endif  // YOU_MAIN_GUI_YOUGUI_H_
