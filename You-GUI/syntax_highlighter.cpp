//@author A0094446X
#include "stdafx.h"
#include "syntax_highlighter.h"
#include "keywords.h"
namespace You {
namespace GUI {

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent) {
	HighlightingRule rule;
	std::wstringstream wss;

	commandNameFormat.setForeground(Qt::darkBlue);
	commandNameFormat.setFontWeight(QFont::Bold);
	for each(const std::wstring &cmd in COMMANDS) {
		wss << L"^" << cmd << L"\\b";
		rule.pattern = QRegExp(QString::fromStdWString(wss.str()));
		rule.format = commandNameFormat;
		rule.secondaryFormat = commandNameFormat;
		rule.tertiaryFormat = commandNameFormat;
		commandHighlightingRules.append(rule);
		wss.str(L"");
	}

	wss.str(L"");

	parameterNameFormat.setFontWeight(QFont::Bold);
	parameterNameFormat.setForeground(Qt::darkMagenta);
	parameterPrefixFormat.setFontWeight(QFont::Bold);
	parameterPrefixFormat.setForeground(Qt::darkGreen);
	for each(const std::wstring &param in PARAMS) {
		wss << L"\\s" << PARAM_PREFIX << L"\\s+" << param << L"\\b";
		rule.pattern = QRegExp(QString::fromStdWString(wss.str()));
		rule.format = parameterNameFormat;
		rule.secondaryFormat = parameterPrefixFormat;
		rule.tertiaryFormat = parameterPrefixFormat;
		paramHighlightingRules.append(rule);
		wss.str(L"");
	}

	wss.str(L"");

	orderNameFormat.setFontWeight(QFont::Bold);
	orderNameFormat.setForeground(Qt::blue);
	orderPrefixFormat.setFontWeight(QFont::Bold);
	orderPrefixFormat.setForeground(Qt::darkGreen);
	for each(const std::wstring &param in PARAMS) {
		for each(const std::wstring &order in ORDERS) {
			wss << L"\\s" << ORDER_PREFIX << L"\\s+" << param << L"\\s+"
				<< order << L"\\b";
			rule.pattern = QRegExp(QString::fromStdWString(wss.str()));
			rule.format = orderNameFormat;
			rule.secondaryFormat = orderPrefixFormat;
			rule.tertiaryFormat = parameterNameFormat;
			orderHighlightingRules.append(rule);
			wss.str(L"");
		}
	}
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
	for each(const HighlightingRule &rule in commandHighlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}

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

	for each(const HighlightingRule &rule in orderHighlightingRules) {
		QRegExp expression(rule.pattern);
		int offset = 0;

		int index = expression.indexIn(text);
		if (index != -1) {
			for (int i = 0; i < ORDERS.size(); i++) {
				if (rule.pattern.pattern().contains(QString::fromStdWString(ORDERS[i]))) {
					offset = ORDERS[i].length();
				}
			}
		}
		
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length - offset, rule.format);
			setFormat(index, ORDER_PREFIX.length() + 1, rule.secondaryFormat);
			setFormat(index + length - offset, length, rule.tertiaryFormat);
			index = expression.indexIn(text, index + length);
		}
	}
}

}  // namespace GUI
}  // namespace You
