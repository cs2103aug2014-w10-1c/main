//@author A0094446X
#include "stdafx.h"
#include "syntax_highlighter.h"
#include "keywords.h"
namespace You {
namespace GUI {

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent) {
	setColors();
	buildRules();
}

SyntaxHighlighter::HighlightingRule SyntaxHighlighter::makeRule(
	std::wstring input,	QTextCharFormat format,
	QTextCharFormat secondaryFormat) {
	HighlightingRule rule;
	rule.pattern = QRegExp(QString::fromStdWString(input));
	rule.format = format;
	rule.secondaryFormat = secondaryFormat;
	return rule;
}

void SyntaxHighlighter::setColors() {
	commandNameFormat.setForeground(Qt::darkBlue);
	commandNameFormat.setFontWeight(QFont::Bold);	
	
	parameterNameFormat.setFontWeight(QFont::Bold);
	parameterNameFormat.setForeground(Qt::darkMagenta);

	parameterPrefixFormat.setFontWeight(QFont::Bold);
	parameterPrefixFormat.setForeground(Qt::darkGreen);

	orderNameFormat.setFontWeight(QFont::Bold);
	orderNameFormat.setForeground(Qt::blue);

	orderPrefixFormat.setFontWeight(QFont::Bold);
	orderPrefixFormat.setForeground(Qt::darkGreen);
}

void SyntaxHighlighter::buildRules() {
	std::wstringstream wss;
	for each(const std::wstring &cmd in COMMANDS) {
		wss << L"^" << cmd << L"\\b";
		commandHighlightingRules.append(makeRule(wss.str(),
			commandNameFormat, commandNameFormat));
		wss.str(L"");
	}

	for each(const std::wstring &param in PARAMS) {
		wss << L"\\s" << PARAM_PREFIX << L"\\s+" << param << L"\\b";
		paramHighlightingRules.append(makeRule(wss.str(),
			parameterNameFormat, parameterPrefixFormat));
		wss.str(L"");
	}

	for each(const std::wstring &param in PARAMS) {
		for each(const std::wstring &order in ORDERS) {
			wss << L"\\s+" << param << L"\\s+" << order << L"\\b";
			orderHighlightingRules.append(makeRule(wss.str(),
				orderNameFormat, parameterNameFormat));
			wss.str(L"");
		}
	}

	wss << L"\\s+" << ORDER_PREFIX << L"\\s+";
	orderPrefixHighlightingRule.append(makeRule(wss.str(),
		orderPrefixFormat, orderPrefixFormat));
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
	// Highlight the basic commands
	for each(const HighlightingRule &rule in commandHighlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}

	// Highlight parameters and the 'set' prefix such as 'set description'
	for each(const HighlightingRule &rule in paramHighlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			setFormat(index, PARAM_PREFIX.length() + 1, rule.secondaryFormat);
			index = expression.indexIn(text, index + length);
		}
	}

	// Highlights the 'order by' keywords
	QRegExp expression(orderPrefixHighlightingRule.at(0).pattern);
	HighlightingRule rule = orderPrefixHighlightingRule.at(0);
	int index = expression.indexIn(text);
	while (index >= 0) {
		int length = expression.matchedLength();
		setFormat(index, length, rule.format);
		index = expression.indexIn(text, index + length);
	}

	// Highlights parameter 'ascending' or 'descending'
	for each(const HighlightingRule &rule in orderHighlightingRules) {
		QRegExp expression(rule.pattern);
		int offset = 0;
		int index = expression.indexIn(text);
		if (index != -1) {
			for (int i = 0; i < ORDERS.size(); i++) {
				if (rule.pattern.pattern().contains(
					QString::fromStdWString(ORDERS[i]))) {
					offset = ORDERS[i].length();
				}
			}
		}
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length - offset, rule.format);
			setFormat(index + length - offset, 
				length - offset, rule.secondaryFormat);
			index = expression.indexIn(text, index + length);
		}
	}
}

}  // namespace GUI
}  // namespace You
