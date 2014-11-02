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
	return qi::parse(
		begin(string),
		end(string),
		QueryParser(),
		result);
}

QueryParser::QueryParser() : QueryParser::base_type(start) {
	start %= (
		(qi::lit('/') > explicitCommand) |
		addCommand
	) > qi::eoi;
	BOOST_SPIRIT_DEBUG_NODE(start);

	explicitCommand %= (
		(qi::lit("add") > space > addCommand) |
		(qi::lit("show") >> space >> showCommand) |
		(qi::lit("edit") >> space >> editCommand) |
		(qi::lit("delete") >> space >> deleteCommand) |
		(qi::lit("undo") >> undoCommand)
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

	addCommandDescription = (
		ParserCharTraits::char_ > addCommandDescriptionTail
	)[qi::_val = phoenix::bind(
		&constructAddQueryFromDescription,
		qi::_1,
		qi::_2)];
	BOOST_SPIRIT_DEBUG_NODE(addCommandDescription);

	addCommandDescriptionTail %= (
		addCommandPriority |
		addCommandDescription
	);
	BOOST_SPIRIT_DEBUG_NODE(addCommandDescriptionTail);

	addCommandPriority %= (
		(-space >> qi::lit('!') >> addCommandDeadlineOptional)
			[qi::_val = phoenix::bind(&constructAddQueryWithPriority, qi::_1)] |
		addCommandDeadlineOptional
	);
	BOOST_SPIRIT_DEBUG_NODE(addCommandPriority);

	addCommandDeadline = (
		space >> (qi::lit("by") | qi::lit("before")) >> space >>
		utilityTime >> *space
	)[qi::_val = phoenix::bind(&constructAddQueryWithDeadline, qi::_1)];
	BOOST_SPIRIT_DEBUG_NODE(addCommandDeadline);

	addCommandDeadlineOptional = (
		addCommandDeadline || (
			*space >> &(qi::char_(":;") | qi::lit("->") | qi::eoi))
	)[qi::_val = phoenix::bind(&constructAddQueryWithOptionalDeadline, qi::_1)];
	BOOST_SPIRIT_DEBUG_NODE(addCommandDeadlineOptional);

	addCommandSubtasks %= (
		addCommand % (*space >> qi::lit(';') >> *space)
	);
	BOOST_SPIRIT_DEBUG_NODE(addCommandSubtasks);
	#pragma endregion

	#pragma region Showing tasks
	showCommand = (
		-showCommandFiltering >>
		-(-space >> (qi::lit("sorted by") | qi::lit("order by")) >> space >>
		showCommandSorting)
	)[qi::_val = phoenix::bind(&constructShowQuery, qi::_1, qi::_2)];
	BOOST_SPIRIT_DEBUG_NODE(showCommand);

	showCommandFiltering %=
		showCommandFilteringColumn % (
			(-space >> qi::lit(',') >> -space) |
			(space >> qi::lit("and") >> space));
	BOOST_SPIRIT_DEBUG_NODE(showCommandFiltering);

	showCommandFilteringColumn = (
		showCommandFields >>
		-space >> showCommandFilteringPredicate >> -space >>
		utilityValue
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
		(L"deadline", TaskField::DEADLINE)
		(L"priority", TaskField::PRIORITY)
		(L"done", TaskField::COMPLETE)
		(L"complete", TaskField::COMPLETE);
	#pragma endregion

	#pragma region Editing tasks
	editCommand = (
		qi::uint_ >> space >> qi::lit("set") >> space >> editCommandRule
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
		editCommandFieldsUnary >>
		-space >> qi::lit('=') >> -space >>
		utilityValue)
	[qi::_val = phoenix::bind(&constructEditQueryUnary, qi::_1, qi::_2)];
	BOOST_SPIRIT_DEBUG_NODE(editCommandRuleUnary);

	editCommandRulePriorities = (
		qi::lit("priority") >>
		-space >> qi::lit('=') >> -space >>
		utilityTaskPriority
	)[qi::_val = phoenix::bind(&constructEditQueryPriority, qi::_1)];
	BOOST_SPIRIT_DEBUG_NODE(editCommandRulePriorities);

	editCommandFieldsUnary.add
		(L"description", TaskField::DESCRIPTION)
		(L"deadline", TaskField::DEADLINE);

	editCommandFieldsNullary.add
		(L"done", TaskField::COMPLETE)
		(L"complete", TaskField::COMPLETE);
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

	utilityValue = (
		(qi::int_) |
		(qi::bool_) |
		utilityTaskPriority |
		utilityLexeme
	)[qi::_val = phoenix::bind(&constructValue, qi::_1)];

	utilityTaskPriority.add
		(L"normal", TaskPriority::NORMAL)
		(L"high", TaskPriority::HIGH);

	utilityLexeme %= qi::as_wstring[(
		qi::lit('\'') > *utilityLexemeContents > qi::lit('\'')
	)];
	BOOST_SPIRIT_DEBUG_NODE(utilityLexeme);

	utilityLexemeContents %= (
		qi::lit("\\'")[qi::_val = '\''] |
		qi::lit("\\\\")[qi::_val = '\\'] |
		(ParserCharTraits::char_ - qi::lit('\''))
	);
	BOOST_SPIRIT_DEBUG_NODE(utilityLexemeContents);

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
