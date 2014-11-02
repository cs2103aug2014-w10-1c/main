//@author A0094446X
#pragma once
#ifndef YOU_GUI_STDAFX_H_
#define YOU_GUI_STDAFX_H_

#include <memory>
#include <functional>

#pragma warning(push)
#include <QApplication>
#include <QList>
#pragma warning(disable: 4127)
#include <QVector>
#include <QWidget>
#include <QtWidgets>
#pragma warning(pop)

#include <boost/date_time/gregorian/greg_month.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning(push)
#pragma warning(disable: 4127 4100 4244 4555)
#include <boost/variant.hpp>
#pragma warning(pop)

#include "qt_helpers.h"

#endif  // YOU_GUI_STDAFX_H_
