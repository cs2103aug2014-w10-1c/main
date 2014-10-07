#pragma once
#ifndef YOU_NLP_PARSE_TREE_TASK_PRIORITIES_H_
#define YOU_NLP_PARSE_TREE_TASK_PRIORITIES_H_

namespace You {
namespace NLP {
		
/// The possible priorities for tasks.
enum class TASK_PRIORITY {
	/// Normal task priority
	NORMAL = 0,

	/// High task priority
	HIGH
};

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_PARSE_TREE_TASK_PRIORITIES_H_
