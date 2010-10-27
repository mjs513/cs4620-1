/*
 * HumanHand.h
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#ifndef HUMANHAND_H_
#define HUMANHAND_H_


#include "Character.h"


class HumanHand : public Character
{
public:
	HumanHand();

	void update(GLWidget &glWidget);
};


#endif /* HUMANHAND_H_ */
