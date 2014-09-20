#include "stdafx.h"
#include "session_manager.h"

SessionManager::SessionManager(){
}

SessionManager::~SessionManager(){
}

void SessionManager::loadModel(QIODevice *device, QStandardItemModel *itemModel) {
	QDataStream dataStream(device);
	int rowCount, columnCount;
	dataStream >> rowCount;
	dataStream >> columnCount;

	for (int row = 0; row < rowCount; row++) {
		for (int column = 0; column < columnCount; column++) {
			QString item;
			dataStream >> item;
			QStandardItem stditm(item);
			itemModel->setItem(row, column, &stditm);
		}
	}
}

void SessionManager::saveModel(QIODevice *device, QStandardItemModel *itemModel) {
	QDataStream dataStream(device);
	int rowCount = itemModel->rowCount();
	int columnCount = itemModel->columnCount();
	dataStream << rowCount;
	dataStream << columnCount;

	for (int row = 0; row < rowCount; row++)
		for (int column = 0; column < columnCount; column++) {
		std::string test;
		dataStream << itemModel->item(row, column)->text();
	}
}

void SessionManager::writeFile(std::string filename, QStandardItemModel* itemModel) {
	/*
	QFile file;
	file.setFileName(QString::fromStdString(filename));
	if (file.open(QIODevice::WriteOnly))
		saveModel(&file, itemModel);
	file.close();
	*/
	QFile file("somefile.bin");
	if (file.open(QIODevice::WriteOnly)) {
		QDataStream stream(&file);
		qint32 n(itemModel->rowCount()), m(itemModel->columnCount());
		stream << n << m;

		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; j++)
				itemModel->item(i, j)->write(stream);

		file.close();
	}
}

void SessionManager::loadFile(std::string filename, QStandardItemModel* itemModel) {
	QFile file(QString::fromStdString(filename));
	loadModel(&file, itemModel);
	file.close();
}
