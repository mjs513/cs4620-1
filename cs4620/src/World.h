/*
 * World.h
 *
 *  Created on: Sep 24, 2010
 *      Author: Roberto
 */

#ifndef WORLD_H_
#define WORLD_H_


#include <vector>

#include "Building.h"
#include "Block.h"
#include "Displayable.h"


class World : public Displayable
{
private:
	typedef std::vector<Block> WorldRow;
	typedef std::vector<WorldRow> WorldMatrix;
	
public:
	World();
	
	const Vector size() const;
	
	void draw(const Frustum &frustum);
	
	bool testFrustum() const;
	
private:
	std::vector<double> _rowHeights,_colWidths;
	WorldMatrix _blockMatrix;
};


#endif /* WORLD_H_ */
