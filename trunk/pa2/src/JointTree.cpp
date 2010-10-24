/*
 * JointTree.cpp
 *
 *  Created on: Oct 22, 2010
 *      Author: Roberto
 */

#include "JointTree.h"

#include <algorithm>


JointTree::JointTree(Joint *root)
	: _root(root)
{
	_iterate(_root);
}

Joint* JointTree::root() const
{
	return _root;
}

const std::vector<Joint*>& JointTree::joints() const
{
	return _joints;
}

void JointTree::_iterate(Joint *joint)
{
	joint->setId(_joints.size());
	
	_joints.push_back(joint);
	
	for(std::vector<Joint*>::const_iterator i = joint->children().begin(); i != joint->children().end(); ++i) {
		_iterate(*i);
	}
}
