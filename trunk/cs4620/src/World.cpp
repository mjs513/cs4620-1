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
	int nrows = 5,ncols = 5;
	
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
			
			row.push_back(Block(blockRect,splitter));
			
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

	OpenGL::color(Color(0.2,0.2,0.2));
	
	Geo::Rectangle base(Point(),this->size());

	glPushMatrix();
	
	OpenGL::translate(Vector(0,0,-0.01));
	
	base.draw();
	
	glPopMatrix();
	
	for(WorldMatrix::iterator i_matrix = _blockMatrix.begin(); i_matrix != _blockMatrix.end(); ++i_matrix) {
		for(WorldRow::iterator i_row = i_matrix->begin(); i_row != i_matrix->end(); ++i_row) {
			i_row->display(frustum);
		}
	}
}

bool World::testFrustum() const
{
	return false;
}
