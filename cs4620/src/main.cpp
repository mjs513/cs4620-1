#include <QtGui/QtGui>
#include <Qt/QApplication.h>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    if (not w.init()) {
    	printf("Error initializing main window\n");
    }

    w.show();
    return a.exec();
}
