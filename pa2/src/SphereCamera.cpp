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


const double DEFAULT_SENSITIVITY = 1;
const double NEARVIEW = 2.0; // Minimum camera distance from center
const double FARVIEW = 90.0; // Maximum camera distance from center

// Update step sizes on camera movements
const double LEFT_RIGHT_STEPS = 0.015;
const double UP_DOWN_STEPS = 0.0075;
const double ZOOM_STEPS = 0.2;


}  // namespace


SphereCamera::SphereCamera(double radius)
	: _theta(0.0), _fi(M_PI/2 - 0.2), _p(20.0) {

	_zpos=_p*cos(_fi);
	_r=sqrt(_p*_p-_zpos*_zpos);
	_xpos=_r*cos(_theta);
	_ypos=_r*sin(_theta);
}

Point SphereCamera::eye() const
{
	return Point(_xpos, _ypos, _zpos);
}

void SphereCamera::moveUp()
{
	if (_fi > 0.0) {
		_fi -= UP_DOWN_STEPS;
		_p = sqrt(_xpos*_xpos + _ypos*_ypos + _zpos*_zpos);
		_zpos = _p*cos(_fi);
		_r = sqrt (_p*_p - _zpos*_zpos);
		_xpos = _r*cos(_theta);
		_ypos = _r*sin(_theta);
	}
}

void SphereCamera::moveDown()
{
	if (_fi < M_PI/1.91) {
		_fi += UP_DOWN_STEPS;
		_p = sqrt(_xpos*_xpos + _ypos*_ypos + _zpos*_zpos);
		_zpos = _p*cos(_fi);
		_r = sqrt (_p*_p - _zpos*_zpos);
		_xpos = _r*cos(_theta);
		_ypos = _r*sin(_theta);
	}
}

void SphereCamera::moveRight()
{
	_theta+=LEFT_RIGHT_STEPS;
	_r = sqrt(_xpos*_xpos + _ypos*_ypos);
	_xpos = _r*cos(_theta);
	_ypos = _r*sin(_theta);
}

void SphereCamera::moveLeft()
{
	_theta-=LEFT_RIGHT_STEPS;
	_r = sqrt(_xpos*_xpos + _ypos*_ypos);
	_xpos = _r*cos(_theta);
	_ypos = _r*sin(_theta);
}


void SphereCamera::moveFront()
{
	_p = sqrt(_xpos*_xpos + _ypos*_ypos + _zpos*_zpos);
	
	if (_p<NEARVIEW) _p = NEARVIEW;
	
	_zpos = (_zpos * (_p-ZOOM_STEPS)) / _p;
	_p = _p-ZOOM_STEPS;

	// Avoid sqrt of negative number
	if (_p*_p - _zpos*_zpos < 0) _r = 1;
	else _r = sqrt (_p*_p - _zpos*_zpos);
	
	_xpos = _r*cos(_theta);
	_ypos = _r*sin(_theta);
}

void SphereCamera::moveBack()
{
	_p = sqrt(_xpos*_xpos + _ypos*_ypos + _zpos*_zpos);
	
	if (_p>FARVIEW-30.0) _p = FARVIEW-30.0;
	else {
		_zpos = (_zpos * (_p+ZOOM_STEPS)) / _p;
		_p = _p+ZOOM_STEPS;
	}

	// Avoid sqrt of negative number
	if (_p*_p - _zpos*_zpos < 0) _r = 1;
	else _r = sqrt (_p*_p - _zpos*_zpos);
	
	_xpos = _r*cos(_theta);
	_ypos = _r*sin(_theta);
}

void SphereCamera::applyTransformation() const
{
	gluLookAt (_xpos, _ypos, _zpos, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}

void SphereCamera::mouseMoved(QMouseEvent *event) {
	const double x = event->x();
	const double y = event->y();

	// Rotation handler
	if (event->buttons() & Qt::RightButton) {
		if (y - _yOld < 0) {
			moveDown();
		}
		else if (y - _yOld > 0) {
			moveUp();
		}
		if (x - _xOld < 0) {
			moveRight();
		}
		else if (x - _xOld > 0) {
			moveLeft();
		}
	}
	// Zoom handler
	if (event->buttons() & Qt::MiddleButton) {
		if (y - _yOld < 0) {
			moveFront();			// Zoom +
		}
		else if (y - _yOld > 0) {
			moveBack();				// Zoom -
		}
	}
	_xOld = x;
	_yOld = y;
}
