//@author A0094446X
#pragma once
#ifndef YOU_GUI_YOU_MAIN_GUI_MESSAGES_H_
#define YOU_GUI_YOU_MAIN_GUI_MESSAGES_H_
#include "stdafx.h"
#include "you_main_gui.h"

const QString YouMainGUI::READY_MESSAGE = "Ready.";

const QString YouMainGUI::EMPTY_TASK_DESCRIPTION_MESSAGE =
"Error: Please fill in a task description.";

const QString YouMainGUI::TASK_NOT_FOUND_MESSAGE =
"Error: Requested task was not found.";

const QString YouMainGUI::PARSE_ERROR_MESSAGE =
"Error: Unable to parse input.";

const QString YouMainGUI::PARSER_EXCEPTION_MESSAGE =
"Error: Unable to parse input.";

const QString YouMainGUI::CONTEXT_INDEX_OUT_OF_RANGE_MESSAGE =
"Error: The task requested does not exist in the list.";

const QString YouMainGUI::CONTEXT_REQUIRED_MESSAGE =
"Error: A context is required.";

const QString YouMainGUI::RESOURCE_RED =
":/Status_red.png";

const QString YouMainGUI::RESOURCE_GREEN =
":/Status_green.png";

#endif  // YOU_GUI_YOU_MAIN_GUI_MESSAGES_H_
