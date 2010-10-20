/*
 * glwidget.cpp
 *
 *  Created on: Sep 1, 2010
 *      Author: Ivaylo Boyadzhiev
 */
#include <QtGui>
#include <QtOpenGL/QtOpenGL>
#include <QTimer>

#include <math.h>
#include <unistd.h>

#include "GLWidget.h"
#include "MainWindow.h"
#include "OpenGL.h"
#include "Color.h"

#include <vector>
#include <cstdlib>
#include <iostream>


namespace {


const double CAMERA_MOVING_STEP = 1;
const double MOUSE_SENSITIVITY_FACTOR = 0.25;

const char *BASE_CAP_FRAMES_DIR = "frames";
const char *BASE_CAP_FRAMES_NAME = "exporter";

const double HIGH_VISIBILITY = 1000;
const double LOW_VISIBILITY = 400;


}  // namespace


GLWidget::GLWidget(QWidget *parent)
	: QGLWidget(parent), _camera(), _enableUserControl(true), _isRecording(false), _frameExporter(0) { }

GLWidget::~GLWidget()
{
	delete _frameExporter;
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(640, 480);
}

void GLWidget::initializeGL()
{
	Color clearColor(0.1,0.1,0.11);
	
	OpenGL::clearColor(clearColor);
	
	glEnable(GL_TEXTURE_2D);

	// Depth buffer setup
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_ALPHA_TEST);
	
	glEnable(GL_CULL_FACE);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	// Initialize lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
    
    glEnable(GL_LIGHT0);
    
    OpenGL::lightColor(GL_LIGHT0,GL_AMBIENT,Color(0.2,0.2,0.2));
    OpenGL::lightColor(GL_LIGHT0,GL_DIFFUSE,Color(1,1,1));
    OpenGL::lightColor(GL_LIGHT0,GL_SPECULAR,Color(1,1,1));
    
	setRecording(false);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45,double(_width)/_height,1,1000);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Position the camera
	_camera.applyTransformation();
	
	// Place the light
    OpenGL::lightPosition(GL_LIGHT0,Vector(1,1,1));
    

    //
    // draw scene
    //
    
    
	static int frameCount = 0,fps = 0;
	static QTime time = QTime::currentTime();
	
	QTime now = QTime::currentTime();
	int elapsed = time.msecsTo(now);
	
	++frameCount;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0,_width,0,_height,10,-10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	OpenGL::color(Color::white());
	
	char buffer[512];
	
	sprintf(buffer,"FPS: %d (not counting idle frames)",fps);
	renderText(20,20,buffer);
	
	if(elapsed > 1000) {
		fps = frameCount;
		
		time = now;
		frameCount = 0;
	}
	
	//----- Commands below removed because of Windows flickering issues -----//
	
	//glFlush(); // Send the commands to the OpenGL state machine
	//this->swapBuffers(); // Display the off-screen rendered buffer

	if(_isRecording) { // Save frame to disk, if recording is enabled
		_frameExporter->writeFrame(grabFrameBuffer());
	}
}

void GLWidget::resizeGL(int w, int h)
{
	_width = w;
	_height = h;
	
	 if(_height == 0) {
		 _height = 1;
	 }

	glViewport(0,0,_width,_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent*) { }

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	//
	// Implement picking
	//
}

void GLWidget::keyPressEvent(QKeyEvent * event)
{
	switch (event->key()) {
		case Qt::Key_W: _camera.moveUp(); break;
		case Qt::Key_S: _camera.moveDown(); break;
		case Qt::Key_A: _camera.moveLeft(); break;
		case Qt::Key_D: _camera.moveRight(); break;
		
		case Qt::Key_R: setRecording(!_isRecording); break;
		
		//case Qt::Key_C: enableUserControl = !enableUserControl; break;
	}
	
	update(); // update the screen
}

void GLWidget::setRecording(bool state) {
	_isRecording = state;

	if (_isRecording) {
		delete _frameExporter;
		_frameExporter = new FrameExporter();
		if (!_frameExporter || !_frameExporter->init()) {
			printf("Unable initialize frame capture object");
			_isRecording = false;
		}
		else {
			((MainWindow*)parent())->statusBar()->showMessage("Recording...");
			((MainWindow*)parent())->setFixedSize(true);
		}
	}
	else {
		((MainWindow*)parent())->statusBar()->clearMessage();
		((MainWindow*)parent())->setFixedSize(false);
	}
}

int GLWidget::FrameExporter::exporterId = -1;
bool GLWidget::FrameExporter::init() {
	// create directory for captured frames
	QDir dir = QDir::currentPath();

	dir.mkdir(BASE_CAP_FRAMES_DIR);

	return true;
}

void GLWidget::FrameExporter::writeFrame(const QImage& frame) {
	QChar ch = '0';
	QString number = QString("%1").arg(nFrames, 5, 10, ch);
	QString file_name = QString("%1/%2%3-%4.png").arg(BASE_CAP_FRAMES_DIR).\
			arg(BASE_CAP_FRAMES_NAME).arg(exporterId).arg(number);

	if ( !frame.save(file_name, "PNG") ) {
		printf("Error writing frame to file %s\n", file_name.toStdString().c_str());
	} else {
	}

	nFrames++;
}
