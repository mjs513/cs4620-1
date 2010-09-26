/*
 * Displayable.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#include "Displayable.h"
#include "Matrix.h"
#include "OpenGL.h"

#include <iostream>


Displayable::~Displayable() { }

void Displayable::display(const Frustum &frustum)
{
	Matrix m;
	
	glGetDoublev(GL_MODELVIEW_MATRIX,m.v);
	
	Vector radiusVector(_boundingSphere.radius(),_boundingSphere.radius(),_boundingSphere.radius());
	
	radiusVector = m*radiusVector;
	
	for(int i = 1; i < 3; ++i) {
		if(radiusVector.v[i] > radiusVector.v[0]) {
			radiusVector.v[0] = radiusVector.v[i];
		}
	}
	
	BoundingSphere transformedSphere(m*_boundingSphere.center(),radiusVector.v[0]);
	bool doDraw = true;
	
	if(testFrustum()) {
		doDraw = frustum.includes(transformedSphere);
	}
	
	if(doDraw) {
		draw(frustum);
	}
}

Displayable& Displayable::setBoundingSphere(const BoundingSphere &sphere)
{
	_boundingSphere = sphere;
	
	return *this;
}

const BoundingSphere& Displayable::boundingSphere() const
{
	return _boundingSphere;
}
