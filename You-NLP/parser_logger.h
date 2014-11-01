//@author A0097630B
#pragma once
#ifndef YOU_NLP_PARSER_LOGGER_H_
#define YOU_NLP_PARSER_LOGGER_H_

#include "You-Utils/log.h"

namespace You {
namespace NLP {

class ParserLogger : public std::wostream {
public:
	ParserLogger();
	~ParserLogger() = default;

	/// The global parser logger.
	static ParserLogger log;

	template<typename TObject>
	ParserLogger& operator<<(TObject&& value) {
		*static_cast<std::wostream*>(this) <<
			boost::lexical_cast<std::wstring>(value);
		return *this;
	}

	/// Overload for writing strings. The names of rules are strings.
	ParserLogger& operator<<(const std::string& string);

	/// Compatibility with ostream manipulators. This is a no-op.
	ParserLogger& operator<<(std::wostream& (*manip)(std::wostream&));

private:
	class LoggerStreamBuf : public std::basic_stringbuf<wchar_t> {
	public:
		int sync() override;
	};

private:
	ParserLogger& operator=(const ParserLogger&) = delete;

private:
	/// The stream buffer holding the contents of the log message.
	LoggerStreamBuf streamBuffer;
};

}  // namespace NLP
}  // namespace You

namespace boost {
namespace spirit {
namespace qi {
namespace detail {

template<typename Char>
inline void token_printer(You::NLP::ParserLogger& m, Char c) {
	m << c;
}

}
}
}
}

#include <boost/fusion/support/is_sequence.hpp>
namespace boost {
namespace fusion {

template<typename Sequence>
inline std::wostream&
	out(std::wostream& os, Sequence& seq) {
	detail::print_sequence(os, seq);
	return os;
}

namespace operators {

template <typename Sequence>
inline typename
	boost::enable_if<
		boost::fusion::traits::is_sequence<Sequence>,
		std::wostream&
	>::type
	operator<<(std::wostream& os, Sequence const& seq) {
	return fusion::out(os, seq);
}

}  // namespace operators
}  // namespace fusion
}  // namespace boost

#pragma endregion

#endif  // YOU_NLP_PARSER_LOGGER_H_
