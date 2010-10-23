/*
 * Ray.h
 *
 *  Created on: Oct 20, 2010
 *      Author: Roberto
 */

#ifndef RAY_H_
#define RAY_H_


#include "Point.h"
#include "Vector.h"


struct Ray
{
	Point p;
	Vector dir;
	
	
	Ray();
	Ray(const Point &p, const Vector &dir);
	Ray(const Point &p1, const Point &p2);
	
	double distance(const Point &p) const;

	Point pointAt(double t) const;
};


#endif /* RAY_H_ */
