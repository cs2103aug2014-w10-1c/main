#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATION_H_

#include "../task_typedefs.h"
#include "pugixml.h"

namespace You {
namespace DataStore {
namespace Internal {

/// A pure virtual class of operations to be put into transaction stack
class IOperation {
public:
	/// Executes the operation
	///
	/// \param[in] document The document to modify.
	/// \return True if the operation succeeded.
	virtual bool run(pugi::xml_document& document) = 0;

	virtual ~IOperation() = default;

protected:
	/// The Task ID to modify
	TaskId taskId;
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_OPERATION_H_
