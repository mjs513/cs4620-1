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
	SphereCamera();
	SphereCamera(const Vector &center, double angleXY, double angleZ, double radius);

	const Point& center() const;
	SphereCamera& setCenter(const Point &center);

	double angleXY() const;
	SphereCamera& setAngleXY(double angleXY);

	double angleZ() const;
	SphereCamera& setAngleZ(double angleZ);

	double radius() const;
	SphereCamera& setRadius(double radius);
	
	double sensitivity() const;
	SphereCamera& setSensitivity(double s);

	Point eye() const;
	
	bool positionChanged() const;
	void markSceneWasRedrawn();
	
	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();
	void moveFront();
	void moveBack();
	
	void applyTransformation() const;
	
private:
	Point _center;
	double _angleXY,_angleZ;
	double _radius;
	double _sensitivity;
	bool _positionChanged;
};

#endif /* SPHERECAMERA_H_ */
