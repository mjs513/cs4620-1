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
	: _beginTime(QTime::currentTime())
{
	_root = new Joint(Point(0,0,2),Vector(0,0,1));

	_root->setAngleInterval(0,0);
	
	Joint *leg10 = new Joint(Point(0,0,0),Vector(1,-1,0));
	Joint *leg11 = new Joint(Point(1,1,0.5),Vector(0,0,1));
	Joint *leg12 = new Joint(Point(0.5,0.5,-1),Vector(1,-1,0));
	Joint *leg13 = new Joint(Point(0,0,-1.5),Vector(1,-1,0));
	
	leg10->setAngleInterval(-90,90);
	leg11->setAngleInterval(-90,90);
	leg12->setAngleInterval(-90,90);
	leg13->setAngleInterval(-90,90);
	
	_root->addChild(leg10);
	leg10->addChild(leg11);
	leg11->addChild(leg12);
	leg12->addChild(leg13);
	
	Joint *leg20 = new Joint(Point(0,0,0),Vector(1,1,0));
	Joint *leg21 = new Joint(Point(1,-1,0.5),Vector(0,0,1));
	Joint *leg22 = new Joint(Point(0.5,-0.5,-1),Vector(1,1,0));
	Joint *leg23 = new Joint(Point(0,0,-1.5),Vector(1,1,0));

	leg20->setAngleInterval(-90,90);
	leg21->setAngleInterval(-90,90);
	leg22->setAngleInterval(-90,90);
	leg23->setAngleInterval(-90,90);

	_root->addChild(leg20);
	leg20->addChild(leg21);
	leg21->addChild(leg22);
	leg22->addChild(leg23);

	Joint *leg30 = new Joint(Point(0,0,0),Vector(1,-1,0));
	Joint *leg31 = new Joint(Point(-1,-1,0.5),Vector(0,0,1));
	Joint *leg32 = new Joint(Point(-0.5,-0.5,-1),Vector(1,-1,0));
	Joint *leg33 = new Joint(Point(0,0,-1.5),Vector(1,-1,0));

	leg30->setAngleInterval(-90,90);
	leg31->setAngleInterval(-90,90);
	leg32->setAngleInterval(-90,90);
	leg33->setAngleInterval(-90,90);

	_root->addChild(leg30);
	leg30->addChild(leg31);
	leg31->addChild(leg32);
	leg32->addChild(leg33);

	Joint *leg40 = new Joint(Point(0,0,0),Vector(1,1,0));
	Joint *leg41 = new Joint(Point(-1,1,0.5),Vector(0,0,1));
	Joint *leg42 = new Joint(Point(-0.5,0.5,-1),Vector(1,1,0));
	Joint *leg43 = new Joint(Point(0,0,-1.5),Vector(1,1,0));

	leg40->setAngleInterval(-90,90);
	leg41->setAngleInterval(-90,90);
	leg42->setAngleInterval(-90,90);
	leg43->setAngleInterval(-90,90);

	_root->addChild(leg40);
	leg40->addChild(leg41);
	leg41->addChild(leg42);
	leg42->addChild(leg43);

	_originalLegPositions.push_back(std::pair<Joint*,Point>(leg13,leg13->calculateGlobalTransformation()*Point()));
	_originalLegPositions.push_back(std::pair<Joint*,Point>(leg23,leg23->calculateGlobalTransformation()*Point()));
	_originalLegPositions.push_back(std::pair<Joint*,Point>(leg43,leg43->calculateGlobalTransformation()*Point()));
	_originalLegPositions.push_back(std::pair<Joint*,Point>(leg33,leg33->calculateGlobalTransformation()*Point()));
}

void WalkingBug::update(GLWidget &glWidget)
{
	std::map<Joint*,Point> &targets = glWidget.endEffectorsTarget();
	float tOffset = 0,n = _originalLegPositions.size();
	
	for(std::vector< std::pair<Joint*,Point> >::iterator i = _originalLegPositions.begin(); i != _originalLegPositions.end(); ++i) {
		targets[i->first] = _updateLeg(i->first,i->second,tOffset);
		
		tOffset += 1.0/n;
	}
}

Point WalkingBug::_updateLeg(Joint *joint, const Point &original, float tOffset)
{
	const Vector offset(1,0,0);
	const int cycle = 2000;
	int time = _beginTime.msecsTo(QTime::currentTime());
	float t = std::fmod(float(time%cycle)/cycle + tOffset,1);
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
