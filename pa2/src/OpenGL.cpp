/*
 * OpenGL.cpp
 *
 *  Created on: 30/03/2010
 *      Author: user
 */

#include "OpenGL.h"


namespace OpenGL {


void lightPosition(GLenum light, const Vector &v)
{
    float f[4];
    
    v.floatV(f);
    
    glLightfv(light,GL_POSITION,f);
}

void lightPosition(GLenum light, const Point &p)
{
    float f[4];
    
    p.floatV(f);
    
    glLightfv(light,GL_POSITION,f);
}

void lightColor(GLenum light, GLenum param, const Color &c)
{
    glLightfv(light,param,c.v);
}

void fogColor(const Color &c)
{
	glFogfv(GL_FOG_COLOR,c.v);
}

void color(const Color &c)
{
    glColor4fv(c.v);
}

void clearColor(const Color &c)
{
    glClearColor(c.r,c.g,c.b,c.a);
}

void vertex(const Point &p)
{
    vertex3(p);
}

void vertex2(const Point &p)
{
    glVertex2dv(p.v);
}

void vertex3(const Point &p)
{
    glVertex3dv(p.v);
}

void vertex4(const Point &p)
{
    glVertex4dv(p.v);
}

void texture(const Point &p)
{
    texture2(p);
}

void texture2(const Point &p)
{
    glTexCoord2dv(p.v);
}

void texture3(const Point &p)
{
    glTexCoord3dv(p.v);
}

void normal(const Vector &v)
{
    glNormal3dv(v.v);
}

void loadMatrix(const Matrix &m)
{
    glLoadMatrixd(m.v);
}

void translate(const Vector &v)
{
    glTranslated(v.x,v.y,v.z);
}

void translate(const Point &p)
{
    glTranslated(p.x,p.y,p.z);
}

void rotate(double degrees, const Vector &axis)
{
    glRotated(degrees,axis.x,axis.y,axis.z);
}

void scale(const Vector &v)
{
    glScaled(v.x,v.y,v.z);
}

void lookAt(const Point &eye, const Point &center, const Vector &up)
{
    gluLookAt(eye.x,eye.y,eye.z,center.x,center.y,center.z,up.x,up.y,up.z);
}

void lookAt(const Point &eye, const Vector &direction, const Vector &up)
{
    Point center = eye + direction;
    
    lookAt(eye,center,up);
}


}  // namespace OpenGL
