//@author A0097630B
#include "stdafx.h"
#include "You-Utils/fatal.h"

#include "exception.h"
#include "date_time_parser.h"
#include "query_parser.h"

namespace You {
namespace NLP {

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace phoenix = boost::phoenix;

ADD_QUERY QueryParser::constructAddQuery(
	ADD_QUERY query,
	boost::optional<ADD_QUERY> dependent,
	boost::optional<std::vector<ADD_QUERY>> subtasks) {
	if (static_cast<bool>(subtasks)) {
		query.subtasks = std::move(subtasks.get());
	}
	if (static_cast<bool>(dependent)) {
		query.dependent = std::make_shared<ADD_QUERY>(
			std::move(dependent.get()));
	}

	return query;
}

ADD_QUERY QueryParser::constructAddQueryFromDescription(
	ParserCharEncoding::char_type lexeme,
	ADD_QUERY query) {
	query.description.insert(query.description.begin(), lexeme);
	return query;
}

ADD_QUERY QueryParser::constructAddQueryFromDescription(
	StringType description,
	ADD_QUERY query) {
	query.description = std::move(description);
	return query;
}

ADD_QUERY QueryParser::constructAddQueryWithPriority(ADD_QUERY query) {
	query.priority = TaskPriority::HIGH;
	return query;
}

ADD_QUERY QueryParser::constructAddQueryWithTime(
	boost::optional<boost::posix_time::ptime> start,
	boost::optional<boost::posix_time::ptime> deadline) {
	return ADD_QUERY {
		std::wstring(),
		TaskPriority::NORMAL,
		std::move(start),
		std::move(deadline)
	};
}

ADD_QUERY QueryParser::constructAddQueryWithOptionalTime(
	boost::optional<ADD_QUERY> query) {
	if (query) {
		return query.get();
	} else {
		return ADD_QUERY {
		};
	}
}

SHOW_QUERY QueryParser::constructShowQuery(
	boost::optional<std::vector<SHOW_QUERY::FIELD_FILTER>> filters,
	boost::optional<std::vector<SHOW_QUERY::FIELD_ORDER>> ordering) {
	SHOW_QUERY result;
	if (static_cast<bool>(filters)) {
		result.predicates = std::move(filters.get());
	}
	if (static_cast<bool>(ordering)) {
		result.order = std::move(ordering.get());
	}

	return result;
}

SHOW_QUERY::FIELD_FILTER QueryParser::constructShowQueryFilteringColumn(
	const TaskField& field,
	const SHOW_QUERY::Predicate& predicate,
	const ValueType& value) {
	SHOW_QUERY::FIELD_FILTER result {
		field,
		predicate
	};

	try {
		switch (field) {
		case TaskField::DESCRIPTION:
			result.value = boost::get<StringType>(value);
			break;
		case TaskField::START:
		case TaskField::DEADLINE:
			result.value = boost::get<boost::posix_time::ptime>(value);
			break;
		case TaskField::PRIORITY:
			result.value = boost::get<TaskPriority>(value);
			break;
		case TaskField::COMPLETE:
			result.value = boost::get<bool>(value);
			break;
		default:
			fatal();
		}
	} catch (boost::bad_get&) {
		throw ParserTypeException();
	}

	return result;
}

SHOW_QUERY::FIELD_ORDER QueryParser::constructShowQuerySortColumn(
	const TaskField& field,
	const boost::optional<SHOW_QUERY::Order> order) {
	return SHOW_QUERY::FIELD_ORDER {
		field,
		static_cast<bool>(order) ? order.get() : SHOW_QUERY::Order::ASCENDING
	};
}

EDIT_QUERY QueryParser::constructEditQuery(
	size_t offset,
	EDIT_QUERY query) {
	query.taskID = offset;

	return query;
}

EDIT_QUERY QueryParser::constructEditQueryNullary(TaskField field) {
	EDIT_QUERY result;

	switch (field) {
	case TaskField::COMPLETE:
		result.complete = true;
		break;
	default:
		fatal();
	}

	return result;
}

EDIT_QUERY QueryParser::constructEditQueryUnary(
	TaskField field,
	ValueType newValue) {
	EDIT_QUERY result;

	try {
		switch (field) {
		case TaskField::DESCRIPTION:
			result.description = std::move(boost::get<StringType>(newValue));
			break;
		case TaskField::START:
			result.start = std::move(
				boost::get<boost::posix_time::ptime>(newValue));
			break;
		case TaskField::DEADLINE:
			result.deadline = std::move(
				boost::get<boost::posix_time::ptime>(newValue));
			break;
		default:
			fatal();
		}

		return result;
	} catch (boost::bad_get&) {
		throw ParserTypeException();
	}
}

EDIT_QUERY QueryParser::constructEditQueryPriority(TaskPriority priority) {
	EDIT_QUERY result;
	result.priority = priority;

	return result;
}

EDIT_QUERY QueryParser::constructEditQueryDeadline(
	boost::posix_time::ptime deadline) {
	EDIT_QUERY result;
	result.deadline = std::move(deadline);

	return result;
}

EDIT_QUERY QueryParser::constructEditQuerySubtask(int subtask) {
	EDIT_QUERY result;
	result.childTask = subtask;

	return result;
}

EDIT_QUERY QueryParser::constructEditQueryDependent(int dependent) {
	EDIT_QUERY result;
	result.dependingTask = dependent;

	return result;
}

EDIT_QUERY QueryParser::constructEditQueryAttachment(
	bool attach,
	StringType file) {
	EDIT_QUERY result;
	result.attachments.push_back({ attach, file });

	return result;
}

DELETE_QUERY QueryParser::constructDeleteQuery(const size_t offset) {
	return DELETE_QUERY {
		offset
	};
}

QueryParser::ValueType QueryParser::constructValue(ValueType value) {
	// Only process if it is a string. It can be a date, in that case.
	if (const StringType* lexeme = boost::get<StringType>(&value)) {
		boost::posix_time::ptime date;
		if (DateTimeParser::parse(
			std::wstring(begin(*lexeme), end(*lexeme)), date)) {
			return date;
		}
	}

	return value;
}

}  // namespace NLP
}  // namespace You
