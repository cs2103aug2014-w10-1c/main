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
		class Context {
		public:
			/// Task List conversion constructor.
			Context(const TaskList& taskList);

			/// Checks if the given context is the default context.
			///
			/// \return true if the current context is the default context.
			bool isDefault() const;

			/// Checks if the given context is a task list context.
			///
			/// \return true if the current context is a task list context.
			/// \ref getTaskList
			bool isTaskList() const;

			/// Gets the task list if the current context is a task list
			/// context.
			///
			/// \return The task list if this is a task list context. Behaviour
			///         is undefined otherwise.
			const TaskList& getTaskList() const;

		public:
			/// This is the global default context.
			static const Context DEFAULT;

		private:
			/// The types of contexts expressible.
			enum class Types {
				/// This is the default context.
				DEFAULT,

				/// This context refers to a task list.
				TASK_LIST
			};

		private:
			/// Hidden default constructor. Use \ref Context::DEFAULT for the
			/// default context.
			///
			/// \see Context::DEFAULT
			Context() = default;

		private:
			/// The type of the context.
			Types type;

			union {
				/// The task list referenced by this context.
				const TaskList& taskList;
			};
		};

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
