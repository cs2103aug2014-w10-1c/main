#pragma once
#ifndef YOU_NLP_CONTROLLER_H_
#define YOU_NLP_CONTROLLER_H_

#include <string>
#include "result.h"

namespace You {
namespace NLP {
	/// The entry-point for all communications with the rest of the You
	/// framework.
	class Controller {
	public:
		Controller(const Controller&) = delete;
		Controller& operator=(const Controller&) = delete;

		/// Gets the singleton instance for the controller.
		static Controller& get();

		/// Executes the given query.
		/// \param[in] query The query to execute.
		Result query(const std::wstring& query) const;

	private:
		Controller() = default;
	};
}
}

#endif  // YOU_NLP_CONTROLLER_H_
