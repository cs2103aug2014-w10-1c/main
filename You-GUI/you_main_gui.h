#ifndef YOU_MAIN_GUI_YOUGUI_H_
#define YOU_MAIN_GUI_YOUGUI_H_

#include <QtWidgets/QMainWindow>
#include "ui_yougui.h"
#include <qlist.h>
#include <QList>
class YouMainGUI : public QMainWindow {
	Q_OBJECT

public:
	explicit YouMainGUI(QWidget *parent = nullptr);
	~YouMainGUI();
	void populateTaskPanel();  // fills item model with example data

private:
	void taskPanelSetup();  // initializes the item model and associates it with tree view
	QList<QStandardItem*> buildRow(std::vector<std::string> rowStrings);
	void createTask(QStandardItem *parent, std::vector<std::string> rowStrings);
	Ui::YouMainGUIClass ui;
	QStandardItemModel *itemModel;
	QStandardItem *hiddenRoot;
	
	private slots:
	void on_commandEnterButton_clicked();
};

#endif  // YOU_MAIN_GUI_YOUGUI_H_
