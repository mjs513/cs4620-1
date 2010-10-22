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


Joint::Joint(const Point &pos, const Vector &rotAxis)
	: _pos(pos), _rotAxis(rotAxis), _parent(0) { }

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

const Matrix Joint::transformation() const
{
	//return Matrix::translationTransform(Point(_distance,0,0))*Matrix::rotationTransform(_angle,Vector(0,0,1));
}

void Joint::display()
{
	glPushMatrix();
	
	Matrix m = transformation();

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
	
	std::cout << "display point = " << m*Point() << std::endl;
	
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
