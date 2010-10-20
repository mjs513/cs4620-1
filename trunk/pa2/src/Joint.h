/*
 * Joint.h
 *
 *  Created on: Oct 19, 2010
 *      Author: Renato
 */

#ifndef JOINT_H_
#define JOINT_H_

#include "Point.h"
#include "Vector.h"

#include <vector>


class Joint
{
public:
	Joint(double distance, double angle, double weight = 1, double thickness = 1);
	~Joint();
	
	void addChild(Joint *joint);
	
	const std::vector<Joint*>& children() const;
	
	double weight() const;
	Joint& setWeight(double weight);

	double distance() const;
	Joint& setDistance(double distance);

	double angle() const;
	Joint& setAngle(double angle);

	void display();
	
	void satisfyConstraints();
	
private:
	void _satisfyDistance();
	
public:
	double _distance,_distanceConstraint,_angle;
	double _weight;
	double _thickness;
	
private:
	std::vector<Joint*> _children;
	Joint *_parent;
};


#endif /* JOINT_H_ */
