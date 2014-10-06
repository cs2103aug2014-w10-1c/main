#pragma once
#ifndef YOU_GUI_VARIANT_HANDLER
#define YOU_GUI_VARIANT_HANDLER
#include "you_main_gui.h"
struct VariantHandler : boost::static_visitor<void> {
	YouMainGUI* parentGUI;
	void operator()(You::Controller::ADD_RESULT addResult) {
		parentGUI->addTask(addResult.result);
	}
	void operator()(You::Controller::EDIT_RESULT editResult) {

	}
	void operator()(You::Controller::DELETE_RESULT deleteResult) {

	}
};

#endif  // YOU_GUI_VARIANT_HANDLER
