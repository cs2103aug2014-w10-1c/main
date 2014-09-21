#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include "internal\pugixml.hpp"

namespace You {
namespace DataStore {

class DataStore {
public:
	const static std::wstring FILE_PATH;

	DataStore();
	~DataStore();

private:
	pugi::xml_document document;

};

} // namespace DataStore
} // namespace You

#endif // YOU_DATASTORE_DATASTORE_H_
