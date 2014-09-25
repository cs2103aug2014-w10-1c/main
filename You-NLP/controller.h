#pragma once
#ifndef YOU_NLP_CONTROLLER_H_
#define YOU_NLP_CONTROLLER_H_

#include <string>
#include "result.h"

namespace You {
namespace NLP {
namespace Internal { class QueryBuilderVisitor; }

/// The entry-point for all communications with the rest of the You
/// framework.
///
/// This is a singleton class.
class Controller {
	friend class You::NLP::Internal::QueryBuilderVisitor;
	class Context;

public:
	/// Gets the singleton instance for the controller.
	static Controller& get();

	/// Executes the given natural language query.
	///
	/// \param[in] query The natural language query to execute.
	/// \param[in] context The task list context in which the query is run.
	/// \return The result of the query.
	/// \exception ContextRequiredException This is raised when a context is
	///                                     needed for the query to run.
	/// \see Result The result is polymorphic.
	Result query(const std::wstring& query, const Context& context) const;

private:
	Controller(const Controller&) = delete;
	Controller& operator=(const Controller&) = delete;
	Controller() = default;
};

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_CONTROLLER_H_
