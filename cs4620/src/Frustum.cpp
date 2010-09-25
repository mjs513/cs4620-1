/*
 * Frustum.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#include "Frustum.h"


Frustum::Frustum(double fieldOfViewY, double aspectRatio, double zNear, double zFar)
	: _fovy(fieldOfViewY), _aspect(aspectRatio), _zNear(zNear), _zFar(zFar) { }

void Frustum::setPerspective() const
{
	
}
