/*
 * World.cpp
 *
 *  Created on: Sep 24, 2010
 *      Author: Roberto
 */

#include "World.h"
#include "RectSplitter.h"
#include "Color.h"
#include "OpenGL.h"

#include <cstdlib>


World::World()
{
	int nrows = 1,ncols = 1;
	
	// Define blocks size.height
	for(int i = 0; i < nrows; ++i) {
		_rowHeights.push_back(80 + 100*double(std::rand())/RAND_MAX);
	}
	
	// Define blocks size.width
	for(int i = 0; i < ncols; ++i) {
		_colWidths.push_back(80 + 100*double(std::rand())/RAND_MAX);
	}
	
	Geo::RectSplitter splitter(Vector(20,20));
	
	_blockMatrix.reserve(nrows);
	
	double y = 0;
	
	for(int i = 0; i < nrows; ++i) {
		_blockMatrix.push_back(WorldRow());
		
		WorldRow &row = _blockMatrix.back();
		
		row.reserve(ncols);
		
		double x = 0;
		
		for(int j = 0; j < ncols; ++j) {
			Geo::Rectangle block = Geo::Rectangle(Point(x,y),Vector(_colWidths[j],_rowHeights[i]));
			
			row.push_back(splitter.split(block.inset(3)));
			
			x += _colWidths[j];
		}
		
		y += _rowHeights[i];
	}
}

const Vector World::size() const
{
	Vector size;

	for(std::vector<double>::const_iterator i = _colWidths.begin(); i != _colWidths.end(); ++i) {
		size.x += *i;
	}

	for(std::vector<double>::const_iterator i = _rowHeights.begin(); i != _rowHeights.end(); ++i) {
		size.y += *i;
	}
	
	return size;
}

void World::display()
{
	std::srand(0);
	
	glBegin(GL_QUADS); {
		for(WorldMatrix::const_iterator i = _blockMatrix.begin(); i != _blockMatrix.end(); ++i) {
			for(WorldRow::const_iterator j = i->begin(); j != i->end(); ++j) {
				for(WorldBlock::const_iterator i_rect = j->begin(); i_rect != j->end(); ++i_rect) {
					Color c;
					
					for(int j = 0; j < 3; ++j) {
						c.v[j] = 0.5 + 0.5*float(rand())/RAND_MAX;
					}
					
					OpenGL::color(c);
					
					const Geo::Rectangle &rect = *i_rect;
	
					OpenGL::vertex(rect.origin());
					OpenGL::vertex(rect.origin() + Vector(rect.size().x,0));
					OpenGL::vertex(rect.origin() + rect.size());
					OpenGL::vertex(rect.origin() + Vector(0,rect.size().y));
				}
			}
		}
	}
	glEnd();
}
