#ifndef YOU_GUI_TESTS_GLOBAL_H
#define YOU_GUI_TESTS_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef YOU_GUI_TESTS_LIB
# define YOU_GUI_TESTS_EXPORT Q_DECL_EXPORT
#else
# define YOU_GUI_TESTS_EXPORT Q_DECL_IMPORT
#endif

#endif // YOU-GUI-TESTS_GLOBAL_H
