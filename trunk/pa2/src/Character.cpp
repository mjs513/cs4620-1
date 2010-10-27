/*
 * Character.cpp
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#include "Character.h"

#include <cmath>


Character::Character()
	: _root(0) { }

Character::~Character()
{
	delete _root;
}

void Character::update(GLWidget &glWidget) { }

void Character::startAnimation()
{
	_animationStartTime = QTime::currentTime();
	
	resetState();
}

const QTime& Character::animationStartTime()
{
	return _animationStartTime;
}

double Character::animationCycleTime(double cycleDuration)
{
	return std::fmod(double(_animationStartTime.msecsTo(QTime::currentTime()))/1000,cycleDuration)/cycleDuration;
}


namespace {


void resetJointState(Joint *j)
{
	j->setAngle(0);
	
	for(std::vector<Joint*>::const_iterator i = j->children().begin(); i != j->children().end(); ++i) {
		resetJointState(*i);
	}
}


}  // namespace


void Character::resetState()
{
	resetJointState(_root);
}

Joint* Character::root()
{
	return _root;
}
