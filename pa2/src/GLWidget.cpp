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
	: QGLWidget(parent), _camera(), _rootJoint(0), _selectedJoint(0), _enableUserControl(true), _isRecording(false), _frameExporter(0)
{
	_animationTimer = new QTimer(this);
	connect(_animationTimer , SIGNAL(timeout()), this, SLOT(animate()));
	_animationTimer->start(1000/35);
}

GLWidget::~GLWidget()
{
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
	
	_camera.setRadius(20);
	
	glPointSize(10);
	glLineWidth(3);
	
	Vector z = Vector(0,0,1);
	
	_rootJoint = new Joint(Point(1,0),z);
	Joint *j1 = new Joint(Point(1,0),z);
	Joint *j2 = new Joint(Point(1,0),z);
	Joint *j3 = new Joint(Point(1,0),z);
	Joint *j4 = new Joint(Point(1,1),z);
	
	_rootJoint->addChild(j1);
	j1->addChild(j2);
	j2->addChild(j3);
	j1->addChild(j4);
	
	
	//_rootJoint = new Joint(0,0);
	/*
	// Create planar chain
	_planarJoint = new Joint(0, 0);
	Joint *j1 = new Joint(1,45);
	Joint *j2 = new Joint(1,30);
	Joint *j3 = new Joint(1,-30);
	Joint *j4 = new Joint(1,-90);

	_planarJoint->addChild(j1);
	j1->addChild(j2);
	j2->addChild(j3);
	j3->addChild(j4);
	*/


	// Create humanoid

	// Body
	/*
	_humanJoint = new Joint(0, 0);
	Joint *head = new Joint(0.5,90);
	Joint *leftShoulder = new Joint(1,180);
	Joint *leftHand = new Joint(1,90);
	Joint *rightShoulder = new Joint(1,0);
	Joint *rightHand = new Joint(1,-90);
	Joint *hip = new Joint(1,-90);
	Joint *rightLeg = new Joint(1,45);
	Joint *rightFoot = new Joint(1,-45);
	Joint *leftLeg = new Joint(1,-45);
	Joint *leftFoot = new Joint(1,45);

	_humanJoint->addChild(leftShoulder);
	_humanJoint->addChild(rightShoulder);
	_humanJoint->addChild(head);
	_humanJoint->addChild(hip);

	leftShoulder->addChild(leftHand);

	rightShoulder->addChild(rightHand);

	hip->addChild(rightLeg);
	hip->addChild(leftLeg);

	rightLeg->addChild(rightFoot);
	
	leftLeg->addChild(leftFoot);

	_rootJoint = _humanJoint;

	_rootJoint = new Joint(0,0);
	Joint *j1 = new Joint(1,0);
	Joint *j2 = new Joint(1,90);
	Joint *j3 = new Joint(1,90);

	_rootJoint->addChild(j1);
	_rootJoint->addChild(j2);
	j1->addChild(j3);*/
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
	
	// Save the matrices for ray picking
	glGetDoublev(GL_MODELVIEW_MATRIX,modelviewMatrix.v);
	glGetDoublev(GL_PROJECTION_MATRIX,projectionMatrix.v);
	
	// Place the light
    OpenGL::lightPosition(GL_LIGHT0,Vector(1,1,1));

    glBegin(GL_LINES); {
    	OpenGL::color(Color::red());
    	OpenGL::vertex(Point(0,0,0));
    	OpenGL::vertex(Point(1,0,0));

    	OpenGL::color(Color::green());
    	OpenGL::vertex(Point(0,0,0));
    	OpenGL::vertex(Point(0,1,0));

    	OpenGL::color(Color::blue());
    	OpenGL::vertex(Point(0,0,0));
    	OpenGL::vertex(Point(0,0,1));
    }
    glEnd();
    
    _rootJoint->display();

	if(_selectedJoint) {
		OpenGL::color(Color::cyan());
		
		GLMatrix m = _selectedJoint->calculateGlobalTransformation();
		
		glBegin(GL_POINTS); {
			OpenGL::vertex(m*Point());
		}
		glEnd();
	}
	
	OpenGL::color(Color::yellow());
	
	for(std::map<Joint*,Point>::const_iterator i = _endEffectorsMotion.begin(); i != _endEffectorsMotion.end(); ++i) {
		glBegin(GL_POINTS); {
			OpenGL::vertex(i->second);
		}
		glEnd();
	}

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

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->buttons() & Qt::LeftButton) {
		const double PICKING_RANGE = 0.001;
		
		Point mouse(event->x(),event->y());
		Point unprojected = OpenGL::unproject(mouse,modelviewMatrix,projectionMatrix);
		Ray pickingRay(_camera.eye(),unprojected);
		
		std::vector<Joint*> jointStack;
		std::vector<GLMatrix> matrixStack;
		double minDist = 1e300/1e-300;
		
		jointStack.push_back(_rootJoint);
		matrixStack.push_back(_rootJoint->transformation());
		
		Point cameraEye = _camera.eye();
		
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
			
			// Only allow picking end effects
			if(j->hasEndEffector()) {
				Point p = m*Point();
				double sqCamDist = (p - cameraEye).squaredLength();
				double d = pickingRay.distance(p);
				
				if((d < minDist) && (d*d/sqCamDist < PICKING_RANGE)) {
					minDist = d;
					_selectedJoint = j;
				}
			}
		}
	}
	else if(!event->buttons()){
		_endEffectorsMotion.clear();
	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	// Move selected joint
	if(_selectedJoint && (event->buttons() & Qt::LeftButton)) {
		GLint viewport[4];
		
		glGetIntegerv(GL_VIEWPORT,viewport);
		
		Point eye = _camera.eye();
		GLMatrix selectedTransformation = _selectedJoint->calculateGlobalTransformation();
		Point selected = selectedTransformation*Point();
		
		// Ray from camera to new mouse position
		Ray ray(eye,OpenGL::unproject(Point(event->x(),event->y()),modelviewMatrix,projectionMatrix));
		
		// Plane normal to camera view that contains selected point
		Vector normal = eye - OpenGL::unproject(Point(0.5*viewport[2],0.5*viewport[2]),modelviewMatrix,projectionMatrix);
		double planeD = -Vector::dot(normal,Vector(selected));
		
		// Intersect ray with plane
		double t = -(planeD + Vector::dot(normal,Vector(ray.p)))/Vector::dot(normal,ray.dir);
		Point newPos = ray.pointAt(t);
		
		_endEffectorsMotion[_selectedJoint] = newPos;
	}
}

void GLWidget::keyPressEvent(QKeyEvent * event)
{
	switch (event->key()) {
		// Camera controls
		case Qt::Key_W: _camera.moveUp(); break;
		case Qt::Key_S: _camera.moveDown(); break;
		case Qt::Key_A: _camera.moveLeft(); break;
		case Qt::Key_D: _camera.moveRight(); break;
		case Qt::Key_Q: _camera.moveFront(); break;
		case Qt::Key_Z: _camera.moveBack(); break;
		
		// Record
		case Qt::Key_R: setRecording(!_isRecording); break;
		
		//case Qt::Key_C: enableUserControl = !enableUserControl; break;

		// Change character views
		case Qt::Key_1: _rootJoint = _planarJoint; break;
		case Qt::Key_2: _rootJoint = _humanJoint; break;
		case Qt::Key_3: break;
		case Qt::Key_4: break;
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

void GLWidget::animate()
{
	if(!_endEffectorsMotion.empty()) {
		IKSolver solver = IKSolver(JointTree(_rootJoint),1);
		
		for(int i = 0; i < 10; ++i) {
			solver.solve(_endEffectorsMotion);
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
