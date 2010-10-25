/*
 * Character.h
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_


#include "Joint.h"


class GLWidget;


class Character
{
public:
	Character();
	virtual ~Character();
	
	virtual void update(GLWidget &glWidget);
	
	Joint* root();
	
protected:
	Joint *_root;
};

#endif /* CHARACTER_H_ */
