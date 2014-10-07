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
	/// \param[in] lexeme The deadline from the parser.
	/// \return The synthesised value for the \ref addCommandDeadline rule.
	static ADD_QUERY constructAddQueryWithDeadline(const LexemeType& lexeme);

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

	#pragma region Editing tasks
	/// Constructs a edit query from the given parse tree values.
	///
	/// \param[in] offset The task which the user is referencing
	/// \param[in] field The field which should be edited.
	/// \param[in] newValue The new value the user wants to change the field to.
	/// \return The synthesised value for the \ref editCommand rule.
	static EDIT_QUERY constructEditQuery(
		const size_t offset,
		EDIT_QUERY::Fields field,
		const LexemeType& newValue);

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

	#pragma region Editing tasks
	/// Edit command rule.
	boost::spirit::qi::rule<IteratorType, EDIT_QUERY(), SkipperType>
		editCommand;

	/// The symbol mapping from task properties to the actual field.
	boost::spirit::qi::symbols<
		ParserCharEncoding::char_type,
		EDIT_QUERY::Fields> editCommandFields;
	#pragma endregion

	#pragma region Deleting tasks
	boost::spirit::qi::rule<IteratorType, DELETE_QUERY(), SkipperType>
		deleteCommand;
	#pragma endregion
};

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_QUERY_PARSER_H_
