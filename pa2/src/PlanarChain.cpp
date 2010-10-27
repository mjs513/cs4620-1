/*
 * PlanarChain.cpp
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#include "PlanarChain.h"
#include "GLWidget.h"
#include <cmath>

PlanarChain::PlanarChain()
{
	Vector z = Vector(0,0,1);
	int n = 20;
	double size = 10;
	
	// Create n serial joints
	for(int i = 0; i < n; ++i) {
		Joint *j = new Joint(Point(size/n,0),z);

		if(_root) {
			j->addChild(_root);
		}
		_root = j;
	}
}

void PlanarChain::update(GLWidget &glWidget) {
	//std::map<Joint*, float> &targets = glWidget.endEffectorsTarget();
	/*
	float tOffset = 0,n = _originalLegPositions.size();

	for(std::vector< std::pair<Joint*,Point> >::iterator i = _originalLegPositions.begin(); i != _originalLegPositions.end(); ++i) {
		targets[i->first] = _updateLeg(i->first,i->second,tOffset);

		tOffset += 1.0/n;
	}*/

	_updateLink(_root);
}

void PlanarChain::_updateLink(Joint *joint) {

	if( !joint->hasEndEffector() )  {
		for(std::vector<Joint*>::const_iterator i = joint->children().begin(); i != joint->children().end(); i++ ) {
			_updateLink(*i);
		}
	}
	else {
		float angle = std::fmod(float(joint->angle() + 5), 180);
		joint->updateAngle(angle*2);
	}



	//Vector v = length*Vector(std::cos(angle),0,std::sin(angle));

}



