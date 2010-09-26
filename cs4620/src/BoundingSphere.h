/*
 * BoundingSphere.h
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#ifndef BOUNDINGSPHERE_H_
#define BOUNDINGSPHERE_H_


#include "Plane.h"


class BoundingSphere
{
public:
	BoundingSphere();
	BoundingSphere(const Point &center, double radius);

	const Point& center() const;
	double radius() const;
	
	bool interceptsPlane(const Plane &plane);
	
	static const BoundingSphere createWithAABox(const Point &min, const Point &max);
	
private:
	Point _center;
	double _radius;
};


#endif /* BOUNDINGSPHERE_H_ */
