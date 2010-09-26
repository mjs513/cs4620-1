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

#include <QtOpenGL/QtOpenGL>

#include <cstdlib>


World::World()
	: _buildingWallTexture(0)
{
	_loadTextures();

	int nrows = 6,ncols = 6;
	
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
			
			row.push_back(Block(blockRect, splitter, _buildingWallTexture));
			
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


void World::_loadTextures()
{
	QImage buildingWall_image;
	bool wrap = true;

	//if (!buildingWall_image.load("buildingWall64_texture.bmp")) {
	if (!buildingWall_image.load("testa.bmp")) {
		buildingWall_image = QImage(16, 16, QImage::Format_RGB32);
		buildingWall_image.fill(Qt::red);
	}

	buildingWall_image = QGLWidget::convertToGLFormat(buildingWall_image);
	glGenTextures(1, &_buildingWallTexture);
	glBindTexture(GL_TEXTURE_2D, _buildingWallTexture);
	printf("World::loadTextures: _buildingWallTexture = %d\n", _buildingWallTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, buildingWall_image.width(), buildingWall_image.height(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, buildingWall_image.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP );
}

bool World::testFrustum() const
{
	return false;
}
