//@author A0094446X
#pragma once
#ifndef YOU_EXCEPTIONS_H_
#define YOU_EXCEPTIONS_H_

#include "You-Utils/exceptions/exception.h"

namespace You {
namespace Exception {

/// Base exception class.
class BaseException : public You::Utils::Exception {
};

class InitLogFileFailedException : public BaseException {
};

}  // namespace Exception
}  // namespace You

#endif  // YOU_EXCEPTIONS_H_
