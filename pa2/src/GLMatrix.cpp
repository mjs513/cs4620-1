/*
    Copyright (C) 2008 Roberto Ramalho Fischer
*/

#include "GLMatrix.h"
#include "Point.h"
#include "Vector.h"

#include <cmath>
#include <GL/gl.h>


GLMatrix::GLMatrix()
{
    for(int i = 0; i < 16; ++i) {
        v[i] = 0;
    }
    
    v[0] = v[5] = v[10] = v[15] = 1;
}

GLMatrix::GLMatrix(double **val)
{
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            m[j][i] = val[j][i];
        }
    }
}

GLMatrix::GLMatrix(double *val)
{
    for(int i = 0; i < 16; ++i) {
        v[i] = val[i];
    }
}

GLMatrix::GLMatrix(float **val)
{
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            m[j][i] = val[j][i];
        }
    }
}

GLMatrix::GLMatrix(float *val)
{
    for(int i = 0; i < 16; ++i) {
        v[i] = val[i];
    }
}

const GLMatrix GLMatrix::operator*=(const GLMatrix &m)
{
    GLint matrixMode;
    
    glGetIntegerv(GL_MATRIX_MODE,&matrixMode);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadMatrixd(m.v);
    glMultMatrixd(this->v);

    glGetDoublev(GL_MODELVIEW_MATRIX,this->v);
    
    glPopMatrix();
    
    glMatrixMode(matrixMode);
    
    return *this;
}

bool GLMatrix::operator==(const GLMatrix &m) const
{
    for(int i = 0; i < 16; ++i) {
        if(this->v[i] != m.v[i]) {
            return false;
        }
    }
    
    return true;
}

bool GLMatrix::operator!=(const GLMatrix &m) const
{
    return !(*this == m);
}


const GLMatrix GLMatrix::scaleTransform(const Vector &v)
{
    GLint matrixMode;
    
    glGetIntegerv(GL_MATRIX_MODE,&matrixMode);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadIdentity();
    glScaled(v.x,v.y,v.z);
    
    GLMatrix m;
    
    glGetDoublev(GL_MODELVIEW_MATRIX,m.v);
    
    glPopMatrix();
    
    glMatrixMode(matrixMode);
    
    return m;
}

const GLMatrix GLMatrix::rotationTransform(double degrees, const Vector &axis)
{
    GLint matrixMode;
    
    glGetIntegerv(GL_MATRIX_MODE,&matrixMode);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadIdentity();
    glRotated(degrees,axis.x,axis.y,axis.z);
    
    GLMatrix m;
    
    glGetDoublev(GL_MODELVIEW_MATRIX,m.v);
    
    glPopMatrix();
    
    glMatrixMode(matrixMode);
    
    return m;
}


namespace {


const GLMatrix translationMatrix(double x, double y, double z)
{
    GLint matrixMode;
    
    glGetIntegerv(GL_MATRIX_MODE,&matrixMode);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadIdentity();
    glTranslated(x,y,z);
    
    GLMatrix m;
    
    glGetDoublev(GL_MODELVIEW_MATRIX,m.v);
    
    glPopMatrix();
    
    glMatrixMode(matrixMode);
    
    return m;
}


}  // namespace


const GLMatrix GLMatrix::translationTransform(const Vector &v)
{
    return translationMatrix(v.x,v.y,v.z);
}

const GLMatrix GLMatrix::translationTransform(const Point &p)
{
    return translationMatrix(p.x,p.y,p.z);
}


namespace {


void multiply(const GLMatrix &m, const double *in, double *out)
{
    for(int i = 0; i < 4; ++i) {
        out[i] = 0;
        
        for(int j = 0; j < 4; ++j) {
            out[i] += m.m[j][i]*in[j];
        }
    }
}


}  // namespace


const Vector operator*(const GLMatrix &m, const Vector &v)
{
    Vector r;
    
    multiply(m,v.v,r.v);
    
    return r;
}

const Point operator*(const GLMatrix &m, const Point &p)
{
    Point r;
    
    multiply(m,p.v,r.v);
    
    return r;
}

const GLMatrix operator*(const GLMatrix &m1, const GLMatrix &m2)
{
    return GLMatrix(m1) *= m2;
}

std::ostream& operator<<(std::ostream &out, const GLMatrix &m)
{
    out << "[ ";
    
    for(int i = 0; i < 4; ++i) {
        out << "[ ";
        
        for(int j = 0; j < 4; ++j) {
            out << m.m[j][i] << " ";
        }
        
        out << "] ";
    }
    
    out << "]";
    
    return out;
}
