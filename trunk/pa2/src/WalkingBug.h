/*
 * WalkingBug.h
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#ifndef WALKINGBUG_H_
#define WALKINGBUG_H_


#include "Character.h"


class WalkingBug : public Character
{
public:
	WalkingBug();
	
	void update(GLWidget &glWidget);
	
private:
};

#endif /* WALKINGBUG_H_ */
