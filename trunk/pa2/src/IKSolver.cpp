/*
 * IKSolver.cpp
 *
 *  Created on: Oct 22, 2010
 *      Author: Roberto
 */

#include "IKSolver.h"

#include "GLMatrix.h"
#include "newmat/newmat.h"


namespace {


struct GlobalJoint
{
	Point pos;
	Vector rotAxis;
	
	
	GlobalJoint() { }
	
	GlobalJoint(Point p, Vector a)
		: pos(p), rotAxis(a) { }
};


void fillGlobalValues(const JointTree &tree, std::vector<GlobalJoint> &endEffectorGlobal, std::vector<GlobalJoint> &jointGlobal)
{
	endEffectorGlobal.resize(tree.endEffectors().size());
	jointGlobal.reserve(tree.joints().size());
	
	std::vector<Joint*> jointStack;
	std::vector<GLMatrix> matrixStack;
	
	jointStack.push_back(tree.root());
	matrixStack.push_back(tree.root()->transformation());
	
	while(!jointStack.empty()) {
		Joint *j = jointStack.back();
		GLMatrix m = matrixStack.back();
		
		jointStack.pop_back();
		matrixStack.pop_back();
		
		for(std::vector<Joint*>::const_iterator i = j->children().begin(); i != j->children().end(); ++i) {
			jointStack.push_back(*i);
			matrixStack.push_back((*i)->transformation()*m);
		}
		
		Point p = m*Point();
		Vector v = m*j->rotAxis();
		int id = j->id();
		
		if(j->isEndEffector()) {
			endEffectorGlobal[id].pos = p;
			endEffectorGlobal[id].rotAxis = v;
		}
		else {
			jointGlobal[id].pos = p;
			jointGlobal[id].rotAxis = v;
		}
	}
}


}  // namespace


IKSolver::IKSolver(const JointTree &tree, double param)
	: _tree(tree), _param(param) { }

void IKSolver::solve() const
{
	const std::vector<Joint*> &endEffectors = _tree.endEffectors();
	const std::vector<Joint*> &joints = _tree.joints();
	std::vector<GlobalJoint> endEffectorGlobal;
	std::vector<GlobalJoint> jointGlobal;
	
	fillGlobalValues(_tree,endEffectorGlobal,jointGlobal);
	
	NEWMAT::Matrix J(3*endEffectors.size(),joints.size()),dp(3*endEffectors.size(),1);
	
	// Fill dp vector
	for(unsigned int i = 0; i < jointGlobal.size(); ++i) {
		Point &p = jointGlobal[i].pos;
		
		dp(3*i,0) = p.x;
		dp(3*i + 1,0) = p.y;
		dp(3*i + 2,0) = p.z;
	}
	
	// Fill J matrix with zeros
	for(int i = 0; i < J.Nrows(); ++i) {
		for(int j = 0; j < J.Ncols(); ++j) {
			J(i,j) = 0;
		}
	}
	
	// Fill J matrix with dp/dtheta values
	for(unsigned int i = 0; i < endEffectors.size(); ++i) {
		const Joint *j = endEffectors[i];
		const Joint *p = j;
		GlobalJoint &globalJoint = endEffectorGlobal[i];
		
		while((p = p->parent()) != 0) {
			GlobalJoint &globalParent = p->isEndEffector() ? endEffectorGlobal[p->id()] : jointGlobal[p->id()];
			Vector v = Vector::cross(globalParent.rotAxis,globalJoint.pos - globalParent.pos);
			
			J(3*i,p->id()) = v.x;
			J(3*i + 1,p->id()) = v.y;
			J(3*i + 2,p->id()) = v.z;
		}
	}
	
	NEWMAT::Matrix JT = J.t();
	NEWMAT::Matrix kI = NEWMAT::IdentityMatrix()*_param;
	
	// Calculate the damped least squares solution
	NEWMAT::Matrix dTheta = JT*(J*JT + kI).i()*dp;
	
	// Update joint angles
	for(int i = 0; i < dTheta.Ncols(); ++i) {
		joints[i]->setAngle(joints[i]->angle() + dTheta(i,0));
	}
}
