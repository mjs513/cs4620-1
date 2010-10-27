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
#include "Character.h"


class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	
	std::map<Joint*,Point>& endEffectorsTarget();
	
	bool animationEnabled();

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
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent* event);

private:
	void setRecording(bool state);
	Point targetPointFromMouse(const Point &mouse, const Point &refPoint);
	void setAnimationEnabled(bool enable);
	
private slots:
	void animate();
	
private:
	int _width,_height;
	
	SphereCamera _camera;
	GLMatrix _projectionMatrix,_modelviewMatrix;
	Point _mouseClick;
	
	Character *_character;
	Character *_planarChain,*_humanHand,*_walkingBug;
	
	bool _animationEnabled;
	Joint *_selectedJoint;
	std::map<Joint*,Point> _endEffectorsTarget;

	// Whether we can use mouse/keyboard to move the camera
	bool _enableUserControl;

	bool _isRecording;
	FrameExporter *_frameExporter;
	QTimer *_animationTimer;
};

#endif /* GLWIDGET_H_ */