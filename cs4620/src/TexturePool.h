/*
 * TexturePool.h
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#ifndef TEXTUREPOOL_H_
#define TEXTUREPOOL_H_


#include <vector>

#include "OpenGL.h"


class TexturePool
{
public:
	TexturePool();
	
	GLuint getRandomWall() const;
	GLuint getRandomWindow() const;
	GLuint getRoad() const;
	
private:
	std::vector<GLuint> _walls;
	std::vector<GLuint> _windows;
	GLuint _road;
};


#endif /* TEXTUREPOOL_H_ */
