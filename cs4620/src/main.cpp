#include <QtGui/QtGui>
#include <Qt/QApplication.h>
#include <cstdlib>
#include <iostream>
#include <cmath>

#include "MainWindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    
    if(!w.init()) {
    	printf("Error initializing main window\n");
    }

    w.show();
    
    return a.exec();
}
