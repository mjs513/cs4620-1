#include <QtGui/QtGui>
#include <Qt/QApplication.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>

#include "MainWindow.h"


int main(int argc, char *argv[])
{
	std::srand(std::time(0));
	
    QApplication a(argc, argv);
    MainWindow w;
	
    if(!w.init()) {
    	std::cout << "Error initializing main window" << std::endl;
    }
    
    w.show();
    
    return a.exec();
}
