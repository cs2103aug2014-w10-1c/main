#pragma once
#ifndef YOU_NLP_INTERNAL_QUERY_PARSER_H_
#define YOU_NLP_INTERNAL_QUERY_PARSER_H_

#include <boost/spirit/include/qi.hpp>

#include "parse_tree.h"
#include "parser.h"

namespace You {
namespace NLP {
namespace Internal {

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
	/// Process the nonterminal returned from the add query rule, converting it
	/// to an appropriate \ref ADD_QUERY type.
	///
	/// \param[in] lexeme The lexeme from the parser.
	/// \return The synthesised value for the \ref addCommand rule.
	static ADD_QUERY constructAddQuery(const LexemeType& lexeme);

	/// Process the nonterminal returned from the add query rule, when a
	/// deadline is specified, converting it to an appropriate \ref ADD_QUERY
	/// type.
	///
	/// \see constructAddQueryWithDeadlineTail
	/// This rule is two-part, because we need exhaustive backtracking.
	///
	/// \param[in] lexeme The lexeme from the parser. This is either the tail,
	///                   which contains the deadline and the rest of the
	///                   description; or the tail and one character of the
	///                   description.
	/// \return The synthesised value for the \ref addCommandWithDeadline rule.
	static ADD_QUERY constructAddQueryWithDeadline(const boost::variant<
		ADD_QUERY,
		boost::fusion::vector<ParserCharEncoding::char_type, ADD_QUERY>
	>& lexeme);

	/// Process the nonterminal returned from the add query rule, when a
	/// deadline is specified, converting it to an appropriate \ref ADD_QUERY
	/// type.
	///
	/// \see constructAddQueryWithDeadline
	/// This rule is two-part, because we need exhaustive backtracking.
	///
	/// \param[in] c The last character of the description.
	/// \param[in] deadline The deadline from the parser.
	/// \return The synthesised value for the \ref addCommandWithDeadline rule.
	static ADD_QUERY constructAddQueryWithDeadlineTail(
		const ParserCharEncoding::char_type c, const LexemeType& deadline);
	#pragma endregion

	#pragma region Editing tasks
	/// Constructs a edit query from the given parse tree values.
	///
	///	\param[in] offset The task which the user is referencing
	/// \param[in] field The field which should be edited.
	/// \param[in] newValue The new value the user wants to change the field to.
	/// \return The synthesised value for the \ref editCommand rule.
	static EDIT_QUERY constructEditQuery(
		const size_t offset,
		EDIT_QUERY::FIELDS field,
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

	/// Add command rule.
	boost::spirit::qi::rule<IteratorType, ADD_QUERY(), SkipperType>
		addCommandWithDescription;

	/// Add command's deadline rule.
	/// {
	boost::spirit::qi::rule<IteratorType, ADD_QUERY(), SkipperType>
		addCommandWithDeadline;
	boost::spirit::qi::rule<IteratorType, ADD_QUERY(), SkipperType>
		addCommandWithDeadlineTail;
	/// }
	#pragma endregion

	#pragma region Editing tasks
	/// Edit command rule.
	boost::spirit::qi::rule<IteratorType, EDIT_QUERY(), SkipperType>
		editCommand;

	/// The symbol mapping from task properties to the actual field.
	boost::spirit::qi::symbols<
		ParserCharEncoding::char_type,
		EDIT_QUERY::FIELDS> editCommandFields;
	#pragma endregion

	#pragma region Deleting tasks
	boost::spirit::qi::rule<IteratorType, DELETE_QUERY(), SkipperType>
		deleteCommand;
	#pragma endregion
};

}  // namespace Internal
}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_INTERNAL_QUERY_PARSER_H_
