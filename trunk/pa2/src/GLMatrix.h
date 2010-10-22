/*
    Copyright (C) 2008 Roberto Ramalho Fischer
*/

#ifndef RRF_MATH_MATRIX_H
#define RRF_MATH_MATRIX_H


#include <ostream>


class Point;
class Vector;


/// Tridimensional Matrix for bidimensional transform purposes.
struct GLMatrix
{
    union {
        double m[4][4];  ///< Matrix values for bidimensional array access.
        double v[16];    ///< Matrix values for unidimensional array access.
    };
    
    
    /// Creates the identity matrix.
    GLMatrix();
    
    /// Creates the matrix using \a val[0..3][0..3]
    GLMatrix(double **val);
    
    /// Creates the matrix using \a val[0..16].
    GLMatrix(double *val);
    
    /// Creates the matrix using \a val[0..3][0..3]
    GLMatrix(float **val);
    
    /// Creates the matrix using \a val[0..16].
    GLMatrix(float *val);
    
    /// Multiplies and assigns with another matrix \a m.
    const GLMatrix operator*=(const GLMatrix &m);
    
    bool operator==(const GLMatrix &m) const;
    
    bool operator!=(const GLMatrix &m) const;
    
    /** \brief  Returns the scale transform matrix.
        \param  v Scaling parameter.
        \return The transform matrix.
    */
    static const GLMatrix scaleTransform(const Vector &scale);
    
    /** \brief  Returns the rotation transform matrix.
        \param  angle The rotation angle in degrees.
        \param  axis The rotation axis.
        \return The transform matrix.
    */
    static const GLMatrix rotationTransform(double degrees, const Vector &axis);
    
    /** \brief  Returns the translation transform matrix.
        \param  v The translation vector.
        \return The transform matrix.
    */
    static const GLMatrix translationTransform(const Vector &v);
    
    /** \brief  Returns the translation transform matrix.
        \param  p The origin for the new coordinate system, translating from current origin to \a p.
        \return The transform matrix.
    */
    static const GLMatrix translationTransform(const Point &p);
};


/// Returns the column vector resulting from the multiplication. Uses w = 0.
const Vector operator*(const GLMatrix &m, const Vector &v);

/// Returns the column vector resulting from the multiplication. Uses w = 1.
const Point operator*(const GLMatrix &m, const Point &p);

/// Returns the resulting matrix of m1*m2.
const GLMatrix operator*(const GLMatrix &m1, const GLMatrix &m2);


std::ostream& operator<<(std::ostream &out, const GLMatrix &m);


#endif
