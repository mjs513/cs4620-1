/*
 * Frustum.h
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#ifndef FRUSTUM_H_
#define FRUSTUM_H_


#include "Plane.h"
#include "BoundingSphere.h"


class Frustum
{
public:
	Frustum();

	void setProperties(double fieldOfViewY, double aspectRatio, double zNear, double zFar);
	
	void setPerspective() const;
	
	void applyCamera(const Point &position, const Vector &front, const Vector &up);
	
	bool includes(BoundingSphere sphere) const;
	
private:
	double _fovy,_aspect,_zNear,_zFar;
	Plane _leftPlane,_rightPlane;
};


#endif /* FRUSTUM_H_ */
