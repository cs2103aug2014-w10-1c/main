#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include "src\pugixml.hpp"

namespace You {
namespace DataStore {

class DataStore {
public:
	static const std::wstring FILE_PATH;

	DataStore();
	~DataStore();

private:
	static const std::wstring FILE_PATH;
	pugi::xml_document document;
};

}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_DATASTORE_H_
