//@author A0094446X
#include "stdafx.h"
#include "syntax_highlighter.h"
namespace You {
namespace GUI {

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent) {
	HighlightingRule rule;

	actionKeywordFormat.setForeground(Qt::darkBlue);
	actionKeywordFormat.setFontWeight(QFont::Bold);
	QStringList actionKeywordFormatPatterns;
	actionKeywordFormatPatterns << "^/add\\b" << "^/edit\\b"
		<< "^/delete\\b";
	for each(const QString &pattern in actionKeywordFormatPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = actionKeywordFormat;
		highlightingRules.append(rule);
	}
	parameterNameFormat.setFontWeight(QFont::Bold);
	parameterNameFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\sset\\s(description|deadline|priority)");
	rule.format = parameterNameFormat;
	highlightingRules.append(rule);
}


void SyntaxHighlighter::highlightBlock(const QString &text) {
	for each(const HighlightingRule &rule in highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
}

}  // namespace GUI
}  // namespace You
