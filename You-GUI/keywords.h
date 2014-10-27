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
const std::array<std::wstring, 4> COMMANDS = { COMMAND_SHOW, COMMAND_ADD,
COMMAND_DELETE, COMMAND_EDIT };

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

}
}
#endif  // YOU_GUI_KEYWORDS_H_
