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
const double NEARVIEW = 1.0;
const double FARVIEW = 100.0;


}  // namespace


SphereCamera::SphereCamera(double radius)
	: _sensitivity(DEFAULT_SENSITIVITY), theta(0.0), fi(M_PI/2), p(20.0) {

	zpos=p*cos(fi);
	r=sqrt(p*p-zpos*zpos);
	xpos=r*cos(theta);
	ypos=r*sin(theta);
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

Point SphereCamera::eye() const
{
	return Point(xpos, ypos, zpos);
}

void SphereCamera::moveUp()
{
	if (fi > 0.0) {
		fi = fi - 0.0125;
		p = sqrt(xpos*xpos + ypos*ypos + zpos*zpos);
		zpos = p*cos(fi);
		r = sqrt (p*p - zpos*zpos);
		xpos = r*cos(theta);
		ypos = r*sin(theta);
	}
}

void SphereCamera::moveDown()
{
	if (fi < M_PI/1.91) {
		fi = fi + 0.0125;
		p = sqrt(xpos*xpos + ypos*ypos + zpos*zpos);
		zpos = p*cos(fi);
		r = sqrt (p*p - zpos*zpos);
		xpos = r*cos(theta);
		ypos = r*sin(theta);
	}
}

void SphereCamera::moveRight()
{
	theta+=0.025;
	r = sqrt(xpos*xpos + ypos*ypos);
	xpos = r*cos(theta);
	ypos = r*sin(theta);
}

void SphereCamera::moveLeft()
{
	theta-=0.025;
	r = sqrt(xpos*xpos + ypos*ypos);
	xpos = r*cos(theta);
	ypos = r*sin(theta);
}


void SphereCamera::moveFront()
{
	p = sqrt(xpos*xpos + ypos*ypos + zpos*zpos);
	
	if (p<NEARVIEW) p = NEARVIEW;
	
	zpos = (zpos * (p-0.25)) / p;
	p = p-0.25;

	// Avoid sqrt of negative number
	if (p*p - zpos*zpos < 0) r = 1;
	else r = sqrt (p*p - zpos*zpos);
	
	xpos = r*cos(theta);
	ypos = r*sin(theta);
}

void SphereCamera::moveBack()
{
	p = sqrt(xpos*xpos + ypos*ypos + zpos*zpos);
	
	if (p>FARVIEW-30.0) p = FARVIEW-30.0;
	else {
		zpos = (zpos * (p+0.25)) / p;
		p = p+0.25;
	}

	// Avoid sqrt of negative number
	if (p*p - zpos*zpos < 0) r = 1;
	else r = sqrt (p*p - zpos*zpos);
	
	xpos = r*cos(theta);
	ypos = r*sin(theta);
}

void SphereCamera::applyTransformation() const
{
	gluLookAt (xpos, ypos, zpos, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}
