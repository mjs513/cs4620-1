/*
 * JointTree.h
 *
 *  Created on: Oct 22, 2010
 *      Author: Roberto
 */

#ifndef JOINTTREE_H_
#define JOINTTREE_H_


#include "Joint.h"

#include <vector>


class JointTree
{
public:
	JointTree(Joint *_root);
	
	Joint* root() const;

	const std::vector<Joint*>& joints() const;
	const std::vector<Joint*>& endEffectors() const;
	
private:
	void _iterate(Joint *joint);
	
private:
	Joint *_root;
	std::vector<Joint*> _joints;
	std::vector<Joint*> _endEffectors;
};


#endif /* JOINTTREE_H_ */
