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

/*
TexturePool::TexturePool()
{
	for(int i = 0; i <= 4; ++i) {
		std::string name = "wallTexture";
		name += i;
		name += ".bmp";

		QImage image(name.c_str());

		image = QGLWidget::convertToGLFormat(image);

		GLuint tex;

		glGenTextures(1,&tex);
		glBindTexture(GL_TEXTURE_2D,tex);

		glTexImage2D(GL_TEXTURE_2D,0,3,image.width(),image.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,image.bits());

		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		_walls.push_back(tex);
	}
}
*/

TexturePool::TexturePool()
{
	// Loads all textures
	for(int i = 0; i <= 4; ++i) {
		std::stringstream ss;

		ss << "wallTexture" << i << ".bmp";

		_walls.push_back( _generateTexture( ss.str(), true ) );
	}
	for(int i = 0; i <= 2; ++i) {
		std::stringstream ss;

		ss << "wallBricks" << i << "_128.png";

		_walls.push_back( _generateTexture( ss.str(), true ) );
	}
	for(int i = 1; i <= 6; ++i) {
		std::stringstream ss;

		ss << "window" << i << ".png";

		_windows.push_back( _generateTexture( ss.str(), true ) );
	}

	_sidewalks.push_back( _generateTexture( "sidewalk.bmp", true ) );

}

GLuint TexturePool::_generateTexture( const std::string &name, bool wrap ) {

	QImage image;

	printf( "Loading texture: %s\n", name.c_str());

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

	glTexImage2D(GL_TEXTURE_2D,0,3,image.width(),image.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,image.bits());

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP );

	return tex;
}

/*
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
*/
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
