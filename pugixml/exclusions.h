#pragma once
#ifndef PUGIXML_EXCLUSIONS_H_
#define PUGIXML_EXCLUSIONS_H_

// A local define since there is no way to test whether a header file exists.
// If you have VS Premium, then add it to the project definition (user
// properties) file
#ifdef MS_CPP_CODECOVERAGE

/// \file Exclusions from code coverage analysis.
/// See http://msdn.microsoft.com/en-sg/library/dd537628.aspx

#include <CodeCoverage/CodeCoverage.h>
#pragma managed(push, off)

ExcludeFromCodeCoverage(pugixml, L"pugi::*");

#pragma managed(pop)

#endif  // MS_CPP_CODECOVERAGE

#endif  // PUGIXML_EXCLUSIONS_H_
