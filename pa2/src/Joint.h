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
	Joint(const Point &pos, const Vector &rotAxis);
	~Joint();
	
	void addChild(Joint *joint);
	
	const std::vector<Joint*>& children() const;
	
	int id() const;

	double angle() const;
	Joint& setAngle(double angle);

	bool isEndEffector() const;

	const Matrix transformation() const;

	void display();

public:
	
private:
	// Joint properties
	int _id;
	Point _pos;
	double _angle;
	Vector _rotAxis;
	bool _isEndEffector;
	
	// Structures to access connected joints
	std::vector<Joint*> _children;
	Joint *_parent;

	// Transformations of the joint in absolute space
	Matrix _absMatrix;
};


#endif /* JOINT_H_ */
