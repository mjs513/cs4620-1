/*
 * Character.cpp
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#include "Character.h"


Character::Character()
	: _root(0) { }

Character::~Character()
{
	delete _root;
}

void Character::update(GLWidget &glWidget) { }

Joint* Character::root()
{
	return _root;
}
