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


Joint::Joint(const Point &pos, const Vector &rotAxis, double thickness, double weight)
	: _id(-1), _pos(pos), _rotAxis(rotAxis), _angle(0), _weight(weight), _minAngle(-1e300/1e-300), _maxAngle(1e300/1e-300),
	  _parent(0), _thickness(thickness) { }

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

double Joint::thickness() const
{
	return _thickness;
}

void Joint::setThickness(double thickness)
{
	_thickness = thickness;
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
	
	// Draw joint
	if(_parent) {
		OpenGL::color(Color::red());
	}
	else {
		OpenGL::color(Color::green());
	}
	
	gluSphere(q,0.1,20,20);

	// Draw link
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	
	OpenGL::color(Color::white());
	
	glBegin(GL_LINES); {
		OpenGL::vertex(Point());
		OpenGL::vertex(_pos);
	}
	glEnd();
	
	glPopAttrib();
	
	// Transform to link's extreme
	OpenGL::translate(_pos);
	
	// Draw end effector
	OpenGL::color(Color::blue());

	if(hasEndEffector()) {
		gluSphere(q,0.1,20,20);
	}
	
	// Draw its children
	for(std::vector<Joint*>::iterator i = _children.begin(); i != _children.end(); ++i) {
		(*i)->display();
	}
	
	gluDeleteQuadric(q);
	
	glPopMatrix();
}
