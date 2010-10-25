/*
 * HumanHand.cpp
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#include "HumanHand.h"


HumanHand::HumanHand()
{
	// Create wrist with 2 degrees of freedom
	Joint *wrist = new Joint(Point(0,0,0.5), Vector(1,0,0), 0.3, 10);
	wrist->setAngleInterval(0, 45);
	
	Joint *wrist2 = new Joint(Point(0,0,0), Vector(0,1,0), 0.1, 10);
	wrist2->setAngleInterval(-45, 15);

	wrist->addChild(wrist2);

	int nfingers = 5;

	// Create 4 fingers (except thumb)
	for(int i = 0; i < nfingers - 1; ++i) {

		// Create fingerBase with 2 degrees of freedom
		Joint *fingerBase = new Joint(Point(i - (nfingers - 1)*0.5, 0, 1), Vector(1, 0, 0), 0.3);
		fingerBase->setAngleInterval(0, 0);

		Joint *fingerBase2 = new Joint(Point(0, 0, 0), Vector(0, 1, 0), 0.3);
		fingerBase2->setAngleInterval(-15, 15);
		
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
		fingerBase->addChild(fingerBase2);
		fingerBase2->addChild(finger1);
		finger1->addChild(finger2);
		finger2->addChild(fingerTip);
	}
	
	// Create thumb
	// Create fingerBase with 2 degrees of freedom
	Joint *thumbBase = new Joint(Point(5 - (nfingers - 1)*0.9, -0.75, 0.5), Vector(1, 0, 0), 0.35);
	thumbBase->setAngleInterval(0, 0);

	Joint *thumbBase2 = new Joint(Point(0, 0, 0), Vector(0, 1, 0), 0.35);
	thumbBase2->setAngleInterval(-15, 60);

	// First segment of finger (2 degrees of freedom)
	Joint *thumbSegment1 = new Joint(Point(0, 0, 1), Vector(0, -1, 0), 0.3);
	thumbSegment1->setAngleInterval(-15, 15);

	Joint *thumbSegment12 = new Joint(Point(0, 0, 0), Vector(1, 0, 0), 0.3);
	thumbSegment12->setAngleInterval(-15, 60);

	// Second segment of finger
	Joint *thumbSegment2 = new Joint(Point(0, 0, 0.66), Vector(0, -1, 0), 0.25);
	thumbSegment2->setAngleInterval(0, 90);

	wrist2->addChild(thumbBase);
	thumbBase->addChild(thumbBase2);
	thumbBase2->addChild(thumbSegment1);
	thumbSegment1->addChild(thumbSegment12);
	thumbSegment12->addChild(thumbSegment2);

	_root = wrist;
}
