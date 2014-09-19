#ifndef YOUGUI_H
#define YOUGUI_H

#include <QtWidgets/QMainWindow>
#include "ui_yougui.h"

class YouGUI : public QMainWindow
{
	Q_OBJECT

public:
	YouGUI(QWidget *parent = 0);
	~YouGUI();

private:
	Ui::YouGUIClass ui;
};

#endif // YOUGUI_H
