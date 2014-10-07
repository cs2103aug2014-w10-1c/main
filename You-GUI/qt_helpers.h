//@author A0097630B
#pragma once
#ifndef YOU_GUI_QT_HELPERS_H_
#define YOU_GUI_QT_HELPERS_H_

#include <QString>
#include <boost/lexical_cast.hpp>

namespace boost {
namespace detail {

/// Partial template specialisation for converting to QString. This is modified
/// from the boost::lexical_cast implementation.
template<typename Source>
struct lexical_converter_impl<QString, Source> {
	typedef lexical_cast_stream_traits<Source, QString> stream_trait;

	typedef detail::lexical_istream_limited_src<
		BOOST_DEDUCED_TYPENAME stream_trait::char_type,
		BOOST_DEDUCED_TYPENAME stream_trait::traits,
		stream_trait::requires_stringbuf,
		stream_trait::len_t::value + 1
	> i_interpreter_type;

	/// Converts the given argument to a QString.
	static inline bool try_convert(const Source& arg, QString& result) {
		i_interpreter_type i_interpreter;

		// Disabling ADL, by directly specifying operators.
		if (!(i_interpreter.operator <<(arg)))
			return false;

		// Copy from the stream.
		result = QString::fromWCharArray(
			i_interpreter.cbegin(),
			i_interpreter.cend() - i_interpreter.cbegin());

		return true;
	}
};

}  // namespace detail
}  // namespace boost

/// operator<< overload for boost::lexical_cast to work with QStrings.
///
/// \param[in] stream The stream to write to
/// \param[in] str The string to write.
/// \return The stream given, for chaining.
inline std::wostream& operator<<(std::wostream& stream, const QString& str) {
	return stream << str.data();
}

/// operator>> overload for boost::lexical_cast to work with QStrings.
///
/// \param[in] stream The stream to read from
/// \param[in] str The string to write to.
/// \return The stream given, for chaining.
inline std::wistream& operator>>(std::wistream& stream, QString& str) {
	std::wstring temp;
	stream >> temp;

	str = QString::fromStdWString(temp);
	return stream;
}

#endif  // YOU_GUI_QT_HELPERS_H_
