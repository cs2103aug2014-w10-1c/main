//@author A0097630B
#pragma once
#ifndef YOU_NLP_QUERY_PARSER_H_
#define YOU_NLP_QUERY_PARSER_H_

#include <boost/spirit/include/qi.hpp>

#include "parse_tree.h"
#include "parser.h"

namespace You {
namespace NLP {

/// The query parser that recognises our input syntax.
class QueryParser : public boost::spirit::qi::grammar<
	ParserIteratorType,
	QUERY(),
	ParserSkipperType> {
public:
	/// The type of the iterator used in this grammar.
	typedef ParserIteratorType IteratorType;

	/// The type of the skipper used in this grammar.
	typedef ParserSkipperType SkipperType;

	/// The type of the lexeme buffer provided in a lexer semantic action.
	typedef std::vector<ParserCharEncoding::char_type> LexemeType;

	/// The type of input strings accepted by this parser.
	typedef std::basic_string<ParserCharEncoding::char_type> StringType;

	/// The type of values on the RHS of any expression in the grammar.
	typedef boost::variant<
		int,
		bool,
		LexemeType,
		boost::posix_time::ptime> ValueType;

public:
	QueryParser();

	/// Parses the given input string into a query.
	///
	/// \param[in] string The string to parse.
	/// \return The query tree representing the given query.
	static QUERY parse(const StringType& string);

private:
	#pragma region Adding tasks
	/// Process the terminal returned from the add query character parse rule,
	/// stringing characters together to form the description.
	///
	/// \param[in] lexeme The lexeme from the parser.
	/// \param[in] query The \ref ADD_QUERY parse tree to the right of the
	///                  current character.
	/// \return The synthesised value for the \ref addCommand rule.
	static ADD_QUERY constructAddQuery(ParserCharEncoding::char_type lexeme,
		const ADD_QUERY& query);

	/// Process the terminal ! in a task description. Before the deadline, this
	/// indicates a high priority task.
	///
	/// \see addCommandPriority
	/// The production rule associated with this semantic action.
	///
	/// \param[in] query The query from the eoi terminal. This should be an
	///                  \ref ADD_QUERY parse tree, with an optional deadline.
	/// \return The synthesised value for the \ref addCommandPriority rule.
	static ADD_QUERY constructAddQueryWithPriority(const ADD_QUERY& query);

	/// Process the nonterminal indicating a deadline, converting it to an
	/// appropriate \ref ADD_QUERY type.
	///
	/// \see addQueryDeadline
	/// The production rule associated with this semantic action.
	///
	/// \param[in] deadline The deadline from the parser.
	/// \return The synthesised value for the \ref addCommandDeadline rule.
	static ADD_QUERY constructAddQueryWithDeadline(
		const boost::posix_time::ptime& deadline);

	/// Process the end-of-input nonterminal, potentially including a deadline.
	/// This captures the deadline, if one is present.
	///
	/// \see addQueryDeadline
	/// The production rule associated with this semantic action.
	///
	/// \param[in] query The nonterminal from the parser.
	/// \return The synthesised value for the \ref addCommandDeadline rule.
	static ADD_QUERY constructAddQueryWithOptionalDeadline(
		const boost::optional<ADD_QUERY>& query);
	#pragma endregion

	#pragma region Showing tasks
	/// Process the non-terminal returned from the show query parse rule,
	/// constructing the predicates and ordering.
	///
	/// \param[in] filters The filter predicates.
	/// \param[in] ordering The ordering for display.
	/// \return The synthesised value for the \ref showCommand rule.
	static SHOW_QUERY constructShowQuery(
		boost::optional<std::vector<SHOW_QUERY::FIELD_FILTER>> filters,
		boost::optional<std::vector<SHOW_QUERY::FIELD_ORDER>> ordering
	);

	/// Process the terminal returned from the show query filtering parse rule,
	/// constructing one filter.
	///
	/// \param[in] field The field to sort by.
	/// \param[in] predicate the predicate to apply.
	/// \param[in] value The value to compare against the field.
	/// \return The synthesised value for the \ref showQueryFilteringColumn
	///         rule.
	static SHOW_QUERY::FIELD_FILTER constructShowQueryFilteringColumn(
		const TaskField& field,
		const SHOW_QUERY::Predicate& predicate,
		const LexemeType& value);

	/// Process the terminal returned from the show query ordering parse rule,
	/// constructing one column's ordering.
	///
	/// \param[in] field The field to sort by.
	/// \param[in] order The order to sort the field by.
	/// \return The synthesised value for the \ref showCommandSortingColumn
	///         rule.
	static SHOW_QUERY::FIELD_ORDER constructShowQuerySortColumn(
		const TaskField& field,
		const boost::optional<SHOW_QUERY::Order> order
	);
	#pragma endregion

	#pragma region Editing tasks
	/// Sets the edit query's task ID.
	///
	/// \param[in] offset The task which the user is referencing.
	static EDIT_QUERY constructEditQuery(
		const size_t offset,
		const EDIT_QUERY& query);

	/// Constructs a edit query from the given parse tree values.
	///
	/// \param[in] field The field which should be edited.
	/// \return The synthesised value for the \ref editCommand rule.
	static EDIT_QUERY constructEditQueryNullary(TaskField field);

	/// Constructs a edit query from the given parse tree values.
	///
	/// \param[in] field The field which should be edited.
	/// \param[in] newValue The new value the user wants to change the field to.
	/// \return The synthesised value for the \ref editCommand rule.
	static EDIT_QUERY constructEditQueryUnary(
		TaskField field,
		const ValueType& newValue);

	/// Constructs a edit query from the given parse tree values.
	///
	/// \param[in] field The field which should be edited.
	/// \param[in] priority The new value the user wants to change the field to.
	/// \return The synthesised value for the \ref editCommand rule.
	static EDIT_QUERY constructEditQueryPriority(
		TaskPriority priority);

	/// Handles failures in parsing. This raises a \ref ParseErrorException.
	///
	/// \exception ParseErrorException The exception representing the parse
	///                                error.
	static void onFailure(
		ParserIteratorType begin,
		ParserIteratorType end,
		ParserIteratorType errorPos,
		const boost::spirit::info& message);
	#pragma endregion

	#pragma region Deleting tasks
	/// Constructs a delete query from the given parse tree values.
	///
	/// \param[in] offset The task for which the user is referencing
	/// \return The synthesised value for the \ref deleteCommand rule.
	static DELETE_QUERY constructDeleteQuery(const size_t offset);
	#pragma endregion

	/// Constructs a posix_time::ptime from the given string.
	///
	/// \param[in] lexeme The lexeme to construct the time from.
	/// \return The synthesised value for the \ref utilityTime rule.
	static boost::posix_time::ptime constructDateTime(const LexemeType& lexeme);

private:
	/// The start rule.
	start_type start;

	/// Explicit command rule.
	boost::spirit::qi::rule<IteratorType, QUERY(), SkipperType> explicitCommand;

	#pragma region Adding tasks
	/// Add command rule.
	boost::spirit::qi::rule<IteratorType, ADD_QUERY(), SkipperType> addCommand;

	/// Add command description rule.
	boost::spirit::qi::rule<IteratorType, ADD_QUERY()> addCommandDescription;

	/// Add command description tail rule.
	boost::spirit::qi::rule<IteratorType, ADD_QUERY()>
		addCommandDescriptionTail;

	/// Add command priority rule.
	boost::spirit::qi::rule<IteratorType, ADD_QUERY()> addCommandPriority;

	/// Add command's deadline rule.
	boost::spirit::qi::rule<IteratorType, ADD_QUERY()> addCommandDeadline;

	/// Add command's optional deadline rule. This acts as the terminal for the
	/// add query parser.
	boost::spirit::qi::rule<IteratorType, ADD_QUERY()>
		addCommandDeadlineOptional;
	#pragma endregion

	#pragma region Showing tasks
	/// Show command rule.
	boost::spirit::qi::rule<IteratorType, SHOW_QUERY(), SkipperType>
		showCommand;

	/// Show command filtering rule.
	boost::spirit::qi::rule<IteratorType,
		std::vector<SHOW_QUERY::FIELD_FILTER>(), SkipperType>
		showCommandFiltering;

	/// Show command filtering rule for one filter
	boost::spirit::qi::rule<IteratorType,
		SHOW_QUERY::FIELD_FILTER(),
		SkipperType> showCommandFilteringColumn;

	/// The symbol mapping from task filter predicates to an actual predicate.
	boost::spirit::qi::symbols<
		ParserCharEncoding::char_type,
		SHOW_QUERY::Predicate> showCommandFilteringPredicate;

	/// Show command sorting rule for multiple columns.
	boost::spirit::qi::rule<IteratorType,
		std::vector<SHOW_QUERY::FIELD_ORDER>(), SkipperType>
		showCommandSorting;

	/// Show command sorting rule for one column.
	boost::spirit::qi::rule<IteratorType,
		SHOW_QUERY::FIELD_ORDER(),
		SkipperType> showCommandSortingColumn;

	/// The symbol mapping from task ordering to ascending/descending.
	boost::spirit::qi::symbols<
		ParserCharEncoding::char_type,
		SHOW_QUERY::Order> showCommandSortingOrders;

	/// The symbol mapping from task properties to the actual field.
	boost::spirit::qi::symbols<
		ParserCharEncoding::char_type,
		TaskField> showCommandFields;
	#pragma endregion

	#pragma region Editing tasks
	/// Edit command rule.
	boost::spirit::qi::rule<IteratorType, EDIT_QUERY(), SkipperType>
		editCommand;

	/// Edit command nonterminal rule for the fields to set.
	boost::spirit::qi::rule<IteratorType, EDIT_QUERY(), SkipperType>
		editCommandRule;

	/// Edit command nonterminal rule for unary fields to set.
	boost::spirit::qi::rule<IteratorType, EDIT_QUERY(), SkipperType>
		editCommandRuleUnary;

	/// Edit command nonterminal rule for nullary fields to set.
	boost::spirit::qi::rule<IteratorType, EDIT_QUERY(), SkipperType>
		editCommandRuleNullary;

	/// Edit command nonterminal rule for the priority to set.
	boost::spirit::qi::rule<IteratorType, EDIT_QUERY(), SkipperType>
		editCommandRulePriorities;

	/// The symbol mapping from task properties to the actual field taking an
	/// argument (hence unary).
	boost::spirit::qi::symbols<
		ParserCharEncoding::char_type,
		TaskField> editCommandFieldsUnary;

	/// The symbol mapping from task properties to the actual field taking no
	/// arguments (hence nullary).
	boost::spirit::qi::symbols<
		ParserCharEncoding::char_type,
		TaskField> editCommandFieldsNullary;

	/// The symbol mapping from priority strings to task priorities.
	boost::spirit::qi::symbols<
		ParserCharEncoding::char_type,
		TaskPriority> editCommandFieldPriorities;
	#pragma endregion

	#pragma region Deleting tasks
	boost::spirit::qi::rule<IteratorType, DELETE_QUERY(), SkipperType>
		deleteCommand;
	#pragma endregion

	#pragma region Undoing tasks
	/// Undo command rule.
	boost::spirit::qi::rule<IteratorType, UNDO_QUERY(), SkipperType>
		undoCommand;
	#pragma endregion

	#pragma region Utility rules
	/// A utility rule which converts numbers to ints, booleans to bools,
	/// dates to posix_time::ptime, and strings verbatim.
	boost::spirit::qi::rule<IteratorType, ValueType()> utilityValue;

	/// A utility rule which converts raw strings (unquoted) into a
	/// posix_time::ptime.
	boost::spirit::qi::rule<IteratorType, boost::posix_time::ptime()>
		utilityTime;

	/// A utility rule which will process all characters verbatim. This is how
	/// the user specifies that he does not want the parser to perform syntax
	/// analysis for this part of the input.
	boost::spirit::qi::rule<IteratorType, LexemeType()> utilityLexeme;

	/// A utility rule which processes all characters within the quotes of
	/// the verbatim string.
	boost::spirit::qi::rule<IteratorType,
		ParserCharEncoding::char_type()> utilityLexemeContents;
	#pragma endregion
};

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_QUERY_PARSER_H_
