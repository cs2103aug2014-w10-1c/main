//@author A0097630B
#include "stdafx.h"
#include "exceptions/parse_error_exception.h"
#include "query_parser.h"

namespace You {
namespace NLP {

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace phoenix = boost::phoenix;

QUERY QueryParser::parse(const StringType& string) {
	QUERY result;

	if (parse(string, result)) {
		return result;
	} else {
		throw ParserException();
	}
}

bool QueryParser::parse(const StringType& string, QUERY& result) {
	try {
		return qi::parse(
			begin(string),
			end(string),
			QueryParser() > qi::omit[*ParserCharTraits::blank] > qi::eoi,
			result);
	} catch (ParserExpectationFailure&) {
		return false;
	}
}

QueryParser::QueryParser() : QueryParser::base_type(start) {
	start %= (
		(qi::lit('/') > explicitCommand) |
		addCommand
	);
	BOOST_SPIRIT_DEBUG_NODE(start);

	explicitCommand %= (
		(qi::lit("add") > space > addCommand) |
		(qi::lit("show") > *space > showCommand) |
		(qi::lit("edit") > space > editCommand) |
		(qi::lit("delete") > space > deleteCommand) |
		(qi::lit("undo") > *space > undoCommand)
	);
	BOOST_SPIRIT_DEBUG_NODE(explicitCommand);

	#pragma region Adding tasks
	addCommand = (
		addCommandDescription >
		-(*space >> qi::lit("->") > *space > addCommand) >
		-(*space >> qi::lit(':') > *space > addCommandSubtasks)
	)[qi::_val = phoenix::bind(
		&constructAddQuery,
		qi::_1,
		qi::_2,
		qi::_3)];
	BOOST_SPIRIT_DEBUG_NODE(addCommand);

	addCommandDescription %= (
		addCommandDescriptionQuoted |
		addCommandDescriptionUnquoted
	);
	BOOST_SPIRIT_DEBUG_NODE(addCommandDescription);

	addCommandDescriptionQuoted = (
		string > addCommandPriority
	)[qi::_val = phoenix::bind(
		static_cast<ADD_QUERY (*)(StringType, ADD_QUERY)>(
			&constructAddQueryFromDescription),
		qi::_1,
		qi::_2)];
	BOOST_SPIRIT_DEBUG_NODE(addCommandDescriptionQuoted);

	addCommandDescriptionUnquoted = (
		ParserCharTraits::char_ > addCommandDescriptionUnquotedTail
	)[qi::_val = phoenix::bind(
		static_cast<ADD_QUERY(*)(ParserCharEncoding::char_type, ADD_QUERY)>(
			&constructAddQueryFromDescription),
		qi::_1,
		qi::_2)];
	BOOST_SPIRIT_DEBUG_NODE(addCommandDescriptionUnquoted);

	addCommandDescriptionUnquotedTail %= (
		addCommandPriority |
		addCommandDescriptionUnquoted
	);
	BOOST_SPIRIT_DEBUG_NODE(addCommandDescriptionUnquotedTail);

	addCommandPriority %= (
		(*space >> qi::lit('!') >> addCommandTimeOptional)
			[qi::_val = phoenix::bind(&constructAddQueryWithPriority, qi::_1)] |
			addCommandTimeOptional
	);
	BOOST_SPIRIT_DEBUG_NODE(addCommandPriority);

	addCommandTime = (
		(space >> (qi::lit("from")) >> space >> time) ||
		(space >> (qi::lit("by") | qi::lit("before") | qi::lit("to")) >>
			space >> time) >>
		*space
	)[qi::_val = phoenix::bind(&constructAddQueryWithTime, qi::_1, qi::_2)];
	BOOST_SPIRIT_DEBUG_NODE(addCommandTime);

	addCommandTimeOptional = (
		addCommandTime || (
			*space >> &(qi::char_(":;") | qi::lit("->") | qi::eoi))
	)[qi::_val = phoenix::bind(&constructAddQueryWithOptionalTime, qi::_1)];
	BOOST_SPIRIT_DEBUG_NODE(addCommandTimeOptional);

	addCommandSubtasks %= (
		addCommand % (*space >> qi::lit(';') >> *space)
	);
	BOOST_SPIRIT_DEBUG_NODE(addCommandSubtasks);
	#pragma endregion

	#pragma region Showing tasks
	showCommand = (
		-showCommandFiltering >
		-(-space >> (qi::lit("sorted by") | qi::lit("order by")) > space >
		showCommandSorting)
	)[qi::_val = phoenix::bind(&constructShowQuery, qi::_1, qi::_2)];
	BOOST_SPIRIT_DEBUG_NODE(showCommand);

	showCommandFiltering %=
		showCommandFilteringColumn % (
			(-space >> qi::lit(',') >> -space) |
			(space >> qi::lit("and") >> space));
	BOOST_SPIRIT_DEBUG_NODE(showCommandFiltering);

	showCommandFilteringColumn = (
		showCommandFields >
		-space > showCommandFilteringPredicate > -space >
		value
	)[qi::_val = phoenix::bind(&constructShowQueryFilteringColumn,
		qi::_1,
		qi::_2,
		qi::_3)];
	BOOST_SPIRIT_DEBUG_NODE(showCommandFiltering);

	showCommandFilteringPredicate.add
		(L"=", SHOW_QUERY::Predicate::EQ)
		(L"!=", SHOW_QUERY::Predicate::NOT_EQ)
		(L"<", SHOW_QUERY::Predicate::LESS_THAN)
		(L"<=", SHOW_QUERY::Predicate::LESS_THAN_EQ)
		(L">", SHOW_QUERY::Predicate::GREATER_THAN)
		(L">=", SHOW_QUERY::Predicate::GREATER_THAN_EQ);

	showCommandSorting %= (
		showCommandSortingColumn % (
			(-space >> qi::lit(",") >> -space) |
			(space >> qi::lit("then") >> space)));
	BOOST_SPIRIT_DEBUG_NODE(showCommandSorting);

	showCommandSortingColumn = (
		showCommandFields >> -(space >> showCommandSortingOrders)
	)[qi::_val = phoenix::bind(&constructShowQuerySortColumn, qi::_1, qi::_2)];
	BOOST_SPIRIT_DEBUG_NODE(showCommandSortingColumn);

	showCommandSortingOrders.add
		(L"asc", SHOW_QUERY::Order::ASCENDING)
		(L"ascending", SHOW_QUERY::Order::ASCENDING)
		(L"desc", SHOW_QUERY::Order::DESCENDING)
		(L"descending", SHOW_QUERY::Order::DESCENDING);

	showCommandFields.add
		(L"description", TaskField::DESCRIPTION)
		(L"start", TaskField::START)
		(L"deadline", TaskField::DEADLINE)
		(L"priority", TaskField::PRIORITY)
		(L"done", TaskField::COMPLETE)
		(L"complete", TaskField::COMPLETE)
		(L"completed", TaskField::COMPLETE);
	#pragma endregion

	#pragma region Editing tasks
	editCommand = (
		qi::uint_ > (
			(space >> qi::lit("set") > space > editCommandRule) |
			(*space >> qi::lit("!") > editSetHighPriority) |
			(space >> qi::lit("by") > space > editSetDeadline) |
			(*space >> qi::lit(":") > *space > editSetSubtask) |
			(*space >> qi::lit("->") > *space > editSetDependent) |
			(space >> qi::lit("attach") > space >
				editAttachmentCommandRule(true)) |
			(space >> qi::lit("detach") > space >
				editAttachmentCommandRule(false)))
	)[qi::_val = phoenix::bind(&constructEditQuery,
		qi::_1, qi::_2)];
	BOOST_SPIRIT_DEBUG_NODE(editCommand);

	editCommandRule %= (
		editCommandRuleNullary |
		editCommandRuleUnary |
		editCommandRulePriorities
	);
	BOOST_SPIRIT_DEBUG_NODE(editCommandRule);

	editCommandRuleNullary = (
		editCommandFieldsNullary
	)[qi::_val = phoenix::bind(&constructEditQueryNullary, qi::_1)];
	BOOST_SPIRIT_DEBUG_NODE(editCommandRuleNullary);

	editCommandRuleUnary = (
		editCommandFieldsUnary >
		*space > qi::lit('=') > *space >
		value)
	[qi::_val = phoenix::bind(&constructEditQueryUnary, qi::_1, qi::_2)];
	BOOST_SPIRIT_DEBUG_NODE(editCommandRuleUnary);

	editCommandRulePriorities = (
		qi::lit("priority") >
		*space > qi::lit('=') > *space >
		taskPriority
	)[qi::_val = phoenix::bind(&constructEditQueryPriority, qi::_1)];
	BOOST_SPIRIT_DEBUG_NODE(editCommandRulePriorities);

	editCommandFieldsUnary.add
		(L"description", TaskField::DESCRIPTION)
		(L"start", TaskField::START)
		(L"deadline", TaskField::DEADLINE);

	editCommandFieldsNullary.add
		(L"done", TaskField::COMPLETE)
		(L"complete", TaskField::COMPLETE)
		(L"completed", TaskField::COMPLETE);

	editSetHighPriority = (
		qi::eps
	)[qi::_val = phoenix::bind(&constructEditQueryPriority,
		TaskPriority::HIGH)];
	BOOST_SPIRIT_DEBUG_NODE(editSetHighPriority);

	editSetDeadline = (
		time
	)[qi::_val = phoenix::bind(&constructEditQueryDeadline, qi::_1)];
	BOOST_SPIRIT_DEBUG_NODE(editSetDeadline);

	editSetSubtask = (
		qi::int_
	)[qi::_val = phoenix::bind(&constructEditQuerySubtask, qi::_1)];
	BOOST_SPIRIT_DEBUG_NODE(editSetSubtask);

	editSetDependent = (
		qi::int_
	)[qi::_val = phoenix::bind(&constructEditQueryDependent, qi::_1)];
	BOOST_SPIRIT_DEBUG_NODE(editSetDependent);

	editAttachmentCommandRule = (
		string
	)[qi::_val = phoenix::bind(&constructEditQueryAttachment, qi::_r1, qi::_1)];
	BOOST_SPIRIT_DEBUG_NODE(editAttachmentCommandRule);
	#pragma endregion

	#pragma region Deleting tasks
	deleteCommand = (
		qi::uint_
	)[qi::_val = phoenix::bind(&constructDeleteQuery, qi::_1)];
	BOOST_SPIRIT_DEBUG_NODE(deleteCommand);
	#pragma endregion

	#pragma region Undoing tasks
	undoCommand = (
		qi::eps
	)[qi::_val = phoenix::construct<UNDO_QUERY>()];
	BOOST_SPIRIT_DEBUG_NODE(undoCommand);
	#pragma endregion

	space = qi::omit[+ParserCharTraits::blank];

	value = (
		(qi::int_) |
		(qi::bool_) |
		taskPriority |
		string
	)[qi::_val = phoenix::bind(&constructValue, qi::_1)];

	taskPriority.add
		(L"normal", TaskPriority::NORMAL)
		(L"high", TaskPriority::HIGH);

	string %= qi::as_wstring[(
		qi::lit('\'') > *stringContents > qi::lit('\'')
	)];
	BOOST_SPIRIT_DEBUG_NODE(string);

	stringContents %= (
		qi::lit("\\'")[qi::_val = '\''] |
		qi::lit("\\\\")[qi::_val = '\\'] |
		(ParserCharTraits::char_ - qi::lit('\''))
	);
	BOOST_SPIRIT_DEBUG_NODE(stringContents);

	qi::on_error<qi::fail>(start,
		phoenix::bind(&onFailure, qi::_1, qi::_2, qi::_3, qi::_4));
}

void QueryParser::onFailure(
	ParserIteratorType /*begin*/,
	ParserIteratorType end,
	ParserIteratorType errorPos,
	const spirit::info& message) {
	StringType lexeme(errorPos, end);
	throw ParseErrorException(message, lexeme);
}

}  // namespace NLP
}  // namespace You
