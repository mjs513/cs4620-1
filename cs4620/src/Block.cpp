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
#include <QtGui>
#include <QtOpenGL/QtOpenGL>


Block::Block(const Geo::Rectangle &base, Geo::RectSplitter &splitter, const TexturePool &texturePool)
	: _base(base), _displayList(0), _sidewalkTexture(0)
{
	// Pick a sidewalk texture for this block
	_sidewalkTexture = texturePool.getRandomSidewalk();

	std::vector<Geo::Rectangle> bases = splitter.split(_base);
	
	_buildings.reserve(bases.size());
	
	// Generate buildings for this block, attributing a random wall and window texture for each building
	for(std::vector<Geo::Rectangle>::const_iterator i_bases = bases.begin(); i_bases != bases.end() ; ++i_bases) {
		Geo::Rectangle base = i_bases->inset(3);
		
		_buildings.push_back(Building(base, texturePool.getRandomWall(), texturePool.getRandomWindow()));
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
				OpenGL::color(Color(0.8,0.8,0.8));

				glBindTexture(GL_TEXTURE_2D, _sidewalkTexture);
				
				glBegin(GL_QUADS); {
					OpenGL::normal(Vector(0,0,1));

					OpenGL::texture2(Point());
					OpenGL::vertex(_base.origin());

					OpenGL::texture2(Point(floor(0.33*_base.size().x),0));
					OpenGL::vertex(_base.origin() + Vector(_base.size().x,0));

					OpenGL::texture2(Point(floor(0.33*_base.size().x),floor(0.33*_base.size().y)));
					OpenGL::vertex(_base.origin() + _base.size());

					OpenGL::texture2(Point(0,floor(0.33*_base.size().y)));
					OpenGL::vertex(_base.origin() + Vector(0,_base.size().y));
				}
				glEnd();
				
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
