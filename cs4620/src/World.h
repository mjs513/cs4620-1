/*
 * World.h
 *
 *  Created on: Sep 24, 2010
 *      Author: Roberto
 */

#ifndef WORLD_H_
#define WORLD_H_


#include <vector>

#include "Rectangle.h"


class World
{
private:
	typedef std::vector<Geo::Rectangle> WorldBlock;
	typedef std::vector<WorldBlock> WorldRow;
	typedef std::vector<WorldRow> WorldMatrix;
	
public:
	World();
	
	void display();
	
private:
	std::vector<double> _rowHeights,_colWidths;
	WorldMatrix _blockMatrix;
};


#endif /* WORLD_H_ */
