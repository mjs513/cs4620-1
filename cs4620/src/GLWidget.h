/*
 * glwidget.h
 *
 *  Created on: Sep 1, 2010
 *      Author: Ivaylo Boyadzhiev
 */

#ifndef GLWIDGET_H_
#define GLWIDGET_H_

#include <QtOpenGL/QGLWidget>
#include "Vector.h"
#include "Point.h"
#include "World.h"

#define CAMERA_MOVING_STEP 0.5
#define MOUSE_SENSITIVITY_FACTOR 0.25

#define BASE_CAP_FRAMES_DIR "frames"
#define BASE_CAP_FRAMES_NAME "exporter"

#define BOX_ROTATION_SPEED 1 // in degrees per frame

// How often (in ms) the animation routine will be called
#define ANIMATION_TIME_MS 40 // Update our scene 25 times per second

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

private:
	class FrameExporter {
		public:
			FrameExporter() : nFrames(0) { exporterId++; }

			bool init();
			void writeFrame(const QImage& frame);

		private:
			static int exporterId;
			int nFrames;
	};

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent* event);

protected slots:
	void animate();

private:
	void buildAxesLines();
	void buildBox();

	void cameraRotateX(double angle);
	void cameraRotateY(double angle);

	void cameraFreeMove(Vector dir);
	void cameraGameMove(double dx, double dy, double dz);

	void loadTextures();
	void setRecording(bool state);

private:
	// Camera position up/left/right vectors
	Point cameraPos;
	Vector cameraForward, cameraUp, cameraLeft;

	GLuint	box;
	GLuint axes;
	double boxRotAngle; // Box rotation angle (used for animation)
	GLuint boxTexture;

	QTimer* animationTimer; // Used to call our animation routine
	QTime* fpsTime; // Measure time interval between 2 fully drawn frames
						  // we will use this to create frame-independent animation

	// Whether we can use mouse/keyboard to move the camera
	bool enableUserControl;

	bool isRecording;
	FrameExporter* frameExporter;
	
	World world;
};

#endif /* GLWIDGET_H_ */
