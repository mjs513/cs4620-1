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
	: _base(base), _displayList(0), _texPool(&texturePool), _randSeed(std::rand())
{
	std::vector<Geo::Rectangle> bases = splitter.split(_base);
	
	_buildings.reserve(bases.size());
	
	double maxHeight = -1;
	
	// Generate buildings for this block, attributing a random wall and window texture for each building
	for(std::vector<Geo::Rectangle>::const_iterator i_bases = bases.begin(); i_bases != bases.end() ; ++i_bases) {
		Geo::Rectangle base = i_bases->inset(3);
		
		_buildings.push_back(Building(base,texturePool));
		
		if(_buildings.back().height() > maxHeight) {
			maxHeight = _buildings.back().height();
		}
	}
	
	setBoundingSphere(BoundingSphere::createWithAABox(_base.origin(),_base.origin() + _base.size() + Vector(0,0,maxHeight)));
}

Block::~Block()
{
	clearDisplayList();
}

void Block::cycleWindowMode()
{
	clearDisplayList();
	
	for(std::vector<Building>::iterator i = _buildings.begin(); i != _buildings.end(); ++i) {
		i->cycleWindowMode();
	}
}

Building::WindowMode::Which Block::windowMode() const
{
	return _buildings.front().windowMode();
}

void Block::clearDisplayList()
{
	if(_displayList >= 0) {
		glDeleteLists(_displayList,1);
	}
}

void Block::draw(const Frustum &frustum)
{
	// Make all rand() deterministic after this point for drawing the same on all frames
	std::srand(_randSeed);
	
	if(!_displayList) {
		//_displayList = glGenLists(1);
		
		//glNewList(_displayList,GL_COMPILE_AND_EXECUTE); {
			OpenGL::color(Color(0.8,0.8,0.8));

			glBindTexture(GL_TEXTURE_2D,_texPool->getRandomSidewalk());
			
			// Draw sidewalk
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
			
			// Draw buildings
			for(std::vector<Building>::iterator i = _buildings.begin(); i != _buildings.end(); ++i) {
				i->draw(frustum);
			}
		//}
		//glEndList();
	}
	else {
		glCallList(_displayList);
	}
}

bool Block::testFrustum() const
{
	return true;
}
