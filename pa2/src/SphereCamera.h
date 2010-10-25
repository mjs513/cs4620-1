/*
 * SphereCamera.h
 *
 *  Created on: Oct 19, 2010
 *      Author: Roberto
 */

#ifndef SPHERECAMERA_H_
#define SPHERECAMERA_H_


#include "Vector.h"
#include "Point.h"

#include <QtGui>


class SphereCamera
{
public:
	SphereCamera(double radius);

	Point eye() const;
	
	void moveUp(double k = 1);
	void moveDown(double k = 1);
	void moveRight(double k = 1);
	void moveLeft(double k = 1);
	void moveFront(double k = 1);
	void moveBack(double k = 1);

	void mouseClicked(QMouseEvent *event);
	void mouseMoved(QMouseEvent *event);
	
	void applyTransformation() const;
	
private:
	double _theta, _fi;
	double _p, _zpos, _r, _xpos, _ypos;
	double _xOld, _yOld;
	double _mouseSensitivity;
};

#endif /* SPHERECAMERA_H_ */
