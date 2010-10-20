/*
 * Joint.h
 *
 *  Created on: Oct 19, 2010
 *      Author: Renato
 */

#ifndef JOINT_H_
#define JOINT_H_

#include "Point.h"

class Joint {

public:
	Point pos;
	float thickX, thickY, thickZ;
	int weight;

public:
	Joint();
	virtual ~Joint();
};

#endif /* JOINT_H_ */
