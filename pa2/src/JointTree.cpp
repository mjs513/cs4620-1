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

const std::vector<Joint*>& JointTree::endEffectors() const
{
	return _endEffectors;
}

void JointTree::_iterate(Joint *joint)
{
	int id;
	
	if(joint->isEndEffector()) {
		_endEffectors.push_back(joint);
		id = _endEffectors.size();
	}
	else {
		_joints.push_back(joint);
		id = _joints.size();
	}
	
	joint->setId(id);
	
	for(std::vector<Joint*>::const_iterator i = joint->children().begin(); i != joint->children().end(); ++i) {
		_iterate(*i);
	}
}
