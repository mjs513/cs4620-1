/*
 * PlanarChain.cpp
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#include "PlanarChain.h"
#include "GLWidget.h"

#include <cmath>
#include <iostream>


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

void PlanarChain::update(GLWidget &glWidget)
{
	const double cycle = 15,angleSpan = 100;
	double t = animationCycleTime(cycle);
	
	// Map root angle:
	//	t = 0 or t = 1, angle = 0
	//  t = 0.25, angle = -150
	//  t = 0.5, angle = 0
	//  t = 0.75, angle = 150
	
	_root->setAngle(angleSpan*std::sin(t*2*M_PI));
	
	if(!_root->children().empty()) {
		Joint *j = _root->children().back();
		
		while(j) {
			j->setAngle(j->parent()->angle()*0.95);
			
			if(!j->children().empty()) {
				j = j->children().back();
			}
			else {
				j = 0;
			}
		}
	}
}
