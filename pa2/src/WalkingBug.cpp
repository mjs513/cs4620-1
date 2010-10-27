/*
 * WalkingBug.cpp
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#include "WalkingBug.h"

#include "GLWidget.h"

#include <iostream>
#include <cmath>


WalkingBug::WalkingBug()
{
	_root = new Joint(Point(0,0,2),Vector(0,0,1));

	_root->setAngleInterval(0,0);
	
	std::vector<std::pair<Joint*,Joint*> > legs;

	legs.push_back(_createLeg(Vector(1,1)));
	legs.push_back(_createLeg(Vector(-1,1)));
	legs.push_back(_createLeg(Vector(-1,-1)));
	legs.push_back(_createLeg(Vector(1,-1)));
	legs.push_back(_createLeg(Vector(0,1)));
	legs.push_back(_createLeg(Vector(0,-1)));
	
	for(std::vector< std::pair<Joint*,Joint*> >::iterator i = legs.begin(); i != legs.end(); ++i) {
		_root->addChild(i->first);
		
		_originalLegPositions.push_back(std::pair<Joint*,Point>(i->second,i->second->calculateGlobalTransformation()*Point()));
	}
}

void WalkingBug::update(GLWidget &glWidget)
{
	const double cycle = 1;
	double t = animationCycleTime(cycle);
	double tOffset = 0,n = _originalLegPositions.size();
	std::map<Joint*,Point> &targets = glWidget.endEffectorsTarget();
	
	for(std::vector< std::pair<Joint*,Point> >::iterator i = _originalLegPositions.begin(); i != _originalLegPositions.end(); ++i) {
		targets[i->first] = _updateLeg(i->first,i->second,std::fmod(t + tOffset,1));
		
		tOffset += 1.0/n;
	}
}

Point WalkingBug::_updateLeg(Joint *joint, const Point &original, double t)
{
	const Vector offset(0.5,0,0);
	Point p;

	// Run half circle
	if(t < 0.5) {
		t *= 2;
		
		float angle = (1 - t)*M_PI;
		double length = offset.length();
		Vector v = length*Vector(std::cos(angle),0,std::sin(angle));
		
		p = original + v;
	}
	
	// Straight way back
	else {
		t = (t - 0.5)*2;
		
		Point begin = original + offset,end = original - offset;
		
		p = Point() + (1 - t)*Vector(begin) + t*Vector(end);
	}
	
	return p;
}

std::pair<Joint*,Joint*> WalkingBug::_createLeg(const Vector &orientation)
{
	Vector xy = orientation.normalized(),rotAxis(xy.y,-xy.x);
	
	xy.z = 0;
	
	Joint *leg0 = new Joint(Point(),rotAxis);
	Joint *leg1 = new Joint(Point() + 1.4*xy + Vector(0,0,0.5),Vector(0,0,1));
	Joint *leg2 = new Joint(Point() + 0.7*xy + Vector(0,0,-1),rotAxis);
	Joint *leg3 = new Joint(Point() + Vector(0,0,-1.5),rotAxis);
	
	leg0->setAngleInterval(-90,90);
	leg1->setAngleInterval(-90,90);
	leg2->setAngleInterval(-90,90);
	leg3->setAngleInterval(-90,90);
	
	leg0->addChild(leg1);
	leg1->addChild(leg2);
	leg2->addChild(leg3);
	
	return std::pair<Joint*,Joint*>(leg0,leg3);
}
