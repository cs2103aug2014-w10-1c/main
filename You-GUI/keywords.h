//@author A0094446X
#pragma once
#ifndef YOU_GUI_KEYWORDS_H_
#define YOU_GUI_KEYWORDS_H_

namespace You {
namespace GUI {

const std::wstring COMMAND_ADD = L"/add";
const std::wstring COMMAND_DELETE = L"/delete";
const std::wstring COMMAND_EDIT = L"/edit";
const std::wstring COMMAND_SHOW = L"/show";
const std::wstring COMMAND_UNDO = L"/undo";
const std::wstring COMMAND_REDO = L"/redo";
const std::wstring COMMAND_ATTACH = L"/attach";
const std::array<std::wstring, 6> COMMANDS = { COMMAND_SHOW, COMMAND_ADD,
COMMAND_DELETE, COMMAND_EDIT, COMMAND_UNDO, COMMAND_ATTACH };

const std::wstring PARAM_PREFIX = L"set";

const std::wstring PARAM_DESCRIPTION = L"description";
const std::wstring PARAM_DEADLINE = L"deadline";
const std::wstring PARAM_PRIORITY = L"priority";
const std::wstring PARAM_DEPENDENCIES = L"dependencies";

const std::array<std::wstring, 4> PARAMS = { PARAM_DESCRIPTION, PARAM_DEADLINE,
PARAM_PRIORITY, PARAM_DEPENDENCIES };

const std::wstring ORDER_PREFIX = L"order by";
const std::wstring ORDER_ASCENDING = L"ascending";
const std::wstring ORDER_DESCENDING = L"descending";

const std::array<std::wstring, 2> ORDERS = { ORDER_ASCENDING,
ORDER_DESCENDING };

const std::wstring SYMBOL_SUBTASK = L":";
const std::wstring SYMBOL_SIBLING = L";";
const std::wstring SYMBOL_HASHTAG = L"#";
const std::wstring SYMBOL_DEPENDENCY = L"->";
const std::wstring SYMBOL_LESS_THAN = L"<";
const std::wstring SYMBOL_MORE_THAN = L">";
const std::wstring SYMBOL_EQUALS = L"=";
const std::wstring SYMBOL_IMPT = L"!";

const std::array<std::wstring, 8> SYMBOLS = { SYMBOL_SUBTASK,
SYMBOL_SIBLING, SYMBOL_HASHTAG, SYMBOL_DEPENDENCY, SYMBOL_LESS_THAN,
SYMBOL_LESS_THAN, SYMBOL_EQUALS, SYMBOL_IMPT };

}  // namespace GUI
}  // namespace You
#endif  // YOU_GUI_KEYWORDS_H_
