//@author A0094446X
#pragma once
#ifndef YOU_GUI_BASE_MANAGER_H_
#define YOU_GUI_BASE_MANAGER_H_
#include "you_main_gui.h"

/// The class from which all components inherit.
class YouMainGUI::BaseManager : public QObject{
	Q_OBJECT
public:
	YouMainGUI * const parentGUI;
	BaseManager();
	explicit BaseManager(YouMainGUI * parentGUI);
	void setup();
};

#endif  // YOU_GUI_BASE_MANAGER_H_
