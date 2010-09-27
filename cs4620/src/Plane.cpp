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

Plane::Plane(double a, double b, double c, double d)
	: _normal(a,b,c)
{
	_normal.normalize();
	
	d *= _normal.x/a;
	
	_point = Point(0,0,-d/c);
}

Plane::Plane(double *v4)
	: _normal(v4)
{
	_normal.normalize();
	
	double d = v4[3]*_normal.x/v4[0];
	
	_point = Point(0,0,-d/v4[2]);
}

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
