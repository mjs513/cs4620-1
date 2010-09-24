#include <QtGui>

#include "mainwindow.h"
#include "glwidget.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	this->exitAct = 0;
	this->glWidget = 0;
}

bool MainWindow::init() {
	glWidget = new GLWidget();
	setCentralWidget(glWidget);

	glWidget->grabKeyboard();
	glWidget->grabMouse();
	glWidget->setMouseTracking(true);

	createActions();
	createMenus();

	setWindowTitle(tr("Introduction to Computer Graphics"));
	setMinimumSize(100, 100);
	resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	return true;
}

MainWindow::~MainWindow() {
	delete exitAct;
	delete glWidget;
}

void MainWindow::createActions() {
	exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcut(tr("Ctrl+X"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(exitApp()));
}

void MainWindow::createMenus() {
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(exitAct);
}

void MainWindow::setFixedSize(bool fixed) {
	if (fixed) {
		QSize fixedSize(width(), height());
		setMinimumSize(fixedSize);
		setMaximumSize(fixedSize);
		setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	} else  {
		setMinimumSize(100, 100);
		setMaximumSize(2048, 1024);
		setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
	}
}

void MainWindow::exitApp() {
	this->close();
}
