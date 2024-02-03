/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov
 homepage http://slovesnov.users.sourceforge.net/?calculator
 email slovesnov@yandex.ru
 All rights reserved.
 file created 3 jul 2014
 ******************************************************/

#ifndef CALCULATORWINDOW_H_
#define CALCULATORWINDOW_H_

#include "aslov.h"

static VString languageString[] =
		{ { "clear", "recount", "copy to memory", "add to buffer",
				"clear buffer", "expression", "result", "common functions",
				"constants", "trigonometric functions", "hyperbolic function",
				"rounding and additional functions", "memory", "buffer",
				"error", "scientific calculator", "program %s version %.2lf",
				"author aleksey slovesnov",
				"copyright (C/C++/gtk) 2002 - doomsday", "homepage" }, {
				"очистить", "пересчитать", "копировать в память",
				"добавить в буфер", "очистить буфер", "выражение", "результат",
				"общие функции", "константы", "тригонометрические функции",
				"гиперболические функции",
				"дополнительные функции и функции округления", "память",
				"буфер", "ошибка", "научный калькулятор",
				"программа %s версия %.2lf", "автор Алексей Словеснов",
				"copyright (C/C++/gtk) 2002 - второго пришествия",
				"домашняя страница" }, };

/* Notes
 * CLEAR, RECOUNT, COPY_TO_MEMORY, ADD_TO_BUFFER, CLEAR_BUFFER should go in a row
 * CLEAR should goes first CLEAR=0
 */
enum STRING_ENUM {
	CLEAR,
	RECOUNT,
	COPY_TO_MEMORY,
	ADD_TO_BUFFER,
	CLEAR_BUFFER,
	EXPRESSION,
	RESULT,
	COMMON_FUNCTIONS,
	CONSTANTS,
	TRIGONOMETRIC_FUNCTIONS,
	HYPERBOLIC_FUNCTIONS,
	ROUNDING_FUNCTIONS,
	MEMORY,
	BUFFER,
	ERROR,
	SCIENTIFIC_CALCULATOR,
	PROGRAM_VERSION,
	AUTHOR,
	COPYRIGHT,
	HOMEPAGE_STRING
};

const STRING_ENUM LABEL[] = { EXPRESSION, RESULT, MEMORY, BUFFER };

enum ENTRY_ENUM {
	ENTRY_INPUT,
	ENTRY_RESULT,
	ENTRY_MEMORY,
	ENTRY_BUFFER,
	ENTRY_A,
	ENTRY_B,
	ENTRY_C,
	ENTRY_D
};

const ENTRY_ENUM ENTRY_ENUM_ARRAY[] = { ENTRY_INPUT, ENTRY_RESULT, ENTRY_MEMORY,
		ENTRY_BUFFER, ENTRY_A, ENTRY_B, ENTRY_C, ENTRY_D };

static VString lexer[] = { { "exp()", "log()", "pow(,)", "sqrt()", "abs()",
		"random()", "min(,)", "max(,)" }, { "pi", "e", "sqrt2", "sqrt1_2",
		"ln2", "ln10", "log2e", "log10e" }, { "sin()", "cos()", "tan()",
		"cot()", "sec()", "csc()", "asin()", "acos()", "atan()", "acot()",
		"asec()", "acsc()" }, { "sinh()", "cosh()", "tanh()", "coth()",
		"sech()", "csch()", "asinh()", "acosh()", "atanh()", "acoth()",
		"asech()", "acsch()" }, { "ceil()", "floor()", "round()", "atan2(,)" } };

class CalculatorWindow {
	GtkWidget *m_window;
	GtkWidget *m_frame[SIZE(lexer)];
	static const int M_BUTTONS_LINE1 = 2;
	static const int M_BUTTONS_LINE3 = 3;
	static const int M_BUTTONS_SIZE = M_BUTTONS_LINE1 + M_BUTTONS_LINE3;
	GtkWidget *m_button[M_BUTTONS_SIZE];
	GtkWidget *m_aboutButton;
	GtkWidget *m_label[SIZE(LABEL)];
	GtkWidget *m_entry[SIZE(ENTRY_ENUM_ARRAY)];
	std::vector<GtkWidget*> m_functionButton;
	int m_language;

	const gchar* getLanguageString(STRING_ENUM e) {
		return g_locale_to_utf8(languageString[m_language][e].c_str(), -1, NULL,
				NULL, NULL);
	}

	void addItemToTable(GtkWidget *table, GtkWidget *w, int row, int column);
	GtkTreeModel* createModel();
	static unsigned countArguments(const char *function);

	void updateLanguage();

	std::string getText(ENTRY_ENUM e);
	void setText(ENTRY_ENUM e, std::string s);

	inline bool isSpecial(const char c) {
		return c != '_' && !isalpha(c) && !isdigit(c);
	}

public:
	CalculatorWindow();
	virtual ~CalculatorWindow();

	void changeLanguage(int language);
	void clickButton(GtkWidget *widget);
	void inputChanged();

	void aboutDialog();
};

#endif /* CALCULATORWINDOW_H_ */
