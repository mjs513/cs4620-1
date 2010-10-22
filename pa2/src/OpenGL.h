/*
 * OpenGL.h
 *
 *  Created on: 30/03/2010
 *      Author: user
 */

#ifndef OPENGL_H_
#define OPENGL_H_

#include "GLMatrix.h"
#include "Point.h"
#include "Vector.h"
#include "Color.h"

#include <GL/gl.h>
#include <GL/glu.h>


namespace OpenGL {


void lightPosition(GLenum light, const Vector &v);
void lightPosition(GLenum light, const Point &p);

void lightColor(GLenum light, GLenum param, const Color &c);

void fogColor(const Color &c);

void color(const Color &c);
void clearColor(const Color &c);

void vertex(const Point &p);
void vertex2(const Point &p);
void vertex3(const Point &p);
void vertex4(const Point &p);

void texture(const Point &p);
void texture2(const Point &p);
void texture3(const Point &p);

void normal(const Vector &v);

void translate(const Vector &v);
void translate(const Point &p);

void rotate(double degrees, const Vector &axis);

void scale(const Vector &v);

void lookAt(const Point &eye, const Point &center, const Vector &up);
void lookAt(const Point &eye, const Vector &direction, const Vector &up);


}  // namespace OpenGL


#endif /* OPENGL_H_ */
