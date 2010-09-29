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
#include "TexturePool.h"

#include <vector>


class Block : public Displayable
{
public:
	Block(const Geo::Rectangle &base, Geo::RectSplitter &splitter, const TexturePool &texturePool);
	~Block();

	void cycleWindowMode();
	Building::WindowMode::Which windowMode() const;
	
	void clearDisplayList();

	void draw(const Frustum &frustum);

	bool testFrustum() const;
	
private:
	Geo::Rectangle _base;
	std::vector<Building> _buildings;
	GLuint _displayList;
	const TexturePool *_texPool;
	int _randSeed;
};


#endif /* BLOCK_H_ */
