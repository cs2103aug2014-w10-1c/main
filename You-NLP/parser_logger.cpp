//@author A0097630B
#include "stdafx.h"
#include "You-Utils/string_utils.h"
#include "parser_logger.h"

namespace You {
namespace NLP {

ParserLogger ParserLogger::log;

ParserLogger::ParserLogger() :
	std::wostream(&streamBuffer) {
}

ParserLogger& ParserLogger::operator<<(const std::string& string) {
	static_cast<std::wostream&>(*this) << string;
	return *this;
}

ParserLogger& ParserLogger::operator<<(
	std::wostream& (*manip)(std::wostream&)) {
	manip(*this);
	return *this;
}

int ParserLogger::LoggerStreamBuf::sync() {
	std::wstring buffer(str());
	str(std::wstring());

	// Remove trailing newlines.
	boost::trim_right(buffer);
	Utils::Log::debug << buffer;

	return 0;
}

}  // namespace NLP
}  // namespace You
