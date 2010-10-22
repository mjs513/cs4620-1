/*
 * IKSolver.h
 *
 *  Created on: Oct 22, 2010
 *      Author: Roberto
 */

#ifndef IKSOLVER_H_
#define IKSOLVER_H_


#include "JointTree.h"


class IKSolver
{
public:
	IKSolver(const JointTree &tree, double param = 1e-3);
	
	void solve() const;
	
private:
	const JointTree &_tree;
	double _param;
};


#endif /* IKSOLVER_H_ */
