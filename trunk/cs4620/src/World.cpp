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
	int nrows = 15,ncols = 15;
	
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
			Geo::Rectangle blockRect = Geo::Rectangle(Point(x,y),Vector(_colWidths[j],_rowHeights[i])).inset(6);
			
			row.push_back(Block(blockRect, splitter, _texturePool));
			
			x += _colWidths[j];
		}
		
		y += _rowHeights[i];
	}
	
	setBoundingSphere(BoundingSphere::createWithAABox(Point(),Point() + size()));
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

void World::draw(const Frustum &frustum)
{
	std::srand(0);
	
	Geo::Rectangle base(Point(),this->size());
	
	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT);

	glDisable(GL_TEXTURE_2D);
	
	// Make road lower than sidewalk
	OpenGL::translate(Vector(0,0,-0.01));

	OpenGL::color(Color(1, 0, 0));

	glBegin(GL_QUADS); {
		OpenGL::normal(Vector(0,0,1));
		
		OpenGL::vertex(base.origin());
		OpenGL::vertex(base.origin() + Vector(base.size().x,0));
		OpenGL::vertex(base.origin() + base.size());
		OpenGL::vertex(base.origin() + Vector(0,base.size().y));
	}
	glEnd();

	glPopAttrib();
	glPopMatrix();
	
	// Display all blocks
	for(WorldMatrix::iterator i_matrix = _blockMatrix.begin(); i_matrix != _blockMatrix.end(); ++i_matrix) {
		for(WorldRow::iterator i_row = i_matrix->begin(); i_row != i_matrix->end(); ++i_row) {
			i_row->display(frustum);
		}
	}
}

bool World::testFrustum() const
{
	return true;
}
