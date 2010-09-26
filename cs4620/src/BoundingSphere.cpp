/*
 * BoundingSphere.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#include "BoundingSphere.h"


BoundingSphere::BoundingSphere()
	: _radius(0) { }

BoundingSphere::BoundingSphere(const Point &center, double radius)
	: _center(center), _radius(radius) { }

const Point& BoundingSphere::center() const
{
	return _center;
}

double BoundingSphere::radius() const
{
	return _radius;
}

bool BoundingSphere::interceptsPlane(const Plane &plane)
{
	return plane.distance(_center) <= _radius;
}

const BoundingSphere BoundingSphere::createWithAABox(const Point &min, const Point &max)
{
	Vector radius = 0.5*(max - min);
	
	return BoundingSphere(min + radius,radius.length());
}
