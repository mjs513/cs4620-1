/*
 * MainWindow.h
 *
 *  Created on: Sep 1, 2010
 *      Author: Ivaylo Boyadzhiev
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QtGui/QMainWindow>

class QAction;
class QMenu;
class GLWidget;
class QTimer;
class QLabel;

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

class MainWindow : public QMainWindow
{
     Q_OBJECT

public:
     MainWindow(QWidget* parent = 0);
     ~MainWindow();

     void setFixedSize(bool fixed = true);
     bool init();

private slots:
      void exitApp();

private:
      void createActions();
      void createMenus();

	  QMenu* fileMenu;
	  QAction* exitAct;

	  GLWidget* glWidget;
};

#endif /* MAINWINDOW_H_ */
