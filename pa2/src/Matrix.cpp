/*
    Copyright (C) 2008 Roberto Ramalho Fischer
*/

#include "Matrix.h"
#include "Point.h"
#include "Vector.h"

#include <cmath>
#include <GL/gl.h>


Matrix::Matrix()
{
    for(int i = 0; i < 16; ++i) {
        v[i] = 0;
    }
    
    v[0] = v[5] = v[10] = v[15] = 1;
}

Matrix::Matrix(double **val)
{
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            m[j][i] = val[j][i];
        }
    }
}

Matrix::Matrix(double *val)
{
    for(int i = 0; i < 16; ++i) {
        v[i] = val[i];
    }
}

Matrix::Matrix(float **val)
{
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            m[j][i] = val[j][i];
        }
    }
}

Matrix::Matrix(float *val)
{
    for(int i = 0; i < 16; ++i) {
        v[i] = val[i];
    }
}

const Matrix Matrix::operator*=(const Matrix &m)
{
    GLint matrixMode;
    
    glGetIntegerv(GL_MATRIX_MODE,&matrixMode);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadMatrixd(this->v);
    glMultMatrixd(m.v);

    glGetDoublev(GL_MODELVIEW_MATRIX,this->v);
    
    glPopMatrix();
    
    glMatrixMode(matrixMode);
    
    return *this;
}

bool Matrix::operator==(const Matrix &m) const
{
    for(int i = 0; i < 16; ++i) {
        if(this->v[i] != m.v[i]) {
            return false;
        }
    }
    
    return true;
}

bool Matrix::operator!=(const Matrix &m) const
{
    return !(*this == m);
}


const Matrix Matrix::scaleTransform(const Vector &v)
{
    GLint matrixMode;
    
    glGetIntegerv(GL_MATRIX_MODE,&matrixMode);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadIdentity();
    glScaled(v.x,v.y,v.z);
    
    Matrix m;
    
    glGetDoublev(GL_MODELVIEW_MATRIX,m.v);
    
    glPopMatrix();
    
    glMatrixMode(matrixMode);
    
    return m;
}

const Matrix Matrix::rotationTransform(double degrees, const Vector &axis)
{
    GLint matrixMode;
    
    glGetIntegerv(GL_MATRIX_MODE,&matrixMode);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadIdentity();
    glRotated(degrees,axis.x,axis.y,axis.z);
    
    Matrix m;
    
    glGetDoublev(GL_MODELVIEW_MATRIX,m.v);
    
    glPopMatrix();
    
    glMatrixMode(matrixMode);
    
    return m;
}


namespace {


const Matrix translationMatrix(double x, double y, double z)
{
    GLint matrixMode;
    
    glGetIntegerv(GL_MATRIX_MODE,&matrixMode);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadIdentity();
    glTranslated(x,y,z);
    
    Matrix m;
    
    glGetDoublev(GL_MODELVIEW_MATRIX,m.v);
    
    glPopMatrix();
    
    glMatrixMode(matrixMode);
    
    return m;
}


}  // namespace


const Matrix Matrix::translationTransform(const Vector &v)
{
    return translationMatrix(v.x,v.y,v.z);
}

const Matrix Matrix::translationTransform(const Point &p)
{
    return translationMatrix(p.x,p.y,p.z);
}


namespace {


void multiply(const Matrix &m, const double *in, double *out)
{
    for(int i = 0; i < 4; ++i) {
        out[i] = 0;
        
        for(int j = 0; j < 4; ++j) {
            out[i] += m.m[j][i]*in[j];
        }
    }
}


}  // namespace


const Vector operator*(const Matrix &m, const Vector &v)
{
    Vector r;
    
    multiply(m,v.v,r.v);
    
    return r;
}

const Point operator*(const Matrix &m, const Point &p)
{
    Point r;
    
    multiply(m,p.v,r.v);
    
    return r;
}

const Matrix operator*(const Matrix &m1, const Matrix &m2)
{
    return Matrix(m1) *= m2;
}

std::ostream& operator<<(std::ostream &out, const Matrix &m)
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
