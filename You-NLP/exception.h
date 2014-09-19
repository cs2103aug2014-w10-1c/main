#pragma once
#ifndef YOU_NLP_EXCEPTION_H_
#define YOU_NLP_EXCEPTION_H_

#include <stdexcept>

namespace You {
namespace NLP {

class Exception : public std::exception {
};

}  // namespace NLP
}  // namespace You

#include "exceptions/context_required_exception.h"
#include "exceptions/parser_exception.h"

#endif  // YOU_NLP_EXCEPTION_H_
