/*
 * glwidget.h
 *
 *  Created on: Sep 1, 2010
 *      Author: Ivaylo Boyadzhiev
 */

#ifndef GLWIDGET_H_
#define GLWIDGET_H_

#include <QtOpenGL/QGLWidget>
#include <map>

#include "Vector.h"
#include "Point.h"
#include "SphereCamera.h"
#include "Joint.h"
#include "Ray.h"


class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

private:
	class FrameExporter
	{
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
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent* event);

private:
	void setRecording(bool state);
	
private slots:
	void animate();
	
private:
	int _width,_height;
	
	SphereCamera _camera;
	GLMatrix _projectionMatrix,_modelviewMatrix;
	GLMatrix _cameraRotationMatrix,_cameraSelectedRotationMatrix;
	Point _mouseClick;
	
	Joint *_rootJoint;
	Joint *_planarChainRootJoint;
	Joint *_handRootJoint;
	Joint *_selectedJoint;
	
	std::map<Joint*,Point> _endEffectorsMotion;

	// Whether we can use mouse/keyboard to move the camera
	bool _enableUserControl;

	bool _isRecording;
	FrameExporter *_frameExporter;
	QTimer *_animationTimer;
};

#endif /* GLWIDGET_H_ */
