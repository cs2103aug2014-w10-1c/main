#ifndef YOU_GUI_YOUGUI_H_
#define YOU_GUI_YOUGUI_H_

#include <QtWidgets/QMainWindow>
#include "ui_yougui.h"

class YouGUI : public QMainWindow{
	Q_OBJECT

public:
	explicit YouGUI(QWidget *parent = nullptr);
	~YouGUI();

private:
	Ui::YouGUIClass ui;
};

#endif  // YOU_GUI_YOUGUI_H_
