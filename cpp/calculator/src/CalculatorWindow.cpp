/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov
 homepage http://slovesnov.users.sourceforge.net/?calculator
 email slovesnov@yandex.ru
 All rights reserved.
 file created 3 jul 2014
 ******************************************************/

#include <cassert>
#include <cmath>
#include "CalculatorWindow.h"
#include "ExpressionEstimator.h"
#include "aslov.h"

enum {
	PIXBUF_COL, TEXT_COL
};

const char HOMEPAGE[] = "http://slovesnov.users.sf.net/?calculator"; //use short name because string is used in about dialog
const char CERROR[] = "cerror";
const char MAIL[] = "slovesnov@yandex.ru";
const std::string LNG[] = { "en", "ru" };
const std::string LANGUAGE[] = { "english", "russian" };
const std::string CONFIG_TAGS[] = { "version", "language" };

const STRING_ENUM GROUP_TITLE[] = { COMMON_FUNCTIONS, CONSTANTS,
		TRIGONOMETRIC_FUNCTIONS, HYPERBOLIC_FUNCTIONS, ROUNDING_FUNCTIONS, };
CalculatorWindow *pWindow;

static gboolean combo_changed(GtkComboBox *comboBox, gpointer) {
	pWindow->changeLanguage(gtk_combo_box_get_active(comboBox));
	return TRUE;
}

static void button_clicked(GtkWidget *widget, gpointer) {
	pWindow->clickButton(widget);
}

static void input_changed(GtkWidget*, gpointer) {
	pWindow->inputChanged();
}

static gboolean label_clicked(GtkWidget *label, const gchar *uri, gpointer) {
	openURL(uri);
	return TRUE;
}

void CalculatorWindow::changeLanguage(int language) {
	m_language = language;
	updateLanguage();
}

std::string CalculatorWindow::getText(ENTRY_ENUM e) {
	const gchar *buffer;
	if (e == ENTRY_BUFFER) {
		GtkTextIter start, end;
		GtkTextBuffer *tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_entry[e]));
		gtk_text_buffer_get_start_iter(tb, &start);
		gtk_text_buffer_get_end_iter(tb, &end);
		buffer = gtk_text_buffer_get_text(tb, &start, &end, TRUE);
	} else {
		buffer = gtk_entry_get_text(GTK_ENTRY(m_entry[e]));
	}
	return buffer;
}

void CalculatorWindow::setText(ENTRY_ENUM e, std::string s) {
	if (e == ENTRY_BUFFER) {
		GtkTextBuffer *tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_entry[e]));
		gtk_text_buffer_set_text(tb, s.c_str(), s.length());
	} else {
		gtk_entry_set_text(GTK_ENTRY(m_entry[e]), s.c_str());
	}
}

void CalculatorWindow::inputChanged() {
	int i;
	const char *p;
	double v;
	std::string::iterator it, prev, next;
	bool error = false;
	std::string s = getText(ENTRY_INPUT), s1;
	if (s.length() != 0) {
		try {
			if (s.find('#') != std::string::npos) {
				throw std::runtime_error("unknown char found");
			}
			for (i = 4; i < 8; i++) {
				for (it = s.begin(); it != s.end(); it++) {
					next = it;
					next++;
					if ((*it == 'a' + (i - 4) || *it == 'A' + (i - 4))
							&& (it == s.begin() || isSpecial(*prev))
							&& (next == s.end() || isSpecial(*next))) {
						*it = '#';
					}
					prev = it;
				}
				s = replaceAll(s, "#",
						"(" + getText(ENTRY_ENUM_ARRAY[i]) + ")");
			}

			v = ExpressionEstimator::calculate(s.c_str());
			s1 = format("%.15lf", v);
			i = s1.length();
			//starts from s1.c_str() + i - 2. -2 because 5.000000000000001 need to skip last digit
			if (s1[i - 2] == '0') {
				for (p = s1.c_str() + i - 2; *p == '0'; p--)
					;
				if (*p != '.') {
					p++;
				}
				s = s1.substr(0, p - s1.c_str());
			} else if (s1[i - 2] == '9') {
				for (p = s1.c_str() + i - 2; *p == '9'; p--)
					;
				printl(*p=='.')
				if (*p == '.') {
					//originally was pow(sqrt(12), 2)=11.999999999999998 now make s=12
					//round works correct with negative numbers
					s = std::to_string(int(std::round(v)));
				} else {
					//pow(sqrt(19.99), 2)=19.989999999999998
					p++;
					s = s1.substr(0, p - s1.c_str());
					//originally was 19.989999999999998 now s=19.98 make s=19.99
					s[s.length() - 1]++;
				}
			} else if (s1[i - 1] == '0') {
				//just last one zero because s1[i-2]!='0' checked upper
				//pow(sqrt(1.23945632114677),2)=1.239456321146770
				s = s1.substr(0, i - 1);
			} else {
				s = s1;
			}
		} catch (std::exception &e) {
			error = true;
			s = getLanguageString(ERROR);
		}
	}
	setText(ENTRY_RESULT, s);

	for (i = 0; i < 2; i++) {
		if (error) {
			addClass(m_entry[i], CERROR); //red font
		} else {
			removeClass(m_entry[i], CERROR); //normal font, all valid
		}
	}

}

void CalculatorWindow::clickButton(GtkWidget *widget) {
	int i;
	std::string s, q, functionName;
	const gchar *function;
	gint start, end;

	i = 0;
	for (auto b : m_button) {
		if (widget == b) {
			break;
		}
		i++;
	}
	if (i == M_BUTTONS_SIZE) {
		if (widget == m_aboutButton) {
			aboutDialog();
		} else {
			gtk_editable_get_selection_bounds(
					GTK_EDITABLE(m_entry[ENTRY_INPUT]), &start, &end);
			s = getText(ENTRY_INPUT);
			function = gtk_button_get_label(GTK_BUTTON(widget));

			functionName = function;
			functionName = functionName.substr(0, functionName.find('('));

			q = s.substr(0, start) + functionName;
			i = countArguments(function);
			if (i == 1 || i == 2) {
				q += "(" + s.substr(start, end - start) + (i == 2 ? "," : "")
						+ ")";
			}

			if (unsigned(end) != s.length()) {
				q += s.substr(end);
			}
			setText(ENTRY_INPUT, q);
		}
	} else {
		switch (i) {
		case CLEAR:
			setText(ENTRY_INPUT, "");
			break;
		case RECOUNT:
			inputChanged();
			break;
		case COPY_TO_MEMORY:
			setText(ENTRY_MEMORY, getText(ENTRY_RESULT));
			break;
		case ADD_TO_BUFFER:
			s = getText(ENTRY_BUFFER);
			if (s.length() != 0) {
				s += "\n";
			}
			setText(ENTRY_BUFFER,
					s + getText(ENTRY_INPUT) + "=" + getText(ENTRY_RESULT));
			break;
		default:
			setText(ENTRY_BUFFER, "");
		}
	}

}

CalculatorWindow::CalculatorWindow() {
	const int maxw = 85;
	pWindow = this;

	int i, j, k, l, m;
	GtkWidget *grid, *g, *b, *box, *mbox, *box1;
	GtkWidget *comboBox;
	GtkCellRenderer *renderer;
	gchar c[] = "a=";

	//load config
	m_language = 0;
	{
		MapStringString m;
		MapStringString::iterator it;
		if (loadConfig(m)) {
			if ((it = m.find("language")) != m.end()) {
				if (parseString(it->second, j) && j >= 0 && j < SIZEI(LNG)) {
					m_language = j;
				}
			}
		}
	}

	for (auto &f : m_frame) {
		f = gtk_frame_new("");
		gtk_frame_set_label_align(GTK_FRAME(f), 0.06, 0.5);
	}

	for (auto &b : m_button) {
		b = gtk_button_new_with_label("");
		g_signal_connect(G_OBJECT(b), "clicked", G_CALLBACK(button_clicked), 0);
	}

	m_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(m_window), GTK_WIN_POS_CENTER);

	b = m_aboutButton = gtk_button_new();
	gtk_button_set_image(GTK_BUTTON(b), image("help.png"));
	g_signal_connect(G_OBJECT(b), "clicked", G_CALLBACK(button_clicked), 0);

	for (i = 0; i < SIZEI(m_label); i++) {
		m_label[i] = gtk_label_new("");
	}

	for (i = 0; i < SIZEI(m_entry); i++) {
		if (ENTRY_ENUM_ARRAY[i] == ENTRY_BUFFER) {
			m_entry[i] = gtk_text_view_new();
			gtk_text_view_set_editable(GTK_TEXT_VIEW(m_entry[i]), FALSE);
		} else {
			m_entry[i] = gtk_entry_new();
			if (ENTRY_ENUM_ARRAY[i] == ENTRY_RESULT
					|| ENTRY_ENUM_ARRAY[i] == ENTRY_MEMORY
					|| ENTRY_ENUM_ARRAY[i] == ENTRY_BUFFER) {
				gtk_editable_set_editable(GTK_EDITABLE(m_entry[i]), FALSE);
			} else if (ENTRY_ENUM_ARRAY[i] >= ENTRY_A) {
				gtk_entry_set_text(GTK_ENTRY(m_entry[i]), "0");
			}
			if (ENTRY_ENUM_ARRAY[i] == ENTRY_INPUT
					|| ENTRY_ENUM_ARRAY[i] >= ENTRY_A) {
				g_signal_connect(m_entry[i], "changed",
						G_CALLBACK(input_changed), 0);
			}
		}

	}

	for (auto &a : lexer) {
		i += a.size();
	}
	m_functionButton.resize(i);

	gtk_container_set_border_width(GTK_CONTAINER(m_window), 10);

	mbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);

	//1st line
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_box_pack_start(GTK_BOX(box), m_label[0], FALSE, FALSE, 0);
	gtk_widget_set_size_request(m_label[0], maxw, 0);

	gtk_box_pack_start(GTK_BOX(box), m_entry[ENTRY_INPUT], TRUE, TRUE, 0);

	for (i = 0; i < M_BUTTONS_LINE1; i++) {
		gtk_box_pack_start(GTK_BOX(box), m_button[i], FALSE, FALSE, 0);
	}
	gtk_box_pack_start(GTK_BOX(mbox), box, FALSE, FALSE, 0);

	//2nd line (additional variables)
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	for (i = 0; i < 4; i++) {
		c[0] = 'a' + i;
		g = gtk_label_new(c);
		gtk_box_pack_start(GTK_BOX(box), g, FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(box), m_entry[i + 4], TRUE, TRUE, 0);
	}
	gtk_box_pack_start(GTK_BOX(mbox), box, FALSE, FALSE, 0);

	//3rd line
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_box_pack_start(GTK_BOX(box), m_label[1], FALSE, FALSE, 0);
	gtk_widget_set_size_request(m_label[1], maxw, 0);
	gtk_box_pack_start(GTK_BOX(box), m_entry[ENTRY_RESULT], TRUE, TRUE, 0);
	for (i = 0; i < M_BUTTONS_LINE3; i++) {
		gtk_box_pack_start(GTK_BOX(box), m_button[i + M_BUTTONS_LINE1], FALSE,
		FALSE, 0);
	}
	gtk_box_pack_start(GTK_BOX(mbox), box, FALSE, FALSE, 0);

	//4th line (function buttons)
	grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), 3);

	for (k = i = 0; i < SIZEI(m_frame); i++) {
		l = i == 4 ? 5 : i;
		addItemToTable(grid, m_frame[i], l % 2, l / 2);

		g = gtk_grid_new();
		gtk_grid_set_row_spacing(GTK_GRID(g), 3);
		gtk_grid_set_column_spacing(GTK_GRID(g), 3);

		l = lexer[i].size();
		for (j = 0; j < l; j++) {
			b = m_functionButton[k++] = gtk_button_new_with_label(
					lexer[i][j].c_str());
			g_signal_connect(G_OBJECT(b), "clicked", G_CALLBACK(button_clicked),
					0);
			gtk_widget_set_hexpand(b, TRUE);

			m = l == 4 ? l : l / 2;
			addItemToTable(g, b, j % m, j / m);

			if (l != 12) {
				gtk_widget_set_size_request(b, maxw, 0);
			}
		}
		gtk_container_add(GTK_CONTAINER(m_frame[i]), g);
	}

	//memory
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_box_pack_start(GTK_BOX(box), m_label[2], FALSE, FALSE, 0);

	//gtk_box_pack_start (GTK_BOX(box), m_entry[ENTRY_MEMORY], TRUE, TRUE, 0);
	//avoid vertical stretch of m_entry[ENTRY_MEMORY]
	box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(box1), m_entry[ENTRY_MEMORY], TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), box1, TRUE, TRUE, 0);

	gtk_widget_set_size_request(m_label[2], maxw, 0);
	addItemToTable(grid, box, 0, 2);

	gtk_box_pack_start(GTK_BOX(mbox), grid, FALSE, FALSE, 0);

	//6th line (buffer)
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);

	box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);

	comboBox = gtk_combo_box_new_with_model(createModel());
	gtk_combo_box_set_active(GTK_COMBO_BOX(comboBox), m_language);
	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(comboBox), renderer, FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(comboBox), renderer,
			"pixbuf", PIXBUF_COL, NULL);
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(comboBox), renderer, FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(comboBox), renderer, "text",
			TEXT_COL, NULL);

	g_signal_connect(comboBox, "changed", G_CALLBACK(combo_changed), 0);

	gtk_box_pack_start(GTK_BOX(box1), m_aboutButton, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box1), m_label[3], TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box1), comboBox, FALSE, FALSE, 0);
	gtk_widget_set_size_request(box1, maxw, 0);

	gtk_box_pack_start(GTK_BOX(box), box1, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), m_entry[ENTRY_BUFFER], TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(mbox), box, TRUE, TRUE, 0);

	updateLanguage();
	loadCSS();

	g_signal_connect_swapped(G_OBJECT(m_window), "destroy",
			G_CALLBACK(gtk_main_quit), G_OBJECT(m_window));

	gtk_container_add(GTK_CONTAINER(m_window), mbox);
	gtk_widget_show_all(m_window);

	//set dot as decimal separator, standard locale, for output
	setlocale(LC_NUMERIC, "C");

	gtk_main();
}

GtkTreeModel* CalculatorWindow::createModel() {
	GdkPixbuf *pb;
	GtkTreeIter iter;
	GtkTreeStore *store;
	guint i;

	store = gtk_tree_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
	for (i = 0; i < SIZE(LNG); i++) {
		pb = pixbuf((LNG[i] + ".gif").c_str());
		gtk_tree_store_append(store, &iter, NULL);
		gtk_tree_store_set(store, &iter, PIXBUF_COL, pb, TEXT_COL,
				(" " + LNG[i]).c_str(), -1);
		g_object_unref(pb);
	}
	return GTK_TREE_MODEL(store);
}

void CalculatorWindow::addItemToTable(GtkWidget *grid, GtkWidget *w, int column,
		int row) {
	gtk_grid_attach(GTK_GRID(grid), w, column, row, 1, 1);
}

CalculatorWindow::~CalculatorWindow() {
	WRITE_CONFIG(CONFIG_TAGS, ExpressionEstimator::version, m_language);
}

void CalculatorWindow::updateLanguage() {
	unsigned i;
	gtk_window_set_title(GTK_WINDOW(m_window),
			getLanguageString(SCIENTIFIC_CALCULATOR));

	i = 0;
	for (auto f : m_frame) {
		gtk_frame_set_label(GTK_FRAME(f), getLanguageString(GROUP_TITLE[i++]));
	}

	i = 0;
	for (auto b : m_button) {
		gtk_button_set_label(GTK_BUTTON(b),
				getLanguageString(STRING_ENUM(i++)));
	}

	for (i = 0; i < SIZE(m_label); i++) {
		gtk_label_set_text(GTK_LABEL(m_label[i]), getLanguageString(LABEL[i]));
	}
	inputChanged();	//on error text need to redraw
}

unsigned CalculatorWindow::countArguments(const char *function) {
	if (strchr(function, '(') == NULL) {
		return 0;
	} else {
		if (strchr(function, ')') == NULL) {
			g_print("error %s %d\n", __FILE__, __LINE__);
		}
		return strchr(function, ',') == NULL ? 1 : 2;
	}
}

void CalculatorWindow::aboutDialog() {
	unsigned i, j;
	std::string s;
	GtkWidget *box, *hbox, *dialog, *label;
	char *markup;
	const char *p;

	dialog = gtk_dialog_new();
	gtk_window_set_title(GTK_WINDOW(dialog),
			getLanguageString(SCIENTIFIC_CALCULATOR));

	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_container_add(GTK_CONTAINER(hbox), image("calculator.png"));

	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	STRING_ENUM sid[] = { PROGRAM_VERSION, AUTHOR, COPYRIGHT, HOMEPAGE_STRING,
			CLEAR,/* build info (just use id)*/
			RECOUNT, /* file size (just use id)*/
	};
	STRING_ENUM id;
	for (i = 0; i < G_N_ELEMENTS(sid); i++) {
		id = sid[i];
		if (id == PROGRAM_VERSION) {
			s = format(getLanguageString(id),
					getLanguageString(SCIENTIFIC_CALCULATOR),
					ExpressionEstimator::version);
		} else if (id == CLEAR) {
			s = getBuildVersionString(true);
		} else if (id == RECOUNT) {
			s = "executable file size "
					+ toString(getApplicationFileSize(), ',');
		} else {
			s = getLanguageString(id);

			if (id == AUTHOR) {
				s += std::string(" email ") + MAIL;
			} else if (id == COPYRIGHT) {
				j = s.find('(');
				if (j != std::string::npos) {
					s = s.substr(0, j + 1) + "\u00A9" + s.substr(j + 2);
				}
			}
		}

		if (id == HOMEPAGE_STRING) {
			p = HOMEPAGE;
			label = gtk_label_new("");
			markup = g_markup_printf_escaped("%s <a href=\"%s,%s\">\%s,%s</a>",
					getLanguageString(id), p, LANGUAGE[m_language].c_str(), p,
					LANGUAGE[m_language].c_str());
			gtk_label_set_markup(GTK_LABEL(label), markup);
			g_free(markup);
			g_signal_connect(label, "activate-link", G_CALLBACK(label_clicked),
					gpointer(p));
		} else {
#ifndef NDEBUG
			//output that NDEBUG is not defined
			if (i == 0) {
				s += " DEBUG VERSION";
			}
#endif
			label = gtk_label_new(s.c_str());
		}

		gtk_widget_set_halign(label, GTK_ALIGN_START);
		//addClass(label,"aboutlabel");

		gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);
		//gtk_container_add(GTK_CONTAINER (box), label);
	}

	gtk_container_add(GTK_CONTAINER(hbox), box);
	gtk_container_add(
			GTK_CONTAINER(gtk_dialog_get_content_area (GTK_DIALOG(dialog))),
			hbox);

	gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(m_window));

	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

}
