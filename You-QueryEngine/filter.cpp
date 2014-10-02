/// \file You-QueryEngine/internal/filter.h
/// Defines the base class for filters.
/// \author A0112054Y
#include "stdafx.h"
#include "filter.h"

namespace You {
namespace QueryEngine {

using FFilter = Filter::FFilter;
// Common filters
std::unique_ptr<Filter> Filter::anyTask() {
	auto f = [] (const Task&) -> bool {
		return true;
	};
	return std::unique_ptr<Filter>(new Filter(f));
}

Filter& Filter::operator&&(const Filter& filter) {
	ffilter = AND(ffilter, filter.ffilter);
	return *this;
}

Filter& Filter::operator||(const Filter& filter) {
	ffilter = OR(ffilter, filter.ffilter);
	return *this;
}

Filter& Filter::operator!() {
	ffilter = NOT(ffilter);
	return *this;
}

FFilter Filter::AND(const FFilter& f, const FFilter& g) {
	return [=] (FFilter::argument_type x) {
		return f(x) && g(x);
	};
}

FFilter Filter::OR(const FFilter& f, const FFilter& g) {
	return [=] (FFilter::argument_type x) {
		return f(x) || g(x);
	};
}

FFilter Filter::NOT(const FFilter& f) {
	return [=] (FFilter::argument_type x) {
		return !f(x);
	};
}

}  // namespace QueryEngine
}  // namespace You
