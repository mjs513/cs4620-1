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

#include "Ray.h"
#include "GLWidget.h"
#include "MainWindow.h"
#include "OpenGL.h"
#include "Color.h"
#include "IKSolver.h"
#include "JointTree.h"
#include "HumanHand.h"
#include "PlanarChain.h"
#include "WalkingBug.h"

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
	: QGLWidget(parent), _camera(20), _character(0), _planarChain(0), _humanHand(0), _walkingBug(0), _animationEnabled(false),
	  _selectedJoint(0), _enableUserControl(true), _isRecording(false), _frameExporter(0)
{
	// Create update timer
	_animationTimer = new QTimer(this);
	connect(_animationTimer , SIGNAL(timeout()), this, SLOT(animate()));
	_animationTimer->start();
}

GLWidget::~GLWidget()
{
	delete _planarChain;
	delete _humanHand;
	delete _walkingBug;
	
	delete _animationTimer;
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

std::map<Joint*,Point>& GLWidget::endEffectorsTarget()
{
	return _endEffectorsTarget;
}

bool GLWidget::animationEnabled()
{
	return _animationEnabled;
}

void GLWidget::initializeGL()
{
	Color clearColor(0,0,0,0);
	
	OpenGL::clearColor(clearColor);
	
	glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);

	// Depth buffer setup
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_ALPHA_TEST);
	
	//glEnable(GL_CULL_FACE);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	// Initialize lighting
    glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
    
    glEnable(GL_LIGHT0);
    
    OpenGL::lightColor(GL_LIGHT0,GL_AMBIENT,Color(0.2,0.2,0.2));
    OpenGL::lightColor(GL_LIGHT0,GL_DIFFUSE,Color(1,1,1));
    OpenGL::lightColor(GL_LIGHT0,GL_SPECULAR,Color(1,1,1));
    
	setRecording(false);
	
	glPointSize(10);
	glLineWidth(3);
	
	_planarChain = new PlanarChain();
	_humanHand = new HumanHand();
	_walkingBug = new WalkingBug();
	
	_character = _planarChain;
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45,double(_width)/_height,1,100);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Position the camera
	_camera.applyTransformation();

	//glMultMatrixd((_cameraSelectedRotationMatrix*_cameraRotationMatrix).v);

	// Save the matrices for ray picking
	glGetDoublev(GL_MODELVIEW_MATRIX,_modelviewMatrix.v);
	glGetDoublev(GL_PROJECTION_MATRIX,_projectionMatrix.v);
	
	// Place the light
    OpenGL::lightPosition(GL_LIGHT0,Vector(1,1,1));
    
    _character->root()->display();

    // Draw selected joint as a cyan colored sphere
    GLUquadric *q = gluNewQuadric();

	for(std::map<Joint*,Point>::const_iterator i = _endEffectorsTarget.begin(); i != _endEffectorsTarget.end(); ++i) {
		// Draw the target position
		glPushMatrix(); {
			OpenGL::color(Color::yellow());
			
			OpenGL::translate(i->second);
			gluSphere(q, 0.15, 20, 20);
		}
		glPopMatrix();
		
		GLMatrix m = i->first->calculateGlobalTransformation();
		
		// Draw line from target position and current position
		glBegin(GL_LINES); {
			Color c = Color::yellow();
			
			OpenGL::color(c);
			OpenGL::vertex(i->second);
			
			c = Color::red();
			
			OpenGL::color(c);
			OpenGL::vertex(m*Point());
		}
		glEnd();
	}
	
	gluDeleteQuadric(q);
	
	// Draw floor plane
	OpenGL::color(Color(0.6,0.6,0.6,0.8));
	
	OpenGL::translate(Vector(0,0,-0.1));
	
	glBegin(GL_QUADS); {
		OpenGL::normal(Vector(0,0,1));

		OpenGL::vertex(Point(-100,-100));
		OpenGL::vertex(Point(100,-100));
		OpenGL::vertex(Point(100,100));
		OpenGL::vertex(Point(-100,100));
	}
	glEnd();

	static int frameCount = 0,fps = 0;
	static QTime time = QTime::currentTime();
	
	QTime now = QTime::currentTime();
	int elapsed = time.msecsTo(now);
	
	++frameCount;
	
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0,_width,0,_height,10,-10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	OpenGL::color(Color::white());
	
	char buffer[512];
	
	sprintf(buffer,"Move: W A S D / Right button");
	renderText(20, _height - 60, buffer);
	sprintf(buffer,"Zoom: Q Z / Middle button");
	renderText(20, _height - 40, buffer);
	sprintf(buffer,"Pick/drag: Left button");
	renderText(20, _height - 20, buffer);

	if( _animationEnabled == true ) {
		sprintf(buffer,"Forward Kinematics");
	}
	else {
		sprintf(buffer,"Inverse Kinematics");
	}
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

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if((event->buttons() & Qt::LeftButton) && !_animationEnabled) {
		const double PICKING_RANGE = 0.001;

		_mouseClick = Point(event->x(),event->y());
		
		Point unprojected = OpenGL::unproject(_mouseClick,_modelviewMatrix,_projectionMatrix);
		Ray pickingRay(_camera.eye(),unprojected);
		
		std::vector<Joint*> jointStack;
		std::vector<GLMatrix> matrixStack;
		double minDist = 1e300/1e-300;
		
		jointStack.push_back(_character->root());
		matrixStack.push_back(_character->root()->transformation());
		
		Point cameraEye = _camera.eye();
		Point selectedPoint;
		
		// Deselect current selection
		_selectedJoint = 0;
		
		// Recursive stack
		while(!jointStack.empty()) {
			Joint *j = jointStack.back();
			GLMatrix m = matrixStack.back();
	
			jointStack.pop_back();
			matrixStack.pop_back();
			
			for(std::vector<Joint*>::const_iterator i = j->children().begin(); i != j->children().end(); ++i) {
				jointStack.push_back(*i);
				matrixStack.push_back((*i)->transformation()*m);
			}
			
			// Test pick
			Point p = m*Point();
			double sqCamDist = (p - cameraEye).squaredLength();
			double d = pickingRay.distance(p);
			
			if((d < minDist) && (d*d/sqCamDist < PICKING_RANGE)) {
				minDist = d;
				_selectedJoint = j;
				selectedPoint = p;
			}
		}

		bool isTargetPoint = false;
		
		for(std::map<Joint*,Point>::iterator i = _endEffectorsTarget.begin(); i != _endEffectorsTarget.end(); ++i) {
			Point p = i->second;
			double sqCamDist = (p - cameraEye).squaredLength();
			double d = pickingRay.distance(p);
			
			if((d < minDist) && (d*d/sqCamDist < PICKING_RANGE)) {
				minDist = d;
				_selectedJoint = i->first;
				selectedPoint = p;
				isTargetPoint = true;
			}
		}
		
		// Create target point for joint
		if(!isTargetPoint) {
			_endEffectorsTarget[_selectedJoint] = selectedPoint;
		}
	}
	
	// Camera movements
	else if(event->buttons() & (Qt::RightButton | Qt::MiddleButton)) {
		_camera.mouseClicked(event);
	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	// Right and middle buttons control camera movements
	if( event->buttons() & (Qt::RightButton | Qt::MiddleButton) ) {
		_camera.mouseMoved(event);
	}
	// Left button control joints
	else if(event->buttons() & Qt::LeftButton) {
		// Move selected joint
		if(_selectedJoint) {
			std::map<Joint*,Point>::iterator selectedTarget = _endEffectorsTarget.find(_selectedJoint);
			Point newPos = targetPointFromMouse(Point(event->x(),event->y()),selectedTarget->second);
			
			selectedTarget->second = newPos;
		}
	}
}

void GLWidget::keyPressEvent(QKeyEvent * event)
{
	Character *oldCharacter = _character;
	
	switch (event->key()) {
		// Camera controls
		case Qt::Key_W: _camera.moveUp(); break;
		case Qt::Key_S: _camera.moveDown(); break;
		case Qt::Key_A: _camera.moveLeft(); break;
		case Qt::Key_D: _camera.moveRight(); break;
		case Qt::Key_Q: _camera.moveFront(); break;
		case Qt::Key_Z: _camera.moveBack(); break;
		
		// Toggle forward/inverse kinematics
		case Qt::Key_Space: setAnimationEnabled(!_animationEnabled); break;

		// Record
		case Qt::Key_R: setRecording(!_isRecording); break;
		
		//case Qt::Key_C: enableUserControl = !enableUserControl; break;

		// Change character views
		case Qt::Key_1: _character = _planarChain; _endEffectorsTarget.clear(); break;
		case Qt::Key_2: _character = _humanHand; _endEffectorsTarget.clear(); break;
		case Qt::Key_3: _character = _walkingBug; _endEffectorsTarget.clear(); break;
		case Qt::Key_4: break;
	}
	
	// Restart animation when switching characters with animation enabled
	if(_animationEnabled && (oldCharacter != _character)) {
		_character->startAnimation();
	}
	
	update(); // update the screen
}

void GLWidget::setRecording(bool state)
{
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

Point GLWidget::targetPointFromMouse(const Point &mouse, const Point &refPoint)
{
	GLint viewport[4];
	
	glGetIntegerv(GL_VIEWPORT,viewport);
	
	Point eye = _camera.eye();
	
	// Ray from camera to new mouse position
	Ray ray(eye,OpenGL::unproject(mouse,_modelviewMatrix,_projectionMatrix));
	
	// Plane normal to camera view that contains selected point
	Vector normal = eye - OpenGL::unproject(Point(0.5*viewport[2],0.5*viewport[2]),_modelviewMatrix,_projectionMatrix);
	double planeD = -Vector::dot(normal,Vector(refPoint));
	
	// Intersect ray with plane
	double t = -(planeD + Vector::dot(normal,Vector(ray.p)))/Vector::dot(normal,ray.dir);
	
	return ray.pointAt(t);
}

void GLWidget::setAnimationEnabled(bool enable)
{
	_animationEnabled = enable;
	
	if(_animationEnabled) {
		_endEffectorsTarget.clear();
		
		_character->startAnimation();
	}
}

void GLWidget::animate()
{
	if(_animationEnabled) {
		_character->update(*this);
	}
	
	if(!_endEffectorsTarget.empty()) {
		IKSolver solver = IKSolver(JointTree(_character->root()),1);
		
		for(int i = 0; i < 10; ++i) {
			solver.solve(_endEffectorsTarget);
		}
	}
	
	updateGL();
}

int GLWidget::FrameExporter::exporterId = -1;

bool GLWidget::FrameExporter::init()
{
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
