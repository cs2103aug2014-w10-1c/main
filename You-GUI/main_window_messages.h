//@author A0094446X
#pragma once
#ifndef YOU_GUI_MAIN_WINDOW_MESSAGES_H_
#define YOU_GUI_MAIN_WINDOW_MESSAGES_H_
#include "main_window.h"

namespace You {
namespace GUI {

const QString MainWindow::READY_MESSAGE = "Ready.";

const QString MainWindow::IO_EXCEPTION_MESSAGE =
"Error: Task file could not be loaded from disk.";

const QString MainWindow::NOT_WELL_FORMED_XML_MESSAGE =
"Error: Task file is not well formed. Please ensure the syntax of the file "
"is valid.";

const QString MainWindow::EMPTY_TASK_DESCRIPTION_MESSAGE =
"Error: Please fill in a task description.";

const QString MainWindow::TASK_NOT_FOUND_MESSAGE =
"Error: Requested task was not found.";

const QString MainWindow::PARSE_ERROR_MESSAGE =
"Error: Unable to parse: ";

const QString MainWindow::PARSER_EXCEPTION_MESSAGE =
"Error: Unable to parse input.";

const QString MainWindow::CONTEXT_INDEX_OUT_OF_RANGE_MESSAGE =
"Error: The task requested does not exist in the list.";

const QString MainWindow::CONTEXT_REQUIRED_MESSAGE =
"Error: A context is required.";

const QString MainWindow::UNKNOWN_EXCEPTION_MESSAGE =
"Error: An unknown exception was thrown.";

const QString MainWindow::CIRCULAR_DEPENDENCY_MESSAGE =
"Error: A circular dependency was detected.";

const QString MainWindow::NOT_UNDOABLE_MESSAGE =
"Error: The previous operation could not be undone.";

const QString MainWindow::PARSER_TYPE_MESSAGE =
"Error: A parser type exception was thrown..";

const QString MainWindow::RESOURCE_RED =
":/Status_red.png";

const QString MainWindow::RESOURCE_GREEN =
":/Status_green.png";

}  // namespace GUI
}  // namespace You
#endif  // YOU_GUI_MAIN_WINDOW_MESSAGES_H_
