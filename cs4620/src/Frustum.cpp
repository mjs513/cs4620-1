/*
 * Frustum.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#include "Frustum.h"
#include "OpenGL.h"
#include "Matrix.h"

#include <cmath>
#include <iostream>


// Culling code; reference: http://www.crownandcutlass.com/features/technicaldetails/frustum.html


Frustum::Frustum()
{
	// Create frustum from projection matrix -- 
	Matrix proj;
	
	glGetDoublev(GL_PROJECTION_MATRIX,proj.v);
	
	_frustum[0][0] = proj.v[3] - proj.v[0];
	_frustum[0][1] = proj.v[7] - proj.v[4];
	_frustum[0][2] = proj.v[11] - proj.v[8];
	_frustum[0][3] = proj.v[15] - proj.v[12];
	
	_frustum[1][0] = proj.v[3] + proj.v[0];
	_frustum[1][1] = proj.v[7] + proj.v[4];
	_frustum[1][2] = proj.v[11] + proj.v[8];
	_frustum[1][3] = proj.v[15] + proj.v[12];
	
	_frustum[2][0] = proj.v[3] + proj.v[1];
	_frustum[2][1] = proj.v[7] + proj.v[5];
	_frustum[2][2] = proj.v[11] + proj.v[9];
	_frustum[2][3] = proj.v[15] + proj.v[13];
	
	_frustum[3][0] = proj.v[3] - proj.v[1];
	_frustum[3][1] = proj.v[7] - proj.v[5];
	_frustum[3][2] = proj.v[11] - proj.v[9];
	_frustum[3][3] = proj.v[15] - proj.v[13];
	
	_frustum[4][0] = proj.v[3] - proj.v[2];
	_frustum[4][1] = proj.v[7] - proj.v[6];
	_frustum[4][2] = proj.v[11] - proj.v[10];
	_frustum[4][3] = proj.v[15] - proj.v[14];
	
	_frustum[5][0] = proj.v[3] + proj.v[2];
	_frustum[5][1] = proj.v[7] + proj.v[6];
	_frustum[5][2] = proj.v[11] + proj.v[10];
	_frustum[5][3] = proj.v[15] + proj.v[14];

	for(int i = 0; i < 6; ++i) {
		double x = 0;
		
		for(int j = 0; j < 3; ++j) {
			x += _frustum[i][j]*_frustum[i][j];
		}
		
		x = std::sqrt(x);
		
		for(int j = 0; j < 4; ++j) {
			_frustum[i][j] /= x;
		}
	}
}

bool Frustum::includes(const Point &p) const
{
	// Transform point using modelview
	Matrix model;
	
	glGetDoublev(GL_MODELVIEW_MATRIX,model.v);
	
	Point p2 = model*p;

	// Test all planes
	for(int i = 0; i < 6; ++i) {
		if(_frustum[i][0]*p2.x + _frustum[i][1]*p2.y + _frustum[i][2]*p2.z + _frustum[i][3] < 0) {
			return false;
		}
	}
	
	return true;
}

bool Frustum::includes(const BoundingSphere &sphere) const
{
	// Transform bounding sphere using modelview
	Matrix model;
	
	glGetDoublev(GL_MODELVIEW_MATRIX,model.v);
	
	Vector radiusVector(sphere.radius(),sphere.radius(),sphere.radius());
	
	radiusVector = model*radiusVector;
	
	for(int i = 1; i < 3; ++i) {
		if(radiusVector.v[i] > radiusVector.v[0]) {
			radiusVector.v[0] = radiusVector.v[i];
		}
	}
	
	// From transformed sphere (now possibly ellpsis), get a conservative bounding sphere around it
	BoundingSphere newSphere(model*sphere.center(),radiusVector.v[0]);
	Point p = newSphere.center();
	
	// Test all planes
	for(int i = 0; i < 6; ++i) {
		if(_frustum[i][0]*p.x + _frustum[i][1]*p.y + _frustum[i][2]*p.z + _frustum[i][3] < -newSphere.radius()) {
			return false;
		}
	}
	
	return true;
}
