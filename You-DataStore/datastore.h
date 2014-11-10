//@author A0114171W
#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include <vector>
#include "task_typedefs.h"
#include "transaction.h"

namespace You {
namespace DataStore {

/// The public methods of the data store.
///
/// This is actually a facade for the \ref Internal::DataStore class.
class DataStore {
public:
	/// Get the instance of DataStore
	static DataStore& get();

	/// Start a new transaction. \see Transaction
	///
	/// \return A reference to a new \ref Transaction
	Transaction begin();

	/// \name Methods modifying tasks
	/// @{

	/// Push a post operation to operation queue
	///
	/// \param[in] taskId The task's ID
	/// \param[in] task The \ref KeyValuePairs of task fields
	void post(TaskId taskId, const KeyValuePairs& task);

	/// Push a put operation to operation queue
	///
	/// \param[in] taskId The task's ID
	/// \param[in] task The \ref KeyValuePairs of task fields
	void put(TaskId taskId, const KeyValuePairs& task);

	/// Push an erase operation to operation queue
	///
	/// \param[in] taskId The task's ID
	void erase(TaskId taskId);

	/// @}

	/// \name Methods modifying application resources
	/// @{

	/// Push a post operation to operation queue
	///
	/// \param[in] resourceId The resource's ID
	/// \param[in] kvp The \ref KeyValuePairs of resource
	void post(std::wstring resourceId, const KeyValuePairs& kvp);

	/// Push a put operation to operation queue
	///
	/// \param[in] resourceId The resource's ID
	/// \param[in] kvp The \ref KeyValuePairs of resource
	void put(std::wstring resourceId, const KeyValuePairs& kvp);

	/// Push an erase operation to operation queue
	///
	/// \param[in] resourceId The resource's ID
	void erase(std::wstring resourceId);

	/// @}

	/// Get all tasks
	/// \return a vector of KeyValuePairs
	std::vector<KeyValuePairs> getAllTasks();

	/// Get all resources
	/// \return a vector of KeyValuePairs
	std::vector<KeyValuePairs> getAllResources();

	/// Wipe the datastore
	void wipeData();

private:
	/// Default constructor. Use \ref get to get the singleton instance.
	DataStore() = default;

private:
	/// Tag name of the tasks branch in the XML
	static const std::wstring TASKS_NODE;
	/// Tag name of the resources branch in the XML
	static const std::wstring RESOURCES_NODE;
};

}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_DATASTORE_H_
