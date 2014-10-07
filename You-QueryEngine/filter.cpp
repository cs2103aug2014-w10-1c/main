/// \file You-QueryEngine/internal/filter.h
/// Defines the base class for filters.
/// \author A0112054Y
#include "stdafx.h"
#include "filter.h"

namespace You {
namespace QueryEngine {

#pragma region Common Filters
Filter Filter::anyTask() {
	const FFilter f = [] (const Task&) {
		return true;
	};
	return Filter(f);
}

Filter Filter::idIsIn(std::vector<Task::ID> taskIDs) {
	const FFilter f = [taskIDs] (const Task& task) {
		return std::find(taskIDs.begin(), taskIDs.end(), task.getID()) !=
			taskIDs.end();
	};
	return Filter(f);
}

Filter Filter::completed() {
	const FFilter f = [] (const Task& task) {
		return task.isCompleted();
	};
	return Filter(f);
}
#pragma endregion

Filter& Filter::operator&&(const Filter& filter) {
	ffilter = AND(ffilter, filter.ffilter);
	return *this;
}

Filter& Filter::operator||(const Filter& filter) {
	ffilter = OR(ffilter, filter.ffilter);
	return *this;
}

Filter& Filter::operator!(void) {
	ffilter = NOT(ffilter);
	return *this;
}

bool Filter::operator()(const Task& task) const {
	return ffilter(task);
}

using FFilter = Filter::FFilter;

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

Filter::operator FFilter() const {
	return this->ffilter;
}

}  // namespace QueryEngine
}  // namespace You
