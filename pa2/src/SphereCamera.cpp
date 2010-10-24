/*
 * SphereCamera.cpp
 *
 *  Created on: Oct 19, 2010
 *      Author: Roberto
 */

#include "SphereCamera.h"

#include "OpenGL.h"
#include "GLMatrix.h"

#include <cmath>
#include <iostream>


namespace {


const Vector INITIAL_BACK(1,0,0),INITIAL_UP(0,0,1),INITIAL_RIGHT(0,1,0);
const double DEFAULT_SENSITIVITY = 1;


}  // namespace


SphereCamera::SphereCamera(double radius)
	: _eye(radius,0,0), _up(0,0,1), _sensitivity(DEFAULT_SENSITIVITY) { }

double SphereCamera::sensitivity() const
{
	return _sensitivity;
}

SphereCamera& SphereCamera::setSensitivity(double s)
{
	_sensitivity = s;
	
	return *this;
}

Point SphereCamera::eye() const
{
	return _eye;
}

void SphereCamera::_moveVertical(int sign)
{
	Vector front = _center - _eye;
	double length = front.length();
	Vector right = Vector::cross(front.normalized(),_up);
	
	_eye += sign*_sensitivity*_up;
	
	front = (_center - _eye).normalized()*length;
	
	_eye = _center - front;
	
	_up = Vector::cross(right,front).normalized();

	std::cout << "move vertical -- before: right = " << right << std::endl;
	std::cout << "move vertical -- after:  right = " << Vector::cross(front.normalized(),_up) << std::endl;
}

void SphereCamera::moveUp()
{
	_moveVertical(1);
}

void SphereCamera::moveDown()
{
	_moveVertical(-1);
}

void SphereCamera::_moveHorizontal(int sign)
{
	Vector front = _center - _eye;
	double length = front.length();
	Vector right = Vector::cross(front.normalized(),_up);
	
	_eye += sign*_sensitivity*right;

	front = (_center - _eye).normalized()*length;

	_eye = _center - front;
}

void SphereCamera::moveRight()
{
	_moveHorizontal(1);
}

void SphereCamera::moveLeft()
{
	_moveHorizontal(-1);
}

void SphereCamera::_moveRadial(int sign)
{
	Vector front = (_center - _eye).normalized();
	
	_eye += sign*_sensitivity*front;
}

void SphereCamera::moveFront()
{
	_moveRadial(1);
}

void SphereCamera::moveBack()
{
	_moveRadial(-1);
}

void SphereCamera::applyTransformation() const
{
	OpenGL::lookAt(_eye,_center,_up);
}
