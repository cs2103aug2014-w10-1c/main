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
const std::wstring COMMAND_ATTACH = L"/attach";
const std::array<std::wstring, 6> COMMANDS = { COMMAND_SHOW, COMMAND_ADD,
COMMAND_DELETE, COMMAND_EDIT, COMMAND_UNDO, COMMAND_ATTACH };

const std::wstring PARAM_PREFIX = L"set";

const std::wstring PARAM_DESCRIPTION = L"description";
const std::wstring PARAM_START = L"deadline";
const std::wstring PARAM_DEADLINE = L"deadline";
const std::wstring PARAM_PRIORITY = L"priority";
const std::wstring PARAM_DEPENDENCIES = L"dependencies";

const std::array<std::wstring, 5> PARAMS = { PARAM_DESCRIPTION, PARAM_START,
PARAM_DEADLINE, PARAM_PRIORITY, PARAM_DEPENDENCIES };

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
const std::wstring SYMBOL_TRUE = L"true";
const std::wstring SYMBOL_FALSE = L"false";
const std::wstring SYMBOL_BEFORE = L"before";
const std::wstring SYMBOL_AFTER = L"after";
const std::wstring SYMBOL_FROM = L"from";
const std::wstring SYMBOL_TO = L"to";
const std::wstring SYMBOL_BY = L"by";

const std::wstring SYMBOL_LESS_THAN_EQUALS = L"<=";
const std::wstring SYMBOL_MORE_THAN_EQUALS = L">=";


const std::array<std::wstring, 17> SYMBOLS = { SYMBOL_SUBTASK,
SYMBOL_SIBLING, SYMBOL_HASHTAG, SYMBOL_DEPENDENCY, SYMBOL_LESS_THAN,
SYMBOL_LESS_THAN, SYMBOL_EQUALS, SYMBOL_IMPT, SYMBOL_TRUE, SYMBOL_FALSE,
SYMBOL_LESS_THAN_EQUALS, SYMBOL_MORE_THAN_EQUALS, SYMBOL_BEFORE, SYMBOL_AFTER,
SYMBOL_FROM, SYMBOL_TO, SYMBOL_BY};

}  // namespace GUI
}  // namespace You
#endif  // YOU_GUI_KEYWORDS_H_
