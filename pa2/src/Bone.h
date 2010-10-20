/*
 * Bone.h
 *
 *  Created on: Oct 20, 2010
 *      Author: Renato
 */

#ifndef BONE_H_
#define BONE_H_

#include "Joint.h"
#include <vector>

/**
 * Defines a connection between 2 Joints.
 */
class Bone {

public:
	float length; // Maybe interesting to save instead of computing endJoint - originJoint ?
	Joint *originJoint, *endJoint;
	Bone *parent;
	std::vector<Bone> children;
	float angle; // Angle with its parent

public:
	Bone(Joint **orig, Joint **end, float lenght);
	virtual ~Bone();

	void draw();
};

#endif /* BONE_H_ */
