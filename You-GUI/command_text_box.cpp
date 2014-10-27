//@author A0094446X
#include "stdafx.h"
#include <QCompleter>
#include <QApplication>
#include "command_text_box.h"
#include "keywords.h"

namespace You {
namespace GUI {

CommandTextBox::CommandTextBox(QWidget *parent) : QTextEdit(parent) {
	setCompleter();
}

CommandTextBox::~CommandTextBox() {}

void CommandTextBox::setCompleter() {
	QStringList wordList;
	std::wstringstream wss;
	wss << PARAM_PREFIX << L" ";
	wordList.append(QString::fromStdWString(wss.str()));
	wss.str(L"");
	wss << ORDER_PREFIX << L" ";
	wordList.append(QString::fromStdWString(wss.str()));

	for each (std::wstring item in PARAMS) {
		wss.str(L"");
		wss << item << L" ";
		wordList.append(QString::fromStdWString(wss.str()));
	}

	for each (std::wstring item in COMMANDS) {
		wss.str(L"");
		wss << item << L" ";
		wordList.append(QString::fromStdWString(wss.str()));
	}

	for each (std::wstring item in ORDERS) {
		wss.str(L"");
		wss << item;
		wordList.append(QString::fromStdWString(wss.str()));
	}

	this->completer = new QCompleter(wordList, this);
	this->completer->setWidget(this);
	this->completer->setCompletionMode(QCompleter::PopupCompletion);
	this->completer->setCaseSensitivity(Qt::CaseInsensitive);
	QObject::connect(this->completer, SIGNAL(activated(QString)),
		this, SLOT(insertCompletion(QString)));
}

void CommandTextBox::insertCompletion(const QString& completion) {
	if (completer->widget() != this)
		return;
	QTextCursor tc = textCursor();
	int extra = completion.length() - completer->completionPrefix().length();
	tc.movePosition(QTextCursor::Left);
	tc.movePosition(QTextCursor::EndOfWord);
	tc.insertText(completion.right(extra));
	setTextCursor(tc);
}

QString CommandTextBox::textUnderCursor() const {
	QTextCursor tc = textCursor();
	tc.movePosition(QTextCursor::StartOfWord);
	if (tc.position() != 0) {
		if (this->toPlainText().at(tc.position() - 1) == "/") {
			tc.setPosition(tc.position() - 1);
			tc.movePosition(tc.NextWord, QTextCursor::KeepAnchor, 2);
			return tc.selectedText();
		} else {
			tc.movePosition(tc.NextWord, QTextCursor::KeepAnchor, 1);
			return tc.selectedText();
		}
	}
	return tc.selectedText();
}

void CommandTextBox::focusInEvent(QFocusEvent *e) {
	if (completer)
		completer->setWidget(this);
	QTextEdit::focusInEvent(e);
}

void CommandTextBox::keyPressEvent(QKeyEvent *e) {
	if (completer && completer->popup()->isVisible()) {
		// The following keys are forwarded by the completer to the widget
		switch (e->key()) {
		case Qt::Key_Enter:
		case Qt::Key_Return:
		case Qt::Key_Escape:
		case Qt::Key_Tab:
		case Qt::Key_Backtab:
			e->ignore();
			return;
		default:
			break;
		}
	}
	QTextEdit::keyPressEvent(e);
	static QString eow("~!@#$%^&*()_+{}|:\"<>?,.;'[]\\-=");
	QString completionPrefix = textUnderCursor();
	if ((e->text().isEmpty() || completionPrefix.length() < 1
		|| eow.contains(e->text().right(1)))) {
		completer->popup()->hide();
		return;
	}
	if (completionPrefix != completer->completionPrefix()) {
		completer->setCompletionPrefix(completionPrefix);
		completer->popup()->setCurrentIndex(
			completer->completionModel()->index(0, 0));
	}

	QRect cr = cursorRect();
	cr.setWidth(completer->popup()->sizeHintForColumn(0)
		+ completer->popup()->verticalScrollBar()->sizeHint().width());
	completer->complete(cr);
}
}  // namespace GUI
}  // namespace You
