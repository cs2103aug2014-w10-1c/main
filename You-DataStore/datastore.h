#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include "src\pugixml.hpp"

namespace You {
namespace DataStore {

class DataStore {
public:
	DataStore() = default;
	~DataStore() = default;

private:
	static const std::wstring FILE_PATH;
	pugi::xml_document document;
	/// Saves the xml object to a file
	/// Returns true if operation successful and false otherwise
	bool saveData();
	/// Loads the xml file into the xml object
	void loadData();
};

}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_DATASTORE_H_
