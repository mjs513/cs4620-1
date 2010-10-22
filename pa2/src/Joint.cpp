/*
 * Joint.cpp
 *
 *  Created on: Oct 19, 2010
 *      Author: Renato
 */

#include "Joint.h"

#include "OpenGL.h"

#include <cmath>
#include <iostream>


Joint::Joint(const Point &pos, const Vector &rotAxis)
	: _id(-1), _pos(pos), _rotAxis(rotAxis), _angle(0), _parent(0) { }

Joint::~Joint()
{
	for(std::vector<Joint*>::iterator i = _children.begin(); i != _children.end(); ++i) {
		delete *i;
	}
}

void Joint::addChild(Joint *joint)
{
	_children.push_back(joint);
	
	joint->_parent = this;
}

const std::vector<Joint*>& Joint::children() const
{
	return _children;
}

const Joint* Joint::parent() const
{
	return _parent;
}

int Joint::id() const
{
	return _id;
}

void Joint::setId(int id)
{
	_id = id;
}

double Joint::angle() const
{
	return _angle;
}

void Joint::setAngle(double angle)
{
	_angle = angle;
}

void Joint::updateAngle(double deltaTheta) {
	double newAngle = angle() + deltaTheta;
	double deltaThetaMin = _minAngle - newAngle;
	double deltaThetaMax = _maxAngle - newAngle;

	// Adjust theta's convergence speed
	if( deltaTheta > _maxDeltaAngle ) {
		deltaTheta = _maxDeltaAngle;
	}

	deltaTheta = std::max(deltaThetaMin, std::min(deltaThetaMax, deltaTheta));
	setAngle( _angle + deltaTheta );
}

Point Joint::pos() const
{
	return _pos;
}

void Joint::setPos(const Point &pos)
{
	_pos = pos;
}

Vector Joint::rotAxis() const
{
	return _rotAxis;
}

void Joint::setRotAxis(const Vector &rotAxis)
{
	_rotAxis = rotAxis;
}

bool Joint::isEndEffector() const
{
	return _children.empty();
}

const GLMatrix Joint::transformation() const
{
	return GLMatrix::rotationTransform(_angle,_rotAxis)*GLMatrix::translationTransform(_pos);
}

void Joint::display()
{
	glPushMatrix();
	
	GLMatrix m = transformation();

	OpenGL::color(Color::white());
	
	// Draw bones
	glBegin(GL_LINES); {
		OpenGL::vertex(Point());
		OpenGL::vertex(m*Point());
	}
	glEnd();
	
	// Transform to this joint's frame
	glMultMatrixd(m.v);
	
	glGetDoublev(GL_MODELVIEW_MATRIX,m.v);
	
	if(_parent) {
		OpenGL::color(Color::red());
	}
	else {
		OpenGL::color(Color::green());
	}

	// Draw joints
	glBegin(GL_POINTS); {
		OpenGL::vertex(Point());
	}
	glEnd();
	
	glGetDoublev(GL_MODELVIEW_MATRIX,m.v);
	
	for(std::vector<Joint*>::iterator i = _children.begin(); i != _children.end(); ++i) {
		(*i)->display();
	}
	
	glPopMatrix();
}
