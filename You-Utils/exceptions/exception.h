//@author A0097630B
#pragma once
#ifndef YOU_UTILS_EXCEPTIONS_EXCEPTION_H_
#define YOU_UTILS_EXCEPTIONS_EXCEPTION_H_

#include <stdexcept>

namespace You {
namespace Utils {

class Exception : public std::exception {
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_EXCEPTIONS_EXCEPTION_H_
