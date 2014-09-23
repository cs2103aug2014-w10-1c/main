#include "stdafx.h"
#include "DataStore.h"

namespace You {
namespace DataStore {

const std::wstring DataStore::FILE_PATH =std::wstring(L"data.xml");

	DataStore::DataStore() {
		pugi::xml_parse_result status = document.load_file(FILE_PATH.c_str());

		// Not sure if the if block below is necessary
		if (status == pugi::xml_parse_status::status_file_not_found) {
			document.reset();
		}
	}

	DataStore::~DataStore() {
		document.save_file(FILE_PATH.c_str());
	}

}  // namespace DataStore
}  // namespace You
