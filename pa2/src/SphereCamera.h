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


class SphereCamera
{
public:
	SphereCamera(double radius);

	double sensitivity() const;
	SphereCamera& setSensitivity(double s);

	Point eye() const;
	
	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();
	void moveFront();
	void moveBack();
	
	void applyTransformation() const;
	
private:
	void _moveVertical(int sign);
	void _moveHorizontal(int sign);
	void _moveRadial(int sign);
	
private:
	Point _center,_eye;
	Vector _up;
	double _sensitivity;
};

#endif /* SPHERECAMERA_H_ */
