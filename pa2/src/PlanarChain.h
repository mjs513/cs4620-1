/*
 * PlanarChain.h
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#ifndef PLANARCHAIN_H_
#define PLANARCHAIN_H_


#include "Character.h"

#include <QtGui>

class PlanarChain : public Character
{
public:
	PlanarChain();

	void update(GLWidget &glWidget);
};


#endif /* PLANARCHAIN_H_ */
