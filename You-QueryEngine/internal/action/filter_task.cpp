/// \author A0112054Y
#include "stdafx.h"

#include  "../task_serializer.h"
#include "filter_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {

Response FilterTask::execute() {
	using STask = TaskSerializer::STask;
	std::vector<STask> slist;
	std::vector<Task> list;
	auto filter = this->filter;
	auto sfilter = [=] (const STask& s) -> bool {
		auto deserialized = TaskSerializer::deserialize(s);
		return filter(const_cast<Task&>(deserialized));
	};
#if 0
	Transaction t(dataStorage.begin());
	slist = t.filter(sfilter);
	t.commit();
#endif
	std::for_each(slist.cbegin(), slist.cend(),
		[&list] (const STask s) {
			list.push_back(TaskSerializer::deserialize(s));
		}
	);
	return list;
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You
