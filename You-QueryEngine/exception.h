#pragma once
#ifndef YOU_QUERYENGINE_EXCEPTION_H_
#define YOU_QUERYENGINE_EXCEPTION_H_

#include <stdexcept>

namespace You {
namespace QueryEngine {

class Exception : public std::exception {
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_EXCEPTION_H_
