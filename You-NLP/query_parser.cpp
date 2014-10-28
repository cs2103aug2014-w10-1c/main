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
	return qi::phrase_parse(
		begin(string),
		end(string),
		QueryParser(),
		ParserSkipperType(),
		result);
}

QueryParser::QueryParser() : QueryParser::base_type(start) {
	auto space = qi::omit[+ParserCharTraits::space];

	start %= (
		(qi::lit(L'/') > explicitCommand) |
		addCommand
	) >> qi::eoi;
	start.name("start");

	explicitCommand %= (
		(qi::lit(L"add") >> space >> addCommand) |
		(qi::lit(L"show") >> space >> showCommand) |
		(qi::lit(L"edit") >> space >> editCommand) |
		(qi::lit(L"delete") >> space >> deleteCommand) |
		(qi::lit(L"undo") >> space >> undoCommand)
	);
	explicitCommand.name("explicitCommand");

	#pragma region Adding tasks
	addCommand %= (
		addCommandDescription
	);
	addCommand.name("addCommand");

	addCommandDescription = (
		ParserCharTraits::char_ >> addCommandDescriptionTail
	)[qi::_val = phoenix::bind(&constructAddQuery, qi::_1, qi::_2)];
	addCommandDescription.name("addCommandDescription");

	addCommandDescriptionTail %= (
		addCommandPriority |
		addCommandDescription
	);
	addCommandDescriptionTail.name("addCommandDescriptionTail");

	addCommandPriority %= (
		(-space >> qi::lit('!') >> addCommandDeadlineOptional)
			[qi::_val = phoenix::bind(&constructAddQueryWithPriority, qi::_1)] |
		addCommandDeadlineOptional
	);
	addCommandPriority.name("addCommandPriority");

	addCommandDeadline = (
		space >> (qi::lit("by") | qi::lit("before")) >>
		utilityTime
	)[qi::_val = phoenix::bind(&constructAddQueryWithDeadline, qi::_1)];
	addCommandDeadline.name("addCommandDeadline");

	addCommandDeadlineOptional = (
		addCommandDeadline || qi::eoi
	)[qi::_val = phoenix::bind(&constructAddQueryWithOptionalDeadline, qi::_1)];

	#pragma endregion

	#pragma region Showing tasks
	showCommand = (
		-showCommandFiltering >>
		-((qi::lit(L"sorted by") | qi::lit(L"order by") | qi::lit(L"sort")) >>
		showCommandSorting)
	)[qi::_val = phoenix::bind(&constructShowQuery, qi::_1, qi::_2)];

	showCommandFiltering %= (
		showCommandFilteringColumn % (qi::lit(L",") | qi::lit(L"and"))
	);

	showCommandFilteringColumn = (
		showCommandFields >>
		showCommandFilteringPredicate >>
		utilityValue
	)[qi::_val = phoenix::bind(&constructShowQueryFilteringColumn,
		qi::_1,
		qi::_2,
		qi::_3)];

	showCommandFilteringPredicate.add
		(L"=", SHOW_QUERY::Predicate::EQ)
		(L"!=", SHOW_QUERY::Predicate::NOT_EQ)
		(L"<", SHOW_QUERY::Predicate::LESS_THAN)
		(L"<=", SHOW_QUERY::Predicate::LESS_THAN_EQ)
		(L">", SHOW_QUERY::Predicate::GREATER_THAN)
		(L">=", SHOW_QUERY::Predicate::GREATER_THAN_EQ);

	showCommandSorting %= (
		showCommandSortingColumn % (qi::lit(L",") | qi::lit(L"then"))
	);

	showCommandSortingColumn = (
		showCommandFields >> -showCommandSortingOrders
	)[qi::_val = phoenix::bind(&constructShowQuerySortColumn, qi::_1, qi::_2)];

	showCommandSortingOrders.add
		(L"asc", SHOW_QUERY::Order::ASCENDING)
		(L"ascending", SHOW_QUERY::Order::ASCENDING)
		(L"desc", SHOW_QUERY::Order::DESCENDING)
		(L"descending", SHOW_QUERY::Order::DESCENDING);
	showCommandSortingOrders.name("showCommandSortingOrders");

	showCommandFields.add
		(L"description", TaskField::DESCRIPTION)
		(L"deadline", TaskField::DEADLINE)
		(L"priority", TaskField::PRIORITY)
		(L"done", TaskField::COMPLETE)
		(L"complete", TaskField::COMPLETE);
	showCommandFields.name("showCommandFields");
	#pragma endregion

	#pragma region Editing tasks
	editCommand = (
		qi::uint_ >> qi::lit(L"set") >> editCommandRule
	)[qi::_val = phoenix::bind(&constructEditQuery,
		qi::_1, qi::_2)];
	editCommand.name("editCommand");

	editCommandRule %= (
		editCommandRuleNullary |
		editCommandRuleUnary |
		editCommandRulePriorities
	);
	editCommandRule.name("editCommandRule");

	editCommandRuleNullary = (
		editCommandFieldsNullary
	)[qi::_val = phoenix::bind(&constructEditQueryNullary, qi::_1)];
	editCommandRuleNullary.name("editCommandRuleNullary");

	editCommandRuleUnary = (
		editCommandFieldsUnary >> qi::lit('=') >> utilityValue)
	[qi::_val = phoenix::bind(&constructEditQueryUnary, qi::_1, qi::_2)];
	editCommandRuleUnary.name("editCommandRuleUnary");

	editCommandRulePriorities = (
		qi::lit(L"priority") >> qi::lit('=') >> utilityTaskPriority
	)[qi::_val = phoenix::bind(&constructEditQueryPriority, qi::_1)];
	editCommandRulePriorities.name("editCommandRulePriorities");

	editCommandFieldsUnary.add
		(L"description", TaskField::DESCRIPTION)
		(L"deadline", TaskField::DEADLINE);
	editCommandFieldsUnary.name("editCommandFieldsUnary");

	editCommandFieldsNullary.add
		(L"done", TaskField::COMPLETE)
		(L"complete", TaskField::COMPLETE);
	editCommandFieldsNullary.name("editCommandFieldsNullary");
	#pragma endregion

	#pragma region Deleting tasks
	deleteCommand = (
		qi::uint_
	)[qi::_val = phoenix::bind(&constructDeleteQuery, qi::_1)];
	deleteCommand.name("deleteCommand");
	#pragma endregion

	#pragma region Undoing tasks
	undoCommand = (
		qi::eps
	)[qi::_val = phoenix::construct<UNDO_QUERY>()];
	#pragma endregion

	utilityValue = (
		(qi::int_) |
		(qi::bool_) |
		utilityTaskPriority |
		utilityLexeme
	)[qi::_val = phoenix::bind(&constructValue, qi::_1)];

	utilityTaskPriority.add
		(L"normal", TaskPriority::NORMAL)
		(L"high", TaskPriority::HIGH);
	utilityTaskPriority.name("utilityTaskPriority");

	utilityTime = (
		+ParserCharTraits::char_
	)[qi::_val = phoenix::bind(&constructDateTime, qi::_1)];

	utilityLexeme %= (
		qi::lit('\'') > *utilityLexemeContents > qi::lit('\'')
	);

	utilityLexemeContents %= (
		qi::lit("\\'")[qi::_val = L'\''] |
		qi::lit("\\\\")[qi::_val = L'\\'] |
		(ParserCharTraits::char_ - qi::lit('\''))
	);

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
