#pragma once
#ifndef YOU_CONTROLLER_TESTS_MOCKS_QUERY_H_
#define YOU_CONTROLLER_TESTS_MOCKS_QUERY_H_

#include "You-NLP/parse_tree.h"

namespace You {
namespace Controller {
namespace UnitTests {
namespace Mocks {
namespace Queries {

/// A default Add Query.
extern const You::NLP::ADD_QUERY ADD_QUERY;

/// A default Edit Query.
extern const You::NLP::EDIT_QUERY EDIT_QUERY;

}  // namespace Queries
}  // namespace Mocks
}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_TESTS_MOCKS_QUERY_H_
