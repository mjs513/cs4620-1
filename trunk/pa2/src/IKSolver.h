/*
 * IKSolver.h
 *
 *  Created on: Oct 22, 2010
 *      Author: Roberto
 */

#ifndef IKSOLVER_H_
#define IKSOLVER_H_


#include "JointTree.h"

#include "newmat/newmat.h"

#include <map>


class IKSolver
{
public:
	enum Method {
		TRANSPOSE,
		DAMPED_LEAST_SQUARES,
		COUNT
	};

	IKSolver(Joint *root, double param = 1e-3);
	
	void solve(const std::map<Joint*,Point> &endEffectorsMotion, Method method);
	
private:
	Joint *_root;
	double _param;

	NEWMAT::Matrix _transposeMethod(const NEWMAT::Matrix &Jacobian, const NEWMAT::Matrix &dp);
	NEWMAT::Matrix _dampedLeastSquaresMethod(const NEWMAT::Matrix &Jacobian, const NEWMAT::Matrix &dp);
};


#endif /* IKSOLVER_H_ */
