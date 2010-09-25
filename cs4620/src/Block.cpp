/*
 * Block.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#include "Block.h"
#include "Color.h"
#include "OpenGL.h"

#include <cstdlib>


Block::Block(const Geo::Rectangle &base, Geo::RectSplitter &splitter)
	: _base(base)
{
	std::vector<Geo::Rectangle> bases = splitter.split(_base);
	
	_buildings.reserve(bases.size());
	
	for(std::vector<Geo::Rectangle>::const_iterator i_bases = bases.begin(); i_bases != bases.end() ; ++i_bases) {
		Geo::Rectangle base = i_bases->inset(3);
		
		_buildings.push_back(Building(base));
	}
}

void Block::display()
{
	for(std::vector<Building>::iterator i = _buildings.begin(); i != _buildings.end(); ++i) {
		OpenGL::color(Color::gray());
		
		glBegin(GL_QUADS); {
			OpenGL::vertex(_base.origin());
			OpenGL::vertex(_base.origin() + Vector(_base.size().x,0));
			OpenGL::vertex(_base.origin() + _base.size());
			OpenGL::vertex(_base.origin() + Vector(0,_base.size().y));
		}
		glEnd();
		
		Color c;
		
		for(int j = 0; j < 3; ++j) {
			c.v[j] = 0.5 + 0.5*float(rand())/RAND_MAX;
		}
		
		OpenGL::color(c);
		
		i->display();
	}
}
