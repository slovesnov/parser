/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov 
 homepage http://slovesnov.users.sourceforge.net/?parser
 email slovesnov@yandex.ru
 All rights reserved.
 ******************************************************/

/*
 * TYPE 1 - calculator (under GTK3)
 * else - testFormWindow (under GTK3)
 */
#define TYPE 1

#if TYPE==1
#include "CalculatorWindow.h"

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);
	aslovInit(argv);
	CalculatorWindow c;
}

#else
#include "TestFormWindow.h"

int main(int argc, char *argv[]) {
	gtk_init (&argc, &argv);
	aslovInit(argv);
	TestFormWindow c;
}
#endif
