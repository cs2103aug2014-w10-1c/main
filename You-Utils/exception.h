#pragma once
#ifndef YOU_UTILS_EXCEPTIONS_EXCEPTION_H_
#define YOU_UTILS_EXCEPTIONS_EXCEPTION_H_

#include <stdexcept>

namespace You {
namespace NLP {

class Exception : public std::exception {
};

}  // namespace NLP
}  // namespace You

#endif  // YOU_UTILS_EXCEPTIONS_EXCEPTION_H_
