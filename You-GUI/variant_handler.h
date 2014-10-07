//@author A0094446X
#pragma once
#ifndef YOU_GUI_VARIANT_HANDLER_H_
#define YOU_GUI_VARIANT_HANDLER_H_
#include "you_main_gui.h"

struct VariantHandler : boost::static_visitor<void> {
	YouMainGUI* parentGUI;
	void operator()(You::Controller::ADD_RESULT addResult) {
		parentGUI->addTask(addResult.task);
	}
	void operator()(You::Controller::EDIT_RESULT editResult) {
		parentGUI->editTask(editResult.task);
	}
	void operator()(You::Controller::DELETE_RESULT deleteResult) {
		parentGUI->deleteTask(deleteResult.task);
	}
};

#endif  // YOU_GUI_VARIANT_HANDLER_H_
