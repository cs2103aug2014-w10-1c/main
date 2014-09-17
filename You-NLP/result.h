#pragma once
#ifndef YOU_NLP_RESULT_H_
#define YOU_NLP_RESULT_H_

#include "task_list.h"

namespace You {
namespace NLP {
	/// Defines the result of a query.
	class Result {

	private:
		union {
			/// The task list, if the type of the result if that.
			///
			/// \see TaskList
			TaskList taskList;
		};
	};
}
}

#endif  // YOU_NLP_RESULT_H_
