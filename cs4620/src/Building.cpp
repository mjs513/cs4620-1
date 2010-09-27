/*
 * Building.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#include "Building.h"
#include "OpenGL.h"
#include "Random.h"

#include <cstdlib>
#include <iostream>


Building::Building(const Geo::Rectangle &base, const TexturePool &texPool)
	: _base(base), _texPool(&texPool)
{
	RandomDouble drand;
	double r = drand.rand();

	// 30% of small buildings
	if(r < 0.3) {
		_height = RandomNormal(20,10).rand();
	}

	// 68% of medium buildings
	else if(r < 0.98) {
		_height = RandomNormal(50,30).rand();
	}

	// 2% of very high buildings
	else {
		_height = RandomNormal(90,40).rand();
	}
	
	double floorHeight = RandomDouble(2.8,3.5).rand();
	double columnX = RandomDouble(1.7,2.5).rand();
	double columnY = RandomDouble(1.7,2.5).rand();
	
	_numberOfFloors = int(_height/floorHeight);
	/// TODO
	_numberOfColumnsX = _base.size().x / 2;
	_numberOfColumnsY = _base.size().y / 2;

	setBoundingSphere(BoundingSphere::createWithAABox(base.origin(),base.origin() + base.size() + Vector(0,0,_height)));
}

const Geo::Rectangle& Building::base() const
{
	return _base;
}

double Building::height() const
{
	return _height;
}


namespace {


struct Face
{
	enum Which
	{
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		COUNT
	};
};


Vector normalForFace(Face::Which face)
{
	switch(face) {
		case Face::RIGHT: return Vector(1,0,0);
		case Face::LEFT: return Vector(-1,0,0);
		case Face::FRONT: return Vector(0,1,0);
		case Face::BACK: return Vector(0,-1,0);
		default: { }
	}

	return Vector();
}

unsigned int encodeWindowTile(int face, int floor, int column)
{
	// face: 2 bits (0x3)
	// floor: 15 bits (0x7fff)
	// column: 15 bits (0x7fff)
	
	face = (face & 0x3) << 30;
	floor = (floor & 0x7fff) << 15;
	column = column & 0x7fff;
	
	return face | floor | column;
}

void decodeWindowTile(unsigned int encoded, int &face, int &floor, int &column)
{
	column = encoded & 0x7fff;
	
	encoded >>= 15;
	
	floor = encoded & 0x7fff;
	
	encoded >>= 15;
	
	face = encoded & 0x3;
}


} // namespace


void Building::draw(const Frustum &frustum)
{
	glPushMatrix();
	
	OpenGL::translate(_base.origin() + 0.5*(_base.size() + Vector(0,0,_height)));
	OpenGL::scale(0.5*(_base.size() + Vector(0,0,_height)));
	
	glBindTexture(GL_TEXTURE_2D,_texPool->getRandomWall());

	glBegin(GL_QUADS); {
		// Top Face
		OpenGL::normal(Vector(0,0,1));
		glTexCoord2f(0,0); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1,0); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1,1); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0,1); glVertex3f(-1.0f,  1.0f,  1.0f);

		// Right face
		OpenGL::normal(Vector(-1,0,0));
		glTexCoord2f(0,0); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1,0); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(1,1); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0,1); glVertex3f( 1.0f, -1.0f,  1.0f);

		// Left Face
		OpenGL::normal(Vector(-1,0,0));
		glTexCoord2f(0,0); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0,1); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1,1); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1,0); glVertex3f(-1.0f,  1.0f, -1.0f);

		// Front Face
		OpenGL::normal(Vector(0,1,0));
		glTexCoord2f(0,0); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0,1); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1,1); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1,0); glVertex3f( 1.0f,  1.0f, -1.0f);

		// Back Face
		OpenGL::normal(Vector(0,-1,0));
		glTexCoord2f(0,0); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1,0); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1,1); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(0,1); glVertex3f(-1.0f, -1.0f,  1.0f);
	}
	glEnd();
	
	std::vector< std::vector<unsigned int> > windowTypes(_texPool->windows().size());
	
	// Assign and group windows by texture
	for(int face = 0; face < Face::COUNT; ++face) {
		for(int floor = 0; floor < _numberOfFloors; ++floor) {
			int nColumns = 0;
			
			if((face == Face::RIGHT) || (face == Face::LEFT)) {
				nColumns = _numberOfColumnsY;
			}
			else {
				nColumns = _numberOfColumnsX;
			}
			
			for(int column = 0; column < nColumns; ++column) {
				unsigned int encoded = encodeWindowTile(face,floor,column);
				
				windowTypes[std::rand()%windowTypes.size()].push_back(encoded);
			}
		}
	}

	const float e = 0.001;  // Negligible value for discerning window texture from wall texture
	const float b = 0.01;  // Border around window quad to ensure it will always blend with building, not background
	
	// Window dimensions
	double floorStep = 2.0/_numberOfFloors;
	double xStep = 2.0/_numberOfColumnsX;
	double yStep = 2.0/_numberOfColumnsY;
	
	for(unsigned int i = 0; i < windowTypes.size(); ++i) {
		GLuint texture = _texPool->windows()[i];
		const std::vector<unsigned int> &tiles = windowTypes[i];
		
		glBindTexture(GL_TEXTURE_2D,texture);
		
		glBegin(GL_QUADS); {
			for(std::vector<unsigned int>::const_iterator i_tiles = tiles.begin(); i_tiles != tiles.end(); ++i_tiles) {
				int face,floor,column;
				
				decodeWindowTile(*i_tiles,face,floor,column);
				
				double minH = -1 + floor*floorStep,maxH = minH + floorStep;
				
				switch(face) {
				case Face::FRONT: {
					double minX = -1 + column*xStep,maxX = minX + xStep;
					
					glNormal3d(0,1,0);
					
					glTexCoord2d(0,0);
					glVertex3d(minX + b,1 + e,minH + b);

					glTexCoord2d(0,1);
					glVertex3d(minX + b,1 + e,maxH - b);

					glTexCoord2d(1,1);
					glVertex3d(maxX - b,1 + e,maxH - b);

					glTexCoord2d(1,0);
					glVertex3d(maxX - b,1 + e,minH + b);
					
					break;
				}

				case Face::BACK: {
					double minX = -1 + column*xStep,maxX = minX + xStep;
					
					glNormal3d(0,-1,0);

					glTexCoord2d(0,0);
					glVertex3d(minX + b,-1 - e,minH + b);

					glTexCoord2d(1,0);
					glVertex3d(maxX - b,-1 - e,minH + b);

					glTexCoord2d(1,1);
					glVertex3d(maxX - b,-1 - e,maxH - b);
					
					glTexCoord2d(0,1);
					glVertex3d(minX + b,-1 - e,maxH - b);
					
					break;
				}

				case Face::RIGHT: {
					double minY = -1 + column*yStep,maxY = minY + yStep;
					
					glNormal3d(1,0,0);
					
					glTexCoord2d(0,0);
					glVertex3d(1 + e,minY + b,minH + b);

					glTexCoord2d(1,0);
					glVertex3d(1 + e,maxY - b,minH + b);

					glTexCoord2d(1,1);
					glVertex3d(1 + e,maxY - b,maxH - b);

					glTexCoord2d(0,1);
					glVertex3d(1 + e,minY + b,maxH - b);
					
					break;
				}

				case Face::LEFT: {
					double minY = -1 + column*yStep,maxY = minY + yStep;
					
					glNormal3d(-1,0,0);
					
					glTexCoord2d(0,0);
					glVertex3d(-1 - e,minY + b,minH + b);

					glTexCoord2d(0,1);
					glVertex3d(-1 - e,minY + b,maxH - b);

					glTexCoord2d(1,1);
					glVertex3d(-1 - e,maxY - b,maxH - b);

					glTexCoord2d(1,0);
					glVertex3d(-1 - e,maxY - b,minH + b);
					
					break;
				}
				}
			}
		}
		glEnd();
	}
	/*
	// Front and back windows
	// Condition (minH < 1 - step/2) to be careful with floating point errors causing an extra floor to show up
	for(double minH = -1; minH < 1 - floorStep*0.5; minH += floorStep) {
		OpenGL::normal(Vector(0,1,0));
		
		for(double minX = -1; minX < 1 - xStep*0.5; minX += xStep) {
			double maxH = minH + floorStep;
			double maxX = minX + xStep;

			glBegin(GL_QUADS); {
				glTexCoord2d(0,0);
				glVertex3d(minX,1 + e,minH);

				glTexCoord2d(0,1);
				glVertex3d(minX,1 + e,maxH);

				glTexCoord2d(1,1);
				glVertex3d(maxX,1 + e,maxH);

				glTexCoord2d(1,0);
				glVertex3d(maxX,1 + e,minH);
			}
			glEnd();
		}

		OpenGL::normal(Vector(0,-1,0));
		
		for(double minX = -1; minX < 1 - xStep*0.5; minX += xStep) {
			double maxH = minH + floorStep;
			double maxX = minX + xStep;
			
			_texPool->getRandomWindow();
			
			glBegin(GL_QUADS); {
				glTexCoord2d(0,0);
				glVertex3d(minX,-1 - e,minH);

				glTexCoord2d(1,0);
				glVertex3d(maxX,-1 - e,minH);

				glTexCoord2d(1,1);
				glVertex3d(maxX,-1 - e,maxH);
				
				glTexCoord2d(0,1);
				glVertex3d(minX,-1 - e,maxH);
			}
			glEnd();
		}
	}*/
	
	glPopMatrix();
}

bool Building::testFrustum() const
{
	return false;
}
