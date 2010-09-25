/*
 * Block.h
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#ifndef BLOCK_H_
#define BLOCK_H_


#include "Rectangle.h"
#include "RectSplitter.h"
#include "Building.h"

#include <vector>


class Block
{
public:
	Block(const Geo::Rectangle &base, Geo::RectSplitter &splitter);

	void display();
	
private:
	Geo::Rectangle _base;
	std::vector<Building> _buildings;
};


#endif /* BLOCK_H_ */
