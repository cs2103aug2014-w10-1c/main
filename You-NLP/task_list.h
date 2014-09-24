#pragma once
#ifndef YOU_NLP_TASK_LIST_H_
#define YOU_NLP_TASK_LIST_H_

#include "../You-QueryEngine/query.h"

namespace You {
namespace NLP {

typedef ::You::QueryEngine::Task Task;

/// Defines the task list that can be returned by the engine.
typedef std::vector<Task> TaskList;

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_TASK_LIST_H_
