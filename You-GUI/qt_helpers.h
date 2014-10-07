//@author A0097630B
#pragma once
#ifndef YOU_GUI_QT_HELPERS_H_
#define YOU_GUI_QT_HELPERS_H_

#include <QString>

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
