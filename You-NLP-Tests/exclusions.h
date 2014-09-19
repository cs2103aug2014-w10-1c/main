#pragma once
#ifndef YOU_NLP_TESTS_EXCLUSIONS_H_
#define YOU_NLP_TESTS_EXCLUSIONS_H_

/// \file Exclusions from code coverage analysis.
/// See http://msdn.microsoft.com/en-sg/library/dd537628.aspx

#include <CodeCoverage/CodeCoverage.h>
#pragma managed(push, off)

ExcludeFromCodeCoverage(boost, L"boost::*");

#pragma managed(pop)
#endif  // YOU_NLP_TESTS_EXCLUSIONS_H_
