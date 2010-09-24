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
#include "RectSplitter.h"
#include "Color.h"

#include <vector>
#include <cstdlib>

GLWidget::GLWidget(QWidget *parent)
	: QGLWidget(parent), cameraPos(0,0,10), cameraForward(0,0,-1), cameraUp(0,1,0), cameraLeft(Vector::cross(cameraUp,cameraForward))
{
	boxRotAngle = 0;
	frameExporter = 0;
	enableUserControl = true;
	isRecording = false;

	// Create animation timer
	animationTimer = new QTimer(this);
	connect(animationTimer, SIGNAL(timeout()), this, SLOT(animate()));
	animationTimer->start(ANIMATION_TIME_MS);
}

GLWidget::~GLWidget() {
	glDeleteLists(box, 1);
	glDeleteLists(axes, 1);
	glDeleteTextures(1, &boxTexture);

	delete animationTimer;
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	loadTextures(); // Load textures

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	// Depth buffer setup
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Set initial position for the camera
	cameraFreeMove(Vector(2,2,0));
	cameraRotateY(-20.0f);

// ****** Start of example code ******
	buildBox();
	buildAxesLines();
// ****** End of example code ******

	setRecording(false);
}

// This routine will be called 25 times per second, you
// can tweak this parameter by changing ANIMATION_TIME_MS
void GLWidget::animate() {

	// TODO: Update your animation here

// ****** Start of example code ******
	boxRotAngle += BOX_ROTATION_SPEED;
	if (boxRotAngle > 360) {
		boxRotAngle = (int)boxRotAngle % 360;
	}
// ****** End of example code ******

	updateGL();
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Position the camera
	OpenGL::lookAt(cameraPos,cameraForward,cameraUp);

// ****** Start of example code ******
	glCallList(axes); // Show X, Y, Z axes

	/*
	// Transform the box
	glScalef(.5f, .5f, .5f);
	glTranslatef(3.0f, 0.0f, 2.0f);
	glRotatef(boxRotAngle, 0.0f, 1.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, boxTexture);
	glCallList(box); // Show the box
	glBindTexture(GL_TEXTURE_2D, 0);*/
// ****** End of example code ******
	
	world.display();

	// TODO: Put your drawing here



	//glFlush(); // Send the commands to the OpenGL state machine
	//this->swapBuffers(); // Display the off-screen rendered buffer

	if (isRecording) { // Save frame to disk, if recording is enabled
		frameExporter->writeFrame(grabFrameBuffer());
	}
}

void GLWidget::buildAxesLines() {
	axes = glGenLists(1);

	glNewList(axes, GL_COMPILE);

	glPushMatrix();

	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(3.0, 0.0, 0.0);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 3.0, 0.0);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 3.0);
	glEnd();

	glPopMatrix();

	glEndList();
}

void GLWidget::buildBox() {
	box = glGenLists(1);

	glNewList(box,GL_COMPILE);

	glPushMatrix();
	glBegin(GL_QUADS); // Start drawing QUADS

	glColor3f(1.0f, 1.0f, 1.0f);

	// Bottom face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

	// Front Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

	// Back Face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

	// Right face
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

	// Left Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

	// Top Face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

	glEnd();
	glPopMatrix();
	glEndList();
}

void GLWidget::resizeGL(int width, int height)
{
	 if (height == 0) {
		 height = 1;
	 }

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 10, 1000);

	glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent*) {
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	static QPoint lastPos = event->pos();

	if (enableUserControl && event->buttons() & Qt::LeftButton) {
		int dx = event->x() - lastPos.x();
		int dy = event->y() - lastPos.y();

		if (dy != 0) {
			cameraRotateX((float)dy * MOUSE_SENSITIVITY_FACTOR);
		}

		if (dx != 0) {
			cameraRotateY(-(float)dx * MOUSE_SENSITIVITY_FACTOR);
		}

		update(); // update the screen
	}

	lastPos = event->pos();
}

void GLWidget::keyPressEvent(QKeyEvent * event) {

switch (event->key())
{
	case Qt::Key_W: cameraFreeMove(Vector(0,0,1)); break;
	case Qt::Key_S: cameraFreeMove(Vector(0,0,-1)); break;
	case Qt::Key_A: cameraFreeMove(Vector(1,0,0)); break;
	case Qt::Key_D: cameraFreeMove(Vector(-1,0,0)); break;
	case Qt::Key_Q: cameraFreeMove(Vector(0,1,0)); break;
	case Qt::Key_Z: cameraFreeMove(Vector(0,-1,0)); break;
	
	case Qt::Key_R: setRecording(!isRecording); break;
	//case Qt::Key_C: enableUserControl = !enableUserControl; break;
}

update(); // update the screen
}

void GLWidget::cameraRotateX(double theta) {
	// X' = X

	// Z' = -sin(theta) * Y + cos(theta) * Z
	double angle = (theta * M_PI) / 180.0;
	Vector u =  cameraUp * -sin(angle);
	Vector v =  cameraForward * cos(angle);

	cameraForward = (u + v);
	cameraForward.normalize();

	// Y' = Z' ^ X'
	cameraLeft = Vector::cross(cameraUp,cameraForward);
	cameraLeft.normalize();
}

void GLWidget::cameraRotateY(double theta) {
	// Y` = Y

	// Z` = sin(theta) + cos(theta) * Z
	double angle = (theta * M_PI) / 180.0;
	Vector u =  cameraLeft * sin(angle);
	Vector v =  cameraForward * cos(angle);

	cameraForward = (u + v);
	cameraForward.normalize();

	// Y' = Z' ^ X'
	cameraLeft = Vector::cross(cameraUp,cameraForward);
	cameraLeft.normalize();
}

void GLWidget::cameraFreeMove(Vector dir) {
	dir *= CAMERA_MOVING_STEP;

	if(enableUserControl) {
		cameraPos += dir.x*cameraLeft + dir.y*cameraUp + dir.z*cameraForward;
	}
}

void GLWidget::loadTextures() {
	QImage box_image;

// ****** Start of example code ******
	if (!box_image.load("box_texture.bmp")) {
		box_image = QImage(16, 16, QImage::Format_RGB32);
		box_image.fill(Qt::red);
	}

	box_image = QGLWidget::convertToGLFormat(box_image);
	glGenTextures(1, &boxTexture);
	glBindTexture(GL_TEXTURE_2D, boxTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, box_image.width(), box_image.height(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, box_image.bits());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
// ****** End of example code ******

}

void GLWidget::setRecording(bool state) {
	isRecording = state;

	if (isRecording) {
		delete frameExporter;
		frameExporter = new FrameExporter();
		if (!frameExporter || !frameExporter->init()) {
			printf("Unable initialize frame capture object");
			isRecording = false;
		} else {
			((MainWindow*)parent())->statusBar()->showMessage("Recording...");
			((MainWindow*)parent())->setFixedSize(true);
		}
	} else {
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
