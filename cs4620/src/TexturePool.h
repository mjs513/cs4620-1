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
	
	GLuint getRandomSidewalk() const;
	GLuint getRandomWall() const;
	GLuint getRandomWindow() const;
	GLuint getRoad() const;

private:
	GLuint _generateTexture( const std::string &name, bool wrap );
	
private:
	std::vector<GLuint> _sidewalks;
	std::vector<GLuint> _walls;
	std::vector<GLuint> _windows;
	GLuint _road;
};


#endif /* TEXTUREPOOL_H_ */
