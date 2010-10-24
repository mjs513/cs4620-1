/*
 * IKSolver.cpp
 *
 *  Created on: Oct 22, 2010
 *      Author: Roberto
 */

#include "IKSolver.h"

#include "GLMatrix.h"
#include "newmat/newmat.h"

#include <algorithm>
#include <iostream>
#include <cmath>


namespace {


struct GlobalJoint
{
	Joint *joint;
	Point pos;
	Vector rotAxis;
	
	
	GlobalJoint()
		: joint(0) { }
	
	GlobalJoint(Joint *j, Point p, Vector a)
		: joint(j), pos(p), rotAxis(a) { }
};


struct GlobalEndEffector
{
	Joint *joint;
	Point globalPos;
	Vector deltaPos;
	
	
	GlobalEndEffector(Joint *j)
		: joint(j) { }
};


int indexOfEndEffector(const std::vector<GlobalEndEffector> &v, Joint *j)
{
	for(std::vector<GlobalEndEffector>::const_iterator i = v.begin(); i != v.end(); ++i) {
		if(i->joint == j) {
			return i - v.begin();
		}
	}
	
	return -1;
}


void fillGlobalValues(const JointTree &tree, const map<Joint*,Point> &endEffectorsMotion,
		std::vector<GlobalEndEffector> &endEffectors, std::vector<GlobalJoint> &joints)
{
	endEffectors.reserve(endEffectorsMotion.size());
	
	for(std::map<Joint*,Point>::const_iterator i = endEffectorsMotion.begin(); i != endEffectorsMotion.end(); ++i) {
		endEffectors.push_back(GlobalEndEffector(i->first));
	}
	
	joints.resize(tree.joints().size());
	
	std::vector<Joint*> jointStack;
	std::vector<GLMatrix> matrixStack;
	
	jointStack.push_back(tree.root());
	matrixStack.push_back(GLMatrix());
	
	// Iterate entire tree
	while(!jointStack.empty()) {
		Joint *j = jointStack.back();
		GLMatrix parentM = matrixStack.back();
		GLMatrix jointM = j->transformation()*parentM;
		
		jointStack.pop_back();
		matrixStack.pop_back();
		
		for(std::vector<Joint*>::const_iterator i = j->children().begin(); i != j->children().end(); ++i) {
			jointStack.push_back(*i);
			matrixStack.push_back(jointM);
		}
		
		Point jointOrigin = parentM*Point();
		Point linkEnd = jointM*Point();
		Vector v = parentM*j->rotAxis();
		int id = j->id();
		int eeIndex = indexOfEndEffector(endEffectors,j);
		
		// Joint's end effector is in motion
		if(eeIndex >= 0) {
			endEffectors[eeIndex].globalPos = linkEnd;
			endEffectors[eeIndex].deltaPos = endEffectorsMotion.find(j)->second - linkEnd;
		}
		
		// Fill joint values in world coordinates
		joints[id] = GlobalJoint(j,jointOrigin,v);
	}
}


}  // namespace


IKSolver::IKSolver(const JointTree &tree, double param)
	: _tree(tree), _param(param) { }

void IKSolver::solve(const std::map<Joint*,Point> &endEffectorsMotion) const
{
	std::vector<GlobalEndEffector> endEffectors;
	std::vector<GlobalJoint> joints;

	fillGlobalValues(_tree,endEffectorsMotion,endEffectors,joints);
	
	NEWMAT::Matrix J(3*endEffectors.size(),joints.size()),dp(J.Nrows(),1);

	// Fill dp vector
	for(unsigned int i = 0; i < endEffectors.size(); ++i) {
		Vector &v = endEffectors[i].deltaPos;
		
		dp.element(3*i,0) = v.x;
		dp.element(3*i + 1,0) = v.y;
		dp.element(3*i + 2,0) = v.z;
	}

	// Fill J matrix with zeros
	for(int i = 0; i < J.Nrows(); ++i) {
		for(int j = 0; j < J.Ncols(); ++j) {
			J.element(i,j) = 0;
		}
	}

	// Fill J matrix with dp/dtheta values
	for(unsigned int i = 0; i < endEffectors.size(); ++i) {
		GlobalEndEffector &ee = endEffectors[i];
		const Joint *j = ee.joint;
		const Joint *p = j;
		
		while(p) {
			GlobalJoint &globalParent = joints[p->id()];
			Vector v = Vector::cross(globalParent.rotAxis,ee.globalPos - globalParent.pos);
			
			J.element(3*i,p->id()) = v.x;
			J.element(3*i + 1,p->id()) = v.y;
			J.element(3*i + 2,p->id()) = v.z;
			
			p = p->parent();
		}
	}
	
	NEWMAT::Matrix JT = J.t();
	NEWMAT::Matrix kI = NEWMAT::IdentityMatrix(J.Nrows())*_param;

	// Calculate the damped least squares solution
	NEWMAT::Matrix dTheta = JT*(J*JT + kI).i()*dp;
	
	const double radianToDegree = 180/M_PI;
	
	// Update joint angles
	for(int i = 0; i < dTheta.Nrows(); ++i) {
		Joint *j = joints[i].joint;
		
		//j->updateAngle(dTheta.element(i,0)*radianToDegree);
		j->setAngle(j->angle() + dTheta.element(i,0)*radianToDegree);
	}
}
