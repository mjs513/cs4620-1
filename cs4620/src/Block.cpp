/*
 * Block.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#include "Block.h"
#include "Color.h"
#include "OpenGL.h"
#include "Random.h"

#include <cstdlib>
#include <iostream>


Block::Block(const Geo::Rectangle &base, Geo::RectSplitter &splitter, GLuint texture)
	: _base(base), _displayList(0)
{
	std::vector<Geo::Rectangle> bases = splitter.split(_base);
	
	_buildings.reserve(bases.size());
	
	for(std::vector<Geo::Rectangle>::const_iterator i_bases = bases.begin(); i_bases != bases.end() ; ++i_bases) {
		Geo::Rectangle base = i_bases->inset(3);
		
		_buildings.push_back(Building(base, texture));
	}
	
	setBoundingSphere(BoundingSphere::createWithAABox(_base.origin(),_base.origin() + _base.size()));
}

Block::~Block()
{
	if(_displayList >= 0) {
		glDeleteLists(_displayList,1);
	}
}

void Block::draw(const Frustum &frustum)
{
	if(!_displayList) {
		_displayList = glGenLists(1);

		glNewList(_displayList,GL_COMPILE_AND_EXECUTE); {
			for(std::vector<Building>::iterator i = _buildings.begin(); i != _buildings.end(); ++i) {
				OpenGL::color(Color::gray());
				
				_base.draw();
				
				RandomDouble drand(0.15,0.85);
				Color c;
				
				for(int j = 0; j < 3; ++j) {
					c.v[j] = drand.rand();
				}
				
				OpenGL::color(c);
				
				i->draw(frustum);
			}
		}
		glEndList();
	}
	else {
		glCallList(_displayList);
	}
}

bool Block::testFrustum() const
{
	return false;
}
