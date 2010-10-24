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


Joint::Joint(const Point &pos, const Vector &rotAxis, double thickness)
	: _id(-1), _pos(pos), _rotAxis(rotAxis), _angle(0), _parent(0), _thickness(thickness) { }

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

Joint* Joint::parent()
{
	return _parent;
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

void Joint::updateAngle(double deltaTheta)
{
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

bool Joint::hasEndEffector() const
{
	return _children.empty();
}

const GLMatrix Joint::transformation() const
{
	return GLMatrix::translationTransform(_pos)*GLMatrix::rotationTransform(_angle,_rotAxis);
}

const GLMatrix Joint::calculateGlobalTransformation() const
{
	GLMatrix m;
	const Joint *j = this;
	
	while(j) {
		m = m*j->transformation();
		
		j = j->parent();
	}
	
	return m;
}

void Joint::display()
{
	glPushMatrix();
	
	OpenGL::rotate(_angle,_rotAxis);

	GLUquadric *q = gluNewQuadric();
	
	if(_parent) {
		OpenGL::color(Color::red());
	}
	else {
		OpenGL::color(Color::green());
	}
	
	// Draw joint
	gluSphere(q,_thickness*1.2,20,20);

	OpenGL::color(Color::white());

	// Draw link
	glPushMatrix();
	
	Vector v = Vector(_pos).normalized(),z(0,0,1);
	double ang = std::acos(Vector::dot(v,z));
	
	if(v.z < 0) {
		ang = 2*M_PI - ang;
	}
	
	OpenGL::rotate(ang,Vector::cross(z,v));
	
	gluCylinder(q,_thickness,_thickness,(_pos - Point()).length(),20,20);
	
	glPopMatrix();
	
	
	glBegin(GL_LINES); {
		OpenGL::vertex(Point());
		OpenGL::vertex(_pos);
	}
	glEnd();
	
	OpenGL::translate(_pos);
	
	if(hasEndEffector()) {
		OpenGL::color(Color::blue());
		
		glBegin(GL_POINTS); {
			OpenGL::vertex(Point());
		}
		glEnd();
	}
	
	for(std::vector<Joint*>::iterator i = _children.begin(); i != _children.end(); ++i) {
		(*i)->display();
	}
	
	gluDeleteQuadric(q);
	
	glPopMatrix();
}
