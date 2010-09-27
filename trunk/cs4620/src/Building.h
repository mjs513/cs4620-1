/*
 * Building.h
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#ifndef BUILDING_H_
#define BUILDING_H_


#include "Rectangle.h"
#include "OpenGL.h"
#include "Displayable.h"
#include "TexturePool.h"


class Building : public Displayable
{
public:
	Building(const Geo::Rectangle &base, const TexturePool &texPool);
	
	const Geo::Rectangle& base() const;
	double height() const;
	
	void draw(const Frustum &frustum);
	
	bool testFrustum() const;

	void loadTextures();
	GLuint _buildingWallTexture;

private:
	Geo::Rectangle _base;
	double _height;
	int _numberOfFloors,_numberOfColumnsX,_numberOfColumnsY;

	const TexturePool *_texPool;
};

#endif /* BUILDING_H_ */
