#pragma once
#ifndef YOU_QUERYENGINE_TESTS_MOCKS_TASK_H_
#define YOU_QUERYENGINE_TESTS_MOCKS_TASK_H_

#include "../You-QueryEngine/internal/controller.h"
#include "../You-QueryEngine/internal/model.h"

namespace You {
namespace QueryEngine {
namespace UnitTests {

/// \name Various task examples for testing
/// @{

	extern Task FEED_THE_DOGGY();

	extern Task FEED_THE_KITTEN();

	extern Task SUPER_IMPORTANT_TASK();

	extern Task LESS_IMPORTANT_TASK();

	extern Task FOR_TOMMOROW();

	extern Task FOR_NEXT_WEEK();

	extern Task TEN_DEPENDENCIES();

	extern Task NINE_DEPENDENCIES();

/// @}

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_TESTS_MOCKS_TASK_H_
