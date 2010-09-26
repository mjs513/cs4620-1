/*
 * Frustum.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#include "Frustum.h"
#include "OpenGL.h"

#include <cmath>


// Culling code; reference: http://www.lighthouse3d.com/opengl/viewfrustum


Frustum::Frustum()
	: _fovy(0), _aspect(0), _zNear(0), _zFar(0) { }

void Frustum::setProperties(double fieldOfViewY, double aspectRatio, double zNear, double zFar)
{
	_fovy = fieldOfViewY;
	_aspect = aspectRatio;
	_zNear = zNear;
	_zFar = zFar;
}

void Frustum::setPerspective() const
{
	gluPerspective(_fovy,_aspect,_zNear,_zFar);
}

void Frustum::applyCamera(const Point &position, const Vector &front, const Vector &up)
{
	OpenGL::lookAt(position,front,up);

	Vector f = front.normalized(),u = up.normalized(),r = Vector::cross(f,u);
	Vector nearSize,farSize;
	
	nearSize.y = 2*std::tan(0.5*_fovy)*_zNear;
	nearSize.x = nearSize.y*_aspect;

	farSize.y = 2*std::tan(0.5*_fovy)*_zFar;
	farSize.x = farSize.y*_aspect;

	Point nearCenter = position + f*_zNear;
	Point nearTopLeft = nearCenter + 0.5*nearSize.y*u - 0.5*nearSize.x*r;
	Point nearTopRight = nearCenter + 0.5*nearSize.y*u + 0.5*nearSize.x*r;
	Point nearBottomLeft = nearCenter - 0.5*nearSize.y*u - 0.5*nearSize.x*r;
	Point nearBottomRight = nearCenter - 0.5*nearSize.y*u + 0.5*nearSize.x*r;

	Point farCenter = position + f*_zNear;
	Point farTopLeft = farCenter + 0.5*farSize.y*u - 0.5*farSize.x*r;
	Point farTopRight = farCenter + 0.5*farSize.y*u + 0.5*farSize.x*r;
	Point farBottomLeft = farCenter - 0.5*farSize.y*u - 0.5*farSize.x*r;
	Point farBottomRight = farCenter - 0.5*farSize.y*u + 0.5*farSize.x*r;

	_leftPlane = Plane(Vector::cross(nearTopLeft - nearBottomLeft,farBottomLeft - nearBottomLeft),nearBottomLeft);
	_rightPlane = Plane(Vector::cross(nearTopRight - nearBottomRight,farBottomRight - nearBottomRight),nearBottomRight);
}

bool Frustum::includes(BoundingSphere sphere) const
{
	double leftEval = _leftPlane.evaluate(sphere.center()),rightEval = _rightPlane.evaluate(sphere.center());
	
	if((leftEval > 0) && (rightEval > 0)) {
		return true;
	}
	else if(rightEval > 0) {
		return sphere.interceptsPlane(_leftPlane);
	}
	else if(leftEval > 0) {
		return sphere.interceptsPlane(_rightPlane);
	}
	
	return false;
}
