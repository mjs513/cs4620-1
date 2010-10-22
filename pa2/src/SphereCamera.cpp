/*
 * SphereCamera.cpp
 *
 *  Created on: Oct 19, 2010
 *      Author: Roberto
 */

#include "SphereCamera.h"

#include "OpenGL.h"
#include "Matrix.h"
#include <cmath>


namespace {


const Vector INITIAL_BACK(1,0,0),INITIAL_UP(0,0,1),INITIAL_RIGHT(0,1,0);
const double DEFAULT_SENSITIVITY = 1;


}  // namespace


SphereCamera::SphereCamera()
	: _angleXY(0), _angleZ(0), _radius(1), _sensitivity(DEFAULT_SENSITIVITY), _positionChanged(false) { }

SphereCamera::SphereCamera(const Vector &center, double angleXY, double angleZ, double radius)
	: _center(center), _angleXY(angleXY), _angleZ(angleZ), _radius(radius), _sensitivity(DEFAULT_SENSITIVITY),
	  _positionChanged(false) { }

const Point& SphereCamera::center() const
{
	return _center;
}

SphereCamera& SphereCamera::setCenter(const Point &center)
{
	_center = center;
	
	_positionChanged = true;
	
	return *this;
}

double SphereCamera::angleXY() const
{
	return _angleXY;
}

SphereCamera& SphereCamera::setAngleXY(double angleXY)
{
	_angleXY = angleXY;
	
	_positionChanged = true;
	
	return *this;
}

double SphereCamera::angleZ() const
{
	return _angleZ;
}

SphereCamera& SphereCamera::setAngleZ(double angleZ)
{
	_angleZ = angleZ;
	
	_positionChanged = true;
	
	return *this;
}

double SphereCamera::radius() const
{
	return _radius;
}

SphereCamera& SphereCamera::setRadius(double radius)
{
	_radius = radius;
	
	_positionChanged = true;
	
	return *this;
}

double SphereCamera::sensitivity() const
{
	return _sensitivity;
}

SphereCamera& SphereCamera::setSensitivity(double s)
{
	_sensitivity = s;
	
	return *this;
}

bool SphereCamera::positionChanged() const
{
	return _positionChanged;
}

void SphereCamera::markSceneWasRedrawn()
{
	_positionChanged = false;
}

void SphereCamera::moveUp()
{
	_angleZ -= _sensitivity;
	
	_positionChanged = true;
}

void SphereCamera::moveDown()
{
	_angleZ += _sensitivity;
	
	_positionChanged = true;
}

void SphereCamera::moveRight()
{
	_angleXY += _sensitivity;
	
	_positionChanged = true;
}

void SphereCamera::moveLeft()
{
	_angleXY -= _sensitivity;
	
	_positionChanged = true;
}

void SphereCamera::moveFront()
{
	_radius -= 0.5*_sensitivity;
	
	if(_radius < 0) {
		_radius = 0;
	}
	
	_positionChanged = true;
}

void SphereCamera::moveBack()
{
	_radius += 0.5*_sensitivity;
	
	_positionChanged = true;
}

void SphereCamera::applyTransformation() const
{
	Vector back = INITIAL_BACK,up = INITIAL_UP;
	Matrix m = Matrix::rotationTransform(_angleXY,Vector(0,0,1))*Matrix::rotationTransform(_angleZ,INITIAL_RIGHT);
	
	back = m*back;
	up = m*up;
	
	OpenGL::lookAt(_center + _radius*back,_center,up);
}
