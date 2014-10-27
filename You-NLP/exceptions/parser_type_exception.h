//@author A0097630B
#pragma once
#ifndef YOU_NLP_EXCEPTIONS_PARSER_TYPE_EXCEPTION_H_
#define YOU_NLP_EXCEPTIONS_PARSER_TYPE_EXCEPTION_H_

#include "parser_exception.h"

namespace You {
namespace NLP {

/// Represents a parser type mismatch in the NLP engine.
class ParserTypeException : public ParserException {
};

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_EXCEPTIONS_PARSER_TYPE_EXCEPTION_H_
