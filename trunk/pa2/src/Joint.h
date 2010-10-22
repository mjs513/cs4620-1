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
#include "Matrix.h"

#include <vector>


class Joint
{
public:
	Joint(double distance, double angle, double weight = 1, double thickness = 1, bool isEndEffector = false);
	~Joint();
	
	void addChild(Joint *joint);
	
	const std::vector<Joint*>& children() const;
	
	int id() const;

	double weight() const;
	Joint& setWeight(double weight);

	double distance() const;
	Joint& setDistance(double distance);

	double angle() const;
	Joint& setAngle(double angle);

	bool isEndEffector() const;

	const Matrix transformation() const;

	void display();
	
	void satisfyConstraints();
	
private:
	void _satisfyDistance();

public:
	
private:
	// Joint properties
	int _id;
	double _distance,_distanceConstraint,_angle;
	double _weight;
	double _thickness;
	bool _isEndEffector;
	
	// Structures to access connected joints
	std::vector<Joint*> _children;
	Joint *_parent;

	// Transformations of the joint in relative and absolute space
	Matrix _absMatrix, _relMatrix;
};


#endif /* JOINT_H_ */
