//@author A0097630B
#pragma once
#ifndef YOU_UTILS_EXCEPTIONS_EXCEPTION_H_
#define YOU_UTILS_EXCEPTIONS_EXCEPTION_H_

#include <stdexcept>

// http://stackoverflow.com/questions/3585144
// Basically: the warning is a false positive.
#pragma warning(disable: 4672 4673)

namespace You {
namespace Utils {

class Exception : virtual public std::exception {
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_EXCEPTIONS_EXCEPTION_H_
