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


namespace {


const double MAX_DELTA_ANGLE = 1;


}  // namespace


Joint::Joint(const Point &pos, const Vector &rotAxis, double weight)
	: _id(-1), _pos(pos), _rotAxis(rotAxis), _angle(0), _weight(weight), _minAngle(-1e300/1e-300), _maxAngle(1e300/1e-300),
	  _parent(0), _thickness(0.1) { }

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
	if(angle > _maxAngle) {
		angle = _maxAngle;
	}
	
	if(angle < _minAngle) {
		angle = _minAngle;
	}
	
	_angle = angle;
}

void Joint::updateAngle(double deltaTheta)
{
	double newAngle = angle() + deltaTheta;
	double deltaThetaMin = _minAngle - newAngle;
	double deltaThetaMax = _maxAngle - newAngle;

	// Adjust theta's convergence speed
	if( deltaTheta > MAX_DELTA_ANGLE ) {
		deltaTheta = MAX_DELTA_ANGLE;
	}

	deltaTheta = std::max(deltaThetaMin, std::min(deltaThetaMax, deltaTheta));
	setAngle( _angle + deltaTheta );
}

double Joint::weight() const
{
	return _weight;
}

void Joint::setWeight(double weight)
{
	_weight = weight;
}

double Joint::minAngle() const
{
	return _minAngle;
}

double Joint::maxAngle() const
{
	return _maxAngle;
}

void Joint::setAngleInterval(double min, double max)
{
	if(min > max) {
		max = min;
	}
	
	_minAngle = min;
	_maxAngle = max;
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
	glBegin(GL_LINES); {
		OpenGL::vertex(Point());
		OpenGL::vertex(_pos);
	}
	glEnd();
	
	glPushMatrix();
	
	Vector v = Vector(_pos).normalized();
	double angX = std::acos(v.x),angY = std::acos(v.y);

	if(v.x < 0) {
		angX = 2*M_PI - angX;
	}
	
	if(v.y < 0) {
		angY = 2*M_PI - angY;
	}
	
	OpenGL::rotate(90 - 180/M_PI*angX,Vector(0,1,0));
	OpenGL::rotate(90 - 180/M_PI*angY,Vector(1,0,0));
	
	gluCylinder(q,_thickness,_thickness,(_pos - Point()).length(),20,20);
	
	glPopMatrix();
	
	OpenGL::translate(_pos);

	// Draw end effector
	if(hasEndEffector()) {
		OpenGL::color(Color::blue());
	}
	
	for(std::vector<Joint*>::iterator i = _children.begin(); i != _children.end(); ++i) {
		(*i)->display();
	}
	
	gluDeleteQuadric(q);
	
	glPopMatrix();
}
