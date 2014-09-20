#ifndef SESSION_MANAGER_H_
#define SESSION_MANAGER_H_

#include <QtWidgets/QMainWindow>
class SessionManager {
public:
	SessionManager();
	~SessionManager();
	void loadModel(QIODevice *device, QStandardItemModel *model);
	void saveModel(QIODevice *device, QStandardItemModel *model);
	void writeFile(std::string filename, QStandardItemModel* itemModel);
	void loadFile(std::string filename, QStandardItemModel* itemModel);
private:
};
#endif  // SESSION_MANAGER_H_
