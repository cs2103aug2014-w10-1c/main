#pragma once
#ifndef YOU_CONTROLLER_TESTS_EXCLUSIONS_H_
#define YOU_CONTROLLER_TESTS_EXCLUSIONS_H_

// A local define since there is no way to test whether a header file exists.
// If you have VS Premium, then add it to the project definition (user
// properties) file
#ifdef MS_CPP_CODECOVERAGE

/// \file Exclusions from code coverage analysis.
/// See http://msdn.microsoft.com/en-sg/library/dd537628.aspx

#include <CodeCoverage/CodeCoverage.h>
#pragma managed(push, off)

ExcludeFromCodeCoverage(boost, L"boost::*");
ExcludeFromCodeCoverage(boost_meta, L"??@*@");
ExcludeFromCodeCoverage(You_NLP, L"You::NLP::*");
ExcludeFromCodeCoverage(You_QueryEngine, L"You::QueryEngine::*");
ExcludeFromCodeCoverage(You_DataStore, L"You::DataStore::*");
ExcludeFromCodeCoverage(You_Utils, L"You::Utils::*");

#pragma managed(pop)

#endif  // MS_CPP_CODECOVERAGE

#endif  // YOU_CONTROLLER_TESTS_EXCLUSIONS_H_
