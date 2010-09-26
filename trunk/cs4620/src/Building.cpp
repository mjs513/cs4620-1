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
#include <cstdio>


Building::Building(const Geo::Rectangle &base, GLuint texture)
	: _base(base)
{
	_texture = texture;

	RandomDouble drand;
	double r = drand.rand();

	if(r < 0.3) {
		RandomNormal nrand(20,10);

		_height = nrand.rand();
	}
	else if(r < 0.98) {
		RandomNormal nrand(50,30);

		_height = nrand.rand();
	}
	else {
		RandomNormal nrand(90,40);
		
		_height = nrand.rand();
	}
	
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

void Building::draw(const Frustum &frustum)
{
	glPushMatrix();
	
	OpenGL::translate(_base.origin() + 0.5*(_base.size() + Vector(0,0,_height)));
	OpenGL::scale(0.5*(_base.size() + Vector(0,0,_height)));
	
	glBegin(GL_QUADS); {
		// Back Face
		OpenGL::normal(Vector(0,-1,0));
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		
		// Top Face
		OpenGL::normal(Vector(0,0,1));
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

		// Right face
		OpenGL::normal(Vector(-1,0,0));
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

		// Left Face
		OpenGL::normal(Vector(-1,0,0));
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

		// Front Face
		OpenGL::normal(Vector(0,1,0));
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		
	}
	glEnd();
	
	glPopMatrix();
}

bool Building::testFrustum() const
{
	return false;
}
