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
	Matrix model,proj;

	glGetDoublev(GL_MODELVIEW_MATRIX,model.v);
	glGetDoublev(GL_PROJECTION_MATRIX,proj.v);
	
	Matrix clip = proj*model;
	double frustum[6][4];

	frustum[0][0] = clip.v[3] - clip.v[0];
	frustum[0][1] = clip.v[7] - clip.v[4];
	frustum[0][2] = clip.v[11] - clip.v[8];
	frustum[0][3] = clip.v[15] - clip.v[12];
	
	frustum[1][0] = clip.v[3] + clip.v[0];
	frustum[1][1] = clip.v[7] + clip.v[4];
	frustum[1][2] = clip.v[11] + clip.v[8];
	frustum[1][3] = clip.v[15] + clip.v[12];
	
	frustum[2][0] = clip.v[3] + clip.v[1];
	frustum[2][1] = clip.v[7] + clip.v[5];
	frustum[2][2] = clip.v[11] + clip.v[9];
	frustum[2][3] = clip.v[15] + clip.v[13];
	
	frustum[3][0] = clip.v[3] - clip.v[1];
	frustum[3][1] = clip.v[7] - clip.v[5];
	frustum[3][2] = clip.v[11] - clip.v[9];
	frustum[3][3] = clip.v[15] - clip.v[13];
	
	frustum[4][0] = clip.v[3] - clip.v[2];
	frustum[4][1] = clip.v[7] - clip.v[6];
	frustum[4][2] = clip.v[11] - clip.v[10];
	frustum[4][3] = clip.v[15] - clip.v[14];
	
	frustum[5][0] = clip.v[3] + clip.v[2];
	frustum[5][1] = clip.v[7] + clip.v[6];
	frustum[5][2] = clip.v[11] + clip.v[10];
	frustum[5][3] = clip.v[15] + clip.v[14];

	for(int i = 0; i < 6; ++i) {
		double x = 0;
		
		for(int j = 0; j < 3; ++j) {
			x += frustum[i][j]*frustum[i][j];
		}
		
		x = std::sqrt(x);
		
		for(int j = 0; j < 4; ++j) {
			frustum[i][j] /= x;
		}
	}
	
	for(int i = 0; i < 6; ++i) {
		Vector normal(frustum[i]);
		Point point(0,0,-frustum[i][4]/frustum[i][3]);
		
		_planes.push_back(Plane(normal,point));
	}
}

bool Frustum::includes(const Point &p) const
{
	for(std::vector<Plane>::const_iterator i = _planes.begin(); i != _planes.end(); ++i) {
		if(i->evaluate(p) < 0) {
			return false;
		}
	}
	
	return true;
}

bool Frustum::includes(const BoundingSphere &sphere) const
{
	Point p = sphere.center();
	Matrix m;
	
	glGetDoublev(GL_MODELVIEW_MATRIX,m.v);
	
	p = m*p;

	for(std::vector<Plane>::const_iterator i = _planes.begin(); i != _planes.end(); ++i) {
		if(i->evaluate(p) > sphere.radius()) {
			return false;
		}
	}
	
	return true;
}
