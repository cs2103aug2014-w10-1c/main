#ifndef TASK_ITEM_MODEL_H_
#define TASK_ITEM_MODEL_H_

#include <QtWidgets/QMainWindow>
#include <QStandardItemModel>

class TaskModel : public QStandardItemModel {
	Q_OBJECT

public:
	TaskModel(QObject *parent = nullptr);
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};


#endif  // TASK_ITEM_MODEL_H_
