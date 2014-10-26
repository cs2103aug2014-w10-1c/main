//@author A0094446X
#pragma once
#ifndef YOU_GUI_KEYWORDS_H_
#define YOU_GUI_KEYWORDS_H_

namespace You {
namespace GUI {
const std::wstring COMMAND_ADD = L"/add";
const std::wstring COMMAND_DELETE = L"/delete";
const std::wstring COMMAND_EDIT = L"/edit";

const std::wstring COMMANDS[] = { COMMAND_ADD, COMMAND_DELETE, COMMAND_EDIT };

const std::wstring PARAM_PREFIX = L"set";

const std::wstring PARAM_DESCRIPTION = L"description";
const std::wstring PARAM_DEADLINE = L"deadline";
const std::wstring PARAM_PRIORITY = L"priority";
const std::wstring PARAM_DEPENDENCIES = L"dependencies";

const std::wstring PARAMS[] = { PARAM_DESCRIPTION, PARAM_DEADLINE,
PARAM_PRIORITY, PARAM_DEPENDENCIES };

}
}
#endif  // YOU_GUI_KEYWORDS_H_
