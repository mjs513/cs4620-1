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
	for(int i = 0; i <= 4; ++i) {
		std::string name = "wallTexture";
		name += i;
		name += ".jpg";
		
		QImage image;
		
		if(!image.load(name.c_str())) {
			image = QImage(512,512,QImage::Format_RGB32);
		}

		image = QGLWidget::convertToGLFormat(image);
		
		GLuint tex;
		
		glGenTextures(1,&tex);
		glBindTexture(GL_TEXTURE_2D,tex);
		
		glTexImage2D(GL_TEXTURE_2D,0,3,image.width(),image.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,image.bits());
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		_walls.push_back(tex);
	}
}

GLuint TexturePool::getRandomWall() const
{
	return _walls[rand()%_walls.size()];
}
