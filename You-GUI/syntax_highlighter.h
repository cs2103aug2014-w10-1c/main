//@author A0094446X
#pragma once
#ifndef YOU_GUI_SYNTAX_HIGHLIGHTER_H_
#define YOU_GUI_SYNTAX_HIGHLIGHTER_H_
#include <QSyntaxHighlighter>

namespace You {
namespace GUI {

class SyntaxHighlighter : public QSyntaxHighlighter {
	Q_OBJECT

public:
	explicit SyntaxHighlighter(QTextDocument *parent = 0);

protected:
	void highlightBlock(const QString &text);

private:
	struct HighlightingRule {
		QRegExp pattern;
		QTextCharFormat format;
		QTextCharFormat secondaryFormat;
	};

	HighlightingRule makeRule(std::wstring input, QTextCharFormat format,
		QTextCharFormat secondaryFormat);
	void setColors();
	void buildRules();
	QVector<HighlightingRule> commandHighlightingRules;
	QVector<HighlightingRule> paramHighlightingRules;
	QVector<HighlightingRule> orderHighlightingRules;
	QVector<HighlightingRule> orderPrefixHighlightingRule;
	QVector<HighlightingRule> hashtagHighlightingRule;

	QTextCharFormat commandNameFormat;
	QTextCharFormat parameterNameFormat;
	QTextCharFormat parameterPrefixFormat;
	QTextCharFormat orderPrefixFormat;
	QTextCharFormat orderNameFormat;
	QTextCharFormat hashtagFormat;
};

}  // namespace GUI
}  // namespace You

#endif  // YOU_GUI_SYNTAX_HIGHLIGHTER_H_
