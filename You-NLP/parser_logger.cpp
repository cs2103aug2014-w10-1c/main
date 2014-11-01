//@author A0097630B
#include "stdafx.h"
#include "parser_logger.h"

namespace You {
namespace NLP {

ParserLogger ParserLogger::log;

ParserLogger::ParserLogger() :
	std::wostream(&streamBuffer) {
}

int ParserLogger::LoggerStreamBuf::sync() {
	Utils::Log::debug << str();
	return 0;
}

}  // namespace NLP
}  // namespace You
