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
	Joint(const Point &pos, const Vector &rotAxis, double thickness = 0.1, double weight = 1);
	~Joint();
	
	void addChild(Joint *joint);
	
	const std::vector<Joint*>& children() const;

	Joint* parent();
	const Joint* parent() const;
	
	int id() const;
	void setId(int id);
	
	double angle() const;
	void setAngle(double angle);
	void updateAngle(double deltaTheta);

	double thickness() const;
	void setThickness(double thickness);

	double weight() const;
	void setWeight(double weight);

	double minAngle() const;
	double maxAngle() const;
	void setAngleInterval(double min, double max);
	
	Point pos() const;
	void setPos(const Point &pos);
	
	Vector rotAxis() const;
	void setRotAxis(const Vector &rotAxis);

	bool hasEndEffector() const;

	const GLMatrix transformation() const;
	const GLMatrix calculateGlobalTransformation() const;

	void display();

private:
	// Joint properties
	int _id;
	Point _pos;
	Vector _rotAxis;
	double _angle;
	double _weight;
	
	// Angle limits
	double _minAngle;		// Min permitted angle
	double _maxAngle;		// Max permitted angle
	
	// Structures to access connected joints
	std::vector<Joint*> _children;
	Joint *_parent;

	// Sphere radius around joint
	double _thickness;
};


#endif /* JOINT_H_ */
