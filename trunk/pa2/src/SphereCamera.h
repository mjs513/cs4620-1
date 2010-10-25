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
	
	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();
	void moveFront();
	void moveBack();
	
	void mouseMoved(QMouseEvent *event);
	
	void applyTransformation() const;
	
private:
	double _theta, _fi;
	double _p, _zpos, _r, _xpos, _ypos;
	double _xOld, _yOld;
};

#endif /* SPHERECAMERA_H_ */
