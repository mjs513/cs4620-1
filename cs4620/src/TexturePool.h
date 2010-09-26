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
	
	
private:
	std::vector<GLuint> _wallTextures;
	std::vector<GLuint> _windowTextures;
	GLuint _roadTexture;
};


#endif /* TEXTUREPOOL_H_ */
