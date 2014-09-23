#include "stdafx.h"
#include "DataStore.h"

namespace You {
namespace DataStore {

const std::wstring DataStore::FILE_PATH =std::wstring(L"data.xml");

// Saves the xml object to a file
// Returns true if operation successful and false otherwise
bool You::DataStore::DataStore::saveData() {
	bool status = document.save_file(FILE_PATH.c_str());
	return status;
}

// Loads the xml file into the xml object
void You::DataStore::DataStore::loadData() {
	pugi::xml_parse_result status = document.load_file(FILE_PATH.c_str());

	// Not sure if the if block below is necessary
	if (status == pugi::xml_parse_status::status_file_not_found) {
		document.reset();
	}
}

}  // namespace DataStore
}  // namespace You
