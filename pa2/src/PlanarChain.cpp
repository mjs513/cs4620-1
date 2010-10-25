/*
 * PlanarChain.cpp
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#include "PlanarChain.h"


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
