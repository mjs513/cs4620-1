/*
 * Plane.h
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#ifndef PLANE_H_
#define PLANE_H_


#include "Vector.h"
#include "Point.h"


class Plane
{
public:
	Plane();
	Plane(const Vector &normal, const Point &point);

	const Vector& normal() const;
	Plane& setNormal(const Vector &normal);

	const Point& point() const;
	Plane& setPoint(const Point &point);
	
	double evaluate(const Point &p) const;
	
private:
	Vector _normal;
	Point _point;
};


#endif /* PLANE_H_ */
