#ifndef TASK_MODEL_ITEM_H_
#define TASK_MODEL_ITEM_H_

#include <QtWidgets/QMainWindow>
#include <QStandardItem>

class TaskModelItem : public QStandardItem {
public:
	TaskModelItem();
	TaskModelItem(const QString & text);
	~TaskModelItem();
};


#endif  // TASK_MODEL_ITEM_H_
