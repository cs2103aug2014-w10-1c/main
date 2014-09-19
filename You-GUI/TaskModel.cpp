// taskitemmodel
#include "stdafx.h"
#include "TaskModel.h"

TaskModel::TaskModel(QObject *parent)
	:QStandardItemModel(parent) {
}

int TaskModel::rowCount(const QModelIndex & /*parent*/) const {
	return 2;
}

int TaskModel::columnCount(const QModelIndex & /*parent*/) const {
	return 3;
}

QVariant TaskModel::data(const QModelIndex &index, int role) const {
	if (role == Qt::DisplayRole) {
		return QString("Row%1, Column%2")
			.arg(index.row() + 1)
			.arg(index.column() + 1);
	}
	return QVariant();
}
