#pragma once
#ifndef YOU_NLP_TESTS_EXCLUSIONS_H_
#define YOU_NLP_TESTS_EXCLUSIONS_H_

// A local define since there is no way to test whether a header file exists.
// If you have VS Premium, then add it to the project definition (user
// properties) file
#ifdef MS_CPP_CODECOVERAGE

/// \file Exclusions from code coverage analysis.
/// See http://msdn.microsoft.com/en-sg/library/dd537628.aspx

#include <CodeCoverage/CodeCoverage.h>
#pragma managed(push, off)

ExcludeFromCodeCoverage(boost, L"boost::*");
ExcludeFromCodeCoverage(boost, L"You::QueryEngine::*");

#pragma managed(pop)

#endif  // MS_CPP_CODECOVERAGE

#endif  // YOU_NLP_TESTS_EXCLUSIONS_H_
