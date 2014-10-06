/// \author A0112054Y
#pragma once
#ifndef YOU_QUERYENGINE_TESTS_COMMON_H_
#define YOU_QUERYENGINE_TESTS_COMMON_H_

#include "CppUnitTest.h"
#include "task_model.h"

#define USE_WSTRING_TEST_WRITER(X) namespace Microsoft {\
namespace VisualStudio {\
namespace CppUnitTestFramework {\
\
template <>\
static std::wstring ToString<X> (const X& t) {\
	return boost::lexical_cast<std::wstring>(t);\
}}}}

#endif  // YOU_QUERYENGINE_TESTS_COMMON_H_
