/*
 * TestFormWindow.cpp
 *
 *  Created on: 16.04.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#include <regex>

#include "TestFormWindow.h"

static TestFormWindow *pWindow;
static void input_changed(GtkWidget *w, gpointer) {
	pWindow->inputChanged(w);
}

VString splitR(std::string const &s, std::string const &separator = "\\s+") {
	VString v;
	std::regex rgx(separator);
	std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
	std::sregex_token_iterator end;
	for (; iter != end; ++iter) {
		v.push_back(*iter);
	}
	return v;
}

TestFormWindow::TestFormWindow() {
	GtkWidget *box, *mbox;
	pWindow = this;
	int i;
	m_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(m_window), "test");
	gtk_window_set_position(GTK_WINDOW(m_window), GTK_WIN_POS_CENTER);

	std::string d[] = { "Sin(pI/4)", "x0+2*x1", "x0 x1", "1 2" };
	std::string caption[] = { "expression", "variables", "values" };
	i = 0;
	for (auto &a : m_entry) {
		a = gtk_entry_new();
		gtk_entry_set_text(GTK_ENTRY(a), d[i].c_str());
		g_signal_connect(a, "changed", G_CALLBACK(input_changed), 0);
		i++;
	}
	for (auto &a : m_label) {
		a = gtk_label_new("");
		gtk_widget_set_size_request(a, 250, -1);
	}

	mbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);

	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_box_pack_start(GTK_BOX(box), gtk_label_new(caption[0].c_str()), FALSE,
			FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), m_entry[0], TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), m_label[0], FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(mbox), box);

	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	for (i = 1; i < 4; i++) {
		gtk_box_pack_start(GTK_BOX(box), gtk_label_new(caption[i - 1].c_str()),
		FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(box), m_entry[i], 0, 0, 0);
	}
	gtk_box_pack_start(GTK_BOX(box), m_label[1], FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(mbox), box);

	//cann't use loadCSS because this project already has css file for calculator
	GtkCssProvider *provider;
	GdkDisplay *display;
	GdkScreen *screen;
	std::string s;

	provider = gtk_css_provider_new();
	display = gdk_display_get_default();
	screen = gdk_display_get_default_screen(display);
	gtk_style_context_add_provider_for_screen(screen,
			GTK_STYLE_PROVIDER(provider),
			GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	s = "entry,label{font-size:24px;}";
	gtk_css_provider_load_from_data(provider, s.c_str(), -1, NULL);
	g_object_unref(provider);

	g_signal_connect_swapped(G_OBJECT(m_window), "destroy",
			G_CALLBACK(gtk_main_quit), G_OBJECT(m_window));

	gtk_container_add(GTK_CONTAINER(m_window), mbox);
	gtk_widget_set_size_request(m_entry[1], 350, -1);
	gtk_widget_set_size_request(mbox, 1050, -1);
	gtk_widget_show_all(m_window);
	//set dot as decimal separator, standard locale, for output
	setNumericLocale();

	for (i = 0; i < 2; i++) {
		recount(i);
	}

	gtk_main();

}

TestFormWindow::~TestFormWindow() {
}

void TestFormWindow::inputChanged(GtkWidget *w) {
	recount(w != m_entry[0]);
}

void TestFormWindow::recount(int i) {
	std::string s = gtk_entry_get_text(GTK_ENTRY(m_entry[i])), s1;
	double r;
	VString v;
	std::vector<double> b;
	try {
		if (i) {
			s1 = gtk_entry_get_text(GTK_ENTRY(m_entry[2]));
			v = splitR(s1);
			s1 = gtk_entry_get_text(GTK_ENTRY(m_entry[3]));
			for (auto &a : splitR(s1)) {
				if (parseString(a, r)) {
					b.push_back(r);
				} else {
					throw std::runtime_error("can't parse \"" + a + "\"");
				}
			}
//			printl(s, joinV(v,","))
		}
		e.compile(s, v);
		r = e.calculate(b);
		s = std::to_string(r);
	} catch (std::exception &ex) {
		s = ex.what();
	}
	gtk_label_set_text(GTK_LABEL(m_label[i]), s.c_str());
}
