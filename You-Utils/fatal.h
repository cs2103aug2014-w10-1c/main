//@author A0097630B
#pragma once
#ifndef YOU_UTILS_FATAL_H_
#define YOU_UTILS_FATAL_H_

// These macros are not following naming convention because it must look like
// a function call for code coverage purposes.
#if defined(CODECOVERAGE)
	#define fatal() {}
#else
	#define fatal() assert(false); abort()
#endif

#endif  // YOU_UTILS_FATAL_H_
