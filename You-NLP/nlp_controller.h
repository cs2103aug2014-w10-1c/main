#pragma once
#ifndef YOU_NLP_NLP_CONTROLLER_H_
#define YOU_NLP_NLP_CONTROLLER_H_

namespace You {
namespace NLP {
	/// The entry-point for all communications with the rest of the You
	/// framework.
	class NLPController {
	public:
		NLPController(const NLPController&) = delete;
		NLPController& operator=(const NLPController&) = delete;

		/// Gets the singleton instance for the controller.
		static NLPController& get();

	private:
		NLPController() = default;
	};
}
}

#endif  // YOU_NLP_NLP_CONTROLLER_H_
