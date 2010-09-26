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
#include "Displayable.h"

#include <vector>


class Block : public Displayable
{
public:
	Block(const Geo::Rectangle &base, Geo::RectSplitter &splitter);

	void draw(const Frustum &frustum);

	bool testFrustum() const;
	
private:
	Geo::Rectangle _base;
	std::vector<Building> _buildings;
};


#endif /* BLOCK_H_ */
