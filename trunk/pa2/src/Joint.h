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
#include "GLMatrix.h"

#include <vector>


class Joint
{
public:
	Joint(const Point &pos, const Vector &rotAxis);
	~Joint();
	
	void addChild(Joint *joint);
	
	const std::vector<Joint*>& children() const;

	const Joint* parent() const;
	
	int id() const;
	void setId(int id);
	
	double angle() const;
	void setAngle(double angle);
	void updateAngle(double deltaTheta);
	
	Point pos() const;
	void setPos(const Point &pos);
	
	Vector rotAxis() const;
	void setRotAxis(const Vector &rotAxis);

	bool isEndEffector() const;

	const GLMatrix transformation() const;

	void display();

public:
	
private:

	// Joint properties
	int _id;
	Point _pos;
	Vector _rotAxis;
	double _angle;
	double _minAngle;		// Min permitted angle
	double _maxAngle;		// Max permitted angle
	double _maxDeltaAngle;	// Imposes restrictions to angle's convergence speed

	
	// Structures to access connected joints
	std::vector<Joint*> _children;
	Joint *_parent;
};


#endif /* JOINT_H_ */
