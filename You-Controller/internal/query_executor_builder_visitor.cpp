//@author A0097630B
#include "stdafx.h"
#include "query_executor.h"
#include "../exceptions/context_index_out_of_range_exception.h"
#include "../result.h"
#include "query_executor_builder_visitor.h"

namespace You {
namespace Controller {
namespace Internal {

using You::NLP::TaskField;
using You::NLP::TaskPriority;
using You::NLP::QUERY;
using You::NLP::ADD_QUERY;
using You::NLP::SHOW_QUERY;
using You::NLP::EDIT_QUERY;
using You::NLP::DELETE_QUERY;
using You::NLP::UNDO_QUERY;

QueryExecutorBuilderVisitor::QueryExecutorBuilderVisitor(
	const Controller::Context& context)
	: context(context) {
}

std::unique_ptr<QueryExecutor>
QueryExecutorBuilderVisitor::build(const ADD_QUERY& query) {
	class AddTaskQueryExecutor : public QueryExecutor {
	public:
		explicit AddTaskQueryExecutor(
			std::unique_ptr<QueryEngine::Query>&& query)
			: QueryExecutor(std::move(query)) {
		}

		virtual ~AddTaskQueryExecutor() = default;

	protected:
		Result processResponse(
			const You::QueryEngine::Response& response) override {
			return ADD_RESULT {
				boost::get<Task>(response)
			};
		}
	};

	return std::unique_ptr<QueryExecutor>(
		new AddTaskQueryExecutor(buildAddQuery(query)));
}

std::unique_ptr<QueryEngine::Query>
QueryExecutorBuilderVisitor::buildAddQuery(const ADD_QUERY& query) {
	std::vector<std::unique_ptr<QueryEngine::Query>> subtaskQueries;
	std::vector<std::unique_ptr<QueryEngine::Query>> dependencyQueries;
	// subtaskQueries.reserve(query.subtasks.size());

	std::transform(begin(query.subtasks), end(query.subtasks),
		std::back_inserter(subtaskQueries), [](const ADD_QUERY& q) {
		return QueryExecutorBuilderVisitor::buildAddQuery(q);
	});

	std::shared_ptr<ADD_QUERY> dependentQuery = query.dependent;
	while (dependentQuery) {
		ADD_QUERY withoutDependencies(*dependentQuery);
		withoutDependencies.dependent.reset();
		dependencyQueries.push_back(buildAddQuery(withoutDependencies));
		dependentQuery = dependentQuery->dependent;
	}

	return QueryEngine::AddTask(
		query.description,
		query.deadline ? query.deadline.get() : Task::DEFAULT_DEADLINE,
		query.priority == TaskPriority::HIGH ?
		Task::Priority::HIGH : Task::Priority::NORMAL,
		std::move(dependencyQueries),
		std::move(subtaskQueries)
	);
}

std::unique_ptr<QueryExecutor>
QueryExecutorBuilderVisitor::build(const SHOW_QUERY& query) {
	class ShowTaskQueryExecutor : public QueryExecutor {
	public:
		explicit ShowTaskQueryExecutor(
			std::unique_ptr<QueryEngine::Query>&& query)
			: QueryExecutor(std::move(query)) {
		}

		virtual ~ShowTaskQueryExecutor() = default;

	protected:
		Result processResponse(
			const You::QueryEngine::Response& response) override {
			return SHOW_RESULT {
				boost::get<TaskList>(response)
			};
		}
	};

	using You::QueryEngine::Filter;
	using You::QueryEngine::Comparator;
	Filter filter = Filter::anyTask();
	Comparator comparator(Comparator::notSorted());

	std::for_each(begin(query.predicates), end(query.predicates),
		[&filter](const SHOW_QUERY::FIELD_FILTER& field) {
			std::function<bool(const Task&)> currentFilter;
			switch (field.field) {
			case TaskField::DESCRIPTION:
				assert(boost::get<std::wstring>(&field.value));
				currentFilter = buildComparator(&Task::getDescription,
					field.predicate,
					boost::get<std::wstring>(field.value));
				break;
			case TaskField::DEADLINE:
				assert(boost::get<boost::posix_time::ptime>(&field.value));
				currentFilter = buildComparator(&Task::getDeadline,
					field.predicate,
					boost::get<boost::posix_time::ptime>(field.value));
				break;
			case TaskField::COMPLETE:
				assert(boost::get<bool>(&field.value));
				currentFilter = buildComparator(&Task::isCompleted,
					field.predicate,
					boost::get<bool>(field.value));
				break;
			case TaskField::PRIORITY:
				assert(boost::get<TaskPriority>(&field.value));
				currentFilter = buildComparator(&Task::getPriority,
					field.predicate,
					Controller::nlpToQueryEnginePriority(
						boost::get<TaskPriority>(field.value)));
				break;
			default:
				assert(false); abort();
			}

			filter = filter && Filter(currentFilter);
		});

	std::for_each(begin(query.order), end(query.order),
		[&comparator](const SHOW_QUERY::FIELD_ORDER& field) {
			Comparator comp(Comparator::notSorted());
			switch (field.field) {
			case TaskField::DESCRIPTION:
				comp = Comparator::byDescription();
				break;
			case TaskField::DEADLINE:
				comp = Comparator::byDeadline();
				break;
			case TaskField::PRIORITY:
				comp = Comparator::byPriority();
				break;
			case TaskField::COMPLETE:
			default:
				assert(false); abort();
			}

			if (field.order == SHOW_QUERY::Order::ASCENDING) {
				comp = comp.ascending();
			} else {
				comp = comp.descending();
			}

			comparator = comparator && comp;
		});

	return std::unique_ptr<QueryExecutor>(
		new ShowTaskQueryExecutor(
			QueryEngine::GetTask(filter, comparator)
		)
	);
}

template<typename TValue>
std::function<bool(const Task&)>
QueryExecutorBuilderVisitor::buildComparator(
	TValue(QueryEngine::Task::*selector)() const,
	SHOW_QUERY::Predicate predicate,
	const TValue& value) {
	switch (predicate) {
	case SHOW_QUERY::Predicate::EQ:
		return std::bind(std::equal_to<TValue>(),
			std::bind(selector, std::placeholders::_1),
			value);
	case SHOW_QUERY::Predicate::NOT_EQ:
		return std::bind(std::not_equal_to<TValue>(),
			std::bind(selector, std::placeholders::_1),
			value);
	case SHOW_QUERY::Predicate::LESS_THAN:
		return std::bind(std::less<TValue>(),
			std::bind(selector, std::placeholders::_1),
			value);
	case SHOW_QUERY::Predicate::LESS_THAN_EQ:
		return std::bind(std::less_equal<TValue>(),
			std::bind(selector, std::placeholders::_1),
			value);
	case SHOW_QUERY::Predicate::GREATER_THAN:
		return std::bind(std::greater<TValue>(),
			std::bind(selector, std::placeholders::_1),
			value);
	case SHOW_QUERY::Predicate::GREATER_THAN_EQ:
		return std::bind(std::greater_equal<TValue>(),
			std::bind(selector, std::placeholders::_1),
			value);
	default:
		assert(false); abort();
	}
}

std::unique_ptr<QueryExecutor>
QueryExecutorBuilderVisitor::build(const EDIT_QUERY& query) const {
	class EditTaskQueryExecutor : public QueryExecutor {
	public:
		explicit EditTaskQueryExecutor(
			std::unique_ptr<QueryEngine::Query>&& query)
			: QueryExecutor(std::move(query)) {
		}

		virtual ~EditTaskQueryExecutor() = default;

	protected:
		Result processResponse(
			const You::QueryEngine::Response& response) override {
			return EDIT_RESULT {
				boost::get<Task>(response)
			};
		}
	};

	try {
		Task::ID task = context.at(query.taskID).getID();
		You::Utils::Option<Task::Priority> priority;
		if (query.priority) {
			priority = Controller::nlpToQueryEnginePriority(
				query.priority.get());
		}

		You::Utils::Option<Task::Subtasks> subtasks;
		if (query.childTask) {
			Task::Subtasks subtasksList;
			subtasksList.insert(context.at(query.childTask).getID());
			subtasks = subtasksList;
		}

		You::Utils::Option<Task::Dependencies> dependencies;
		if (query.dependingTask) {
			assert(!query.description &&
				!query.deadline &&
				!priority &&
				!query.complete &&
				!subtasks && "Cannot change dependencies and other properties");
			Task::Dependencies dependenciesList;
			dependenciesList.insert(task);
			dependencies = dependenciesList;
			task = context.at(query.dependingTask).getID();
		}

		return std::unique_ptr<QueryExecutor>(
			new EditTaskQueryExecutor(
				QueryEngine::UpdateTask(
					task,
					query.description,
					query.deadline,
					priority,
					dependencies,
					query.complete,
					boost::none,
					subtasks)));
	} catch (std::out_of_range& e) {
		throw ContextIndexOutOfRangeException(e);
	}
}

std::unique_ptr<QueryExecutor>
QueryExecutorBuilderVisitor::build(const DELETE_QUERY& query) const {
	class DeleteTaskQueryExecutor : public QueryExecutor {
	public:
		explicit DeleteTaskQueryExecutor(
			std::unique_ptr<QueryEngine::Query>&& query)
			: QueryExecutor(std::move(query)) {
		}

		virtual ~DeleteTaskQueryExecutor() = default;

	protected:
		Result processResponse(
			const You::QueryEngine::Response& response) override {
			return DELETE_RESULT {
				boost::get<Task::ID>(response)
			};
		}
	};

	try {
		const Task& task = context.at(query.taskID);

		return std::unique_ptr<QueryExecutor>(
			new DeleteTaskQueryExecutor(
				QueryEngine::DeleteTask(
					task.getID())));
	} catch (std::out_of_range& e) {
		throw ContextIndexOutOfRangeException(e);
	}
}

std::unique_ptr<QueryExecutor>
QueryExecutorBuilderVisitor::build(const UNDO_QUERY& query) const {
	class UndoTaskQueryExecutor : public QueryExecutor {
	public:
		explicit UndoTaskQueryExecutor(
			std::unique_ptr<QueryEngine::Query>&& query)
			: QueryExecutor(std::move(query)) {
		}

		virtual ~UndoTaskQueryExecutor() = default;

	protected:
		Result processResponse(
			const You::QueryEngine::Response& response) override {
			return UNDO_RESULT {
				boost::get<TaskList>(response)
			};
		}
	};

	return std::unique_ptr<QueryExecutor>(
		new UndoTaskQueryExecutor(
			QueryEngine::Undo()));
}

}  // namespace Internal
}  // namespace Controller
}  // namespace You
