#pragma once
#ifndef YOU_NLP_CONTROLLER_H_
#define YOU_NLP_CONTROLLER_H_

#include <string>
#include "result.h"
#include "task_list.h"

namespace You {
namespace NLP {
	/// The entry-point for all communications with the rest of the You
	/// framework.
	///
	/// This is a singleton class.
	class Controller {
	public:
		Controller(const Controller&) = delete;
		Controller& operator=(const Controller&) = delete;

		/// Gets the singleton instance for the controller.
		static Controller& get();

		/// Executes the given natural language query.
		///
		/// \param[in] query The natural language query to execute.
		/// \param[in] context The task list context in which the query is run.
		/// \return The result of the query.
		/// \see Result The result is polymorphic.
		Result query(const std::wstring& query, const TaskList& context) const;

	private:
		Controller() = default;
	};
}
}

#endif  // YOU_NLP_CONTROLLER_H_
