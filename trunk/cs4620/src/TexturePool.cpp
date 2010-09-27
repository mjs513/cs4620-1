/*
 * TexturePool.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#include "TexturePool.h"

#include <string>
#include <sstream>
#include <QtGui>
#include <QtOpenGL/QtOpenGL>

#include <cstdlib>


TexturePool::TexturePool()
{
	// Loads all textures
	for(int i = 0; i <= 4; ++i) {
		std::stringstream ss;

		ss << "wallTexture" << i << ".bmp";

		_walls.push_back( _generateTexture( ss.str(), true ) );
	}
	/*
	for(int i = 0; i <= 2; ++i) {
		std::stringstream ss;

		ss << "wallBricks" << i << "_128.png";

		_walls.push_back( _generateTexture( ss.str(), true ) );
	}
	*/
	for(int i = 1; i <= 6; ++i) {
		std::stringstream ss;

		ss << "window" << i << ".png";

		_windows.push_back( _generateTexture( ss.str(), true ) );
	}
	
	_sidewalks.push_back( _generateTexture( "sidewalk.bmp", true ) );
}

GLuint TexturePool::_generateTexture( const std::string &name, bool wrap )
{
	QImage image;

	// Try to load texture image. If it fails, then fill it with something arbitrary
	if (!image.load( ("textures/" + name).c_str() )) {
		printf("Failed to load texture: %s\n", name.c_str());
		image = QImage(16, 16, QImage::Format_RGB32);
		image.fill(Qt::red);
	}
	
	image = QGLWidget::convertToGLFormat(image);

	GLuint tex;

	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D,0,4,image.width(),image.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,image.bits());

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP );

	return tex;
}

GLuint TexturePool::getRandomSidewalk() const
{
	return _sidewalks[rand()%_sidewalks.size()];
}

GLuint TexturePool::getRandomWall() const
{
	return _walls[rand()%_walls.size()];
}

GLuint TexturePool::getRandomWindow() const
{
	return _windows[rand()%_windows.size()];
}

const std::vector<GLuint>& TexturePool::sidewalks() const
{
	return _sidewalks;
}

const std::vector<GLuint>& TexturePool::walls() const
{
	return _walls;
}

const std::vector<GLuint>&TexturePool:: windows() const
{
	return _windows;
}
