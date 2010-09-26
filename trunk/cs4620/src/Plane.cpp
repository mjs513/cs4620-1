/*
 * Plane.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#include "Plane.h"

#include <cmath>


Plane::Plane() { }

Plane::Plane(const Vector &normal, const Point &point)
	: _normal(normal), _point(point) { }

const Vector& Plane::normal() const
{
	return _normal;
}

Plane& Plane::setNormal(const Vector &normal)
{
	_normal = normal.normalized();
	
	return *this;
}

const Point& Plane::point() const
{
	return _point;
}

Plane& Plane::setPoint(const Point &point)
{
	_point = point;
	
	return *this;
}

double Plane::distance(const Point &p) const
{
	return std::abs(evaluate(p));
}

double Plane::evaluate(const Point &p) const
{
	return Vector::dot(_normal,p - _point);
}