/*
 * Ray.cpp
 *
 *  Created on: Oct 20, 2010
 *      Author: Roberto
 */

#include "Ray.h"


Ray::Ray() { }

Ray::Ray(const Point &p, const Vector &dir)
	: p(p), dir(dir) { }

Ray::Ray(const Point &p1, const Point &p2)
	: p(p1), dir(p2 - p1) { }

double Ray::distance(const Point &point) const
{
	Point projPoint = p + (point - p).projection(dir);
	
	return (projPoint - point).length();
}

Point Ray::pointAt(double t) const
{
	return p + t*dir;
}
