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


Point pointFromPolar(double dist, double angle)
{
	return Point() + dist*Vector(std::cos(angle),std::sin(angle));
}

std::pair<double,double> polarFromPoint(const Point &p)
{
	double length = Vector(p).length();
	double angle = std::acos(p.x/length);
	
	if(p.y < 0) {
		angle = 2*M_PI - angle;
	}
	
	return std::pair<double,double>(length,angle*180/M_PI);
}


}  // namespace


Joint::Joint(double distance, double angle, double weight, double thickness)
	: _distance(distance), _distanceConstraint(distance), _angle(angle), _weight(weight), _thickness(thickness)
{
	_parent = 0;
}

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

double Joint::weight() const
{
	return _weight;
}

Joint& Joint::setWeight(double weight)
{
	_weight = weight;
	
	return *this;
}

double Joint::distance() const
{
	return _distance;
}

Joint& Joint::setDistance(double distance)
{
	_distance = distance;
	
	return *this;
}

double Joint::angle() const
{
	return _angle;
}

Joint& Joint::setAngle(double angle)
{
	_angle = angle;
	
	return *this;
}

const Matrix Joint::transformation() const
{
	return Matrix::rotationTransform(_angle,Vector(0,0,1))*Matrix::translationTransform(Point(_distance,0,0));
}

void Joint::display()
{
	glPushMatrix();
	
	Matrix m = transformation();
	
	glBegin(GL_LINES); {
		OpenGL::vertex(Point());
		OpenGL::vertex(m*Point());
	}
	glEnd();
	
	glMultMatrixd(m.v);
	
	glGetDoublev(GL_MODELVIEW_MATRIX,m.v);
	
	for(std::vector<Joint*>::iterator i = _children.begin(); i != _children.end(); ++i) {
		(*i)->display();
	}
	
	glPopMatrix();
}

void Joint::satisfyConstraints()
{
	this->_satisfyDistance();
	
	for(std::vector<Joint*>::iterator i = _children.begin(); i != _children.end(); ++i) {
		(*i)->_satisfyDistance();
		
		(*i)->satisfyConstraints();
	}
}

void Joint::_satisfyDistance()
{
	const double MIN_DISTANCE = 0.001;
	
	if(this->_parent) {
		double diff = std::fabs(this->_distance - this->_distanceConstraint);
		
		if(std::fabs(diff) > MIN_DISTANCE) {
			Point parentPos = pointFromPolar(_parent->_distance,_parent->_angle);
			Point thisPos = pointFromPolar(this->_distance,this->_angle);
			Vector vdiff = thisPos - parentPos;
			
			vdiff.normalize();
			vdiff *= diff;
			
			parentPos += vdiff*0.5;
			thisPos -= vdiff*0.5;
			
			std::pair<double,double> thisPolar = polarFromPoint(thisPos),parentPolar = polarFromPoint(parentPos);
			
			this->_distance = thisPolar.first;
			this->_angle = thisPolar.second;
			
			_parent->_distance = parentPolar.first;
			_parent->_angle = parentPolar.second;
		}
	}
	
	for(std::vector<Joint*>::iterator i = _children.begin(); i != _children.end(); ++i) {
		
	}
}
