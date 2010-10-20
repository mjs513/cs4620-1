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

private:
	void cameraRotateX(double angle);
	void cameraRotateY(double angle);

	void cameraFreeMove(Vector dir);
	void cameraGameMove(double dx, double dy, double dz);

	void setRecording(bool state);
	
	void setFogRange();

private:
	World *world;
	int width,height;
	
	// Camera position up/left/right vectors
	Point cameraPos;
	Vector cameraForward, cameraUp, cameraLeft;

	// Whether we can use mouse/keyboard to move the camera
	bool enableUserControl;

	bool isRecording;
	FrameExporter* frameExporter;
};

#endif /* GLWIDGET_H_ */
