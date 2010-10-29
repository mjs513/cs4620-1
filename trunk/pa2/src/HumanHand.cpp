/*
 * HumanHand.cpp
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#include "HumanHand.h"
#include <cmath>
#include <iostream>

HumanHand::HumanHand()
{
	// Create wrist with 2 degrees of freedom
	Joint *wrist = new Joint(Point(0,0,0.5), Vector(1,0,0), 0.3, 10000);
	wrist->setAngleInterval(0, 45);
	
	Joint *wrist2 = new Joint(Point(0,0,0), Vector(0,1,0), 0.1, 10000);
	wrist2->setAngleInterval(-45, 15);

	wrist->addChild(wrist2);

	int nfingers = 5;

	// Create 4 fingers (except thumb)
	for(int i = 0; i < nfingers - 1; ++i) {

		// Create fingerBase
		Joint *fingerBase = new Joint(Point(i - (nfingers - 1)*0.5, 0, 1), Vector(0, 1, 0), 0.3);
		fingerBase->setAngleInterval(-15, 15);
		
		// First segment of finger
		Joint *finger1 = new Joint(Point(0, 0, 1), Vector(1, 0, 0), 0.3);
		finger1->setAngleInterval(-15, 45);
		
		// Second segment of finger
		Joint *finger2 = new Joint(Point(0, 0, 0.66), Vector(1, 0, 0), 0.25);
		finger2->setAngleInterval(-15, 95);
		
		// Finger tip
		Joint *fingerTip = new Joint(Point(0, 0, 0.33), Vector(1, 0, 0), 0.2);
		fingerTip->setAngleInterval(0, 35);
		
		wrist2->addChild(fingerBase);
		fingerBase->addChild(finger1);
		finger1->addChild(finger2);
		finger2->addChild(fingerTip);
	}
	
	// Create thumb
	// Create fingerBase
	Joint *thumbBase = new Joint(Point(5 - (nfingers - 1)*0.9, -0.75, 0.5), Vector(0, 1, 0), 0.35);
	thumbBase->setAngleInterval(-15, 15);

	// First segment of finger (2 degrees of freedom)
	Joint *thumbSegment1 = new Joint(Point(0, 0, 1), Vector(0, -1, 0), 0.3);
	thumbSegment1->setAngleInterval(-15, 15);

	Joint *thumbSegment12 = new Joint(Point(0, 0, 0), Vector(1, 0, 0), 0.3);
	thumbSegment12->setAngleInterval(-15, 60);

	// Second segment of finger
	Joint *thumbSegment2 = new Joint(Point(0, 0, 0.66), Vector(0, -1, 0), 0.25);
	thumbSegment2->setAngleInterval(0, 90);

	wrist2->addChild(thumbBase);
	thumbBase->addChild(thumbSegment1);
	thumbSegment1->addChild(thumbSegment12);
	thumbSegment12->addChild(thumbSegment2);

	_root = wrist;
}

void HumanHand::update(GLWidget &glWidget)
{
	const double cycle = 10;
	double t = animationCycleTime(cycle);

	if(!_root->children().empty()) {

		// Get wrist part that is parent for all fingers
		Joint *rootChild = _root->children().front();

		// Auxiliary joint pointer for iterating on a finger
		Joint *j;

		// Iterate on each finger
		for(std::vector<Joint*>::const_iterator i = rootChild->children().begin(); i != rootChild->children().end(); i++) {

			j = (*i);

			// Iterate on the whole finger and update angles
			while(j) {
				j->updateAngle( std::sin( t*5*M_PI) );

				if(!j->children().empty()) {
					j = j->children().back();
				}
				else {
					j = 0;
				}
			}
		}
	}
}
