#pragma once
#ifndef YOU_DATASTORE_IOPERATION_H_
#define YOU_DATASTORE_IOPERATION_H_

namespace You {
namespace DataStore {

/// A pure virtual class of operations to be put into transaction stack
class IOperation {
public:
	IOperation() = default;
	~IOperation() = default;

	/// Executes the operation
	virtual void run() = 0;
};

}   // namespace DataStore
}   // namespace You

#endif  // YOU_DATASTORE_IOPERATION_H_
