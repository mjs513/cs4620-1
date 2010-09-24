/*
    Copyright (C) 2008 Roberto Ramalho Fischer
*/

#ifndef RRF_MATH_VECTOR_H
#define RRF_MATH_VECTOR_H


#include <ostream>


class Point;


/// Bidmensional vector.
struct Vector
{
    union {
        struct { double x,y,z; const double w; };
        double v[4];
    };
    
    
    /// Creates the zero vector (0,0,0).
    Vector();
    
    /** \brief  Creates vector with given values, z length equal to 0.
        \param  x X length.
        \param  y Y length.
    */
    Vector(double x, double y);
    
    /** \brief  Creates vector with given values.
        \param  x X length.
        \param  y Y length.
        \param  z Z length.
    */
    Vector(double x, double y, double z);
    
    /** \brief  Creates point with given values.
        \param  p Array of 3 values in order: x, y and z.
    */
    Vector(double *p);
    
    /** \brief  Creates point with given values.
        \param  p Array of 3 values in order: x, y and z.
    */
    Vector(float *p);

    /// Creates vector with coordinates from a point.
    explicit Vector(const Point &p);
    
    /// Returns vector length.
    double length() const;
    
    /// Returns the square of the length.
    double squaredLength() const;
    
    /// Normalizes vector length.
    void normalize();
    
    /// Returns normalized vector length.
    const Vector normalized() const;
    
    /// Returns the projection of this on v.
    const Vector projection(const Vector &v) const;
    
    /// Fills an array of floats using the double values from this.
    void floatV(float v[], int n = 4) const;
    
    /// Attributes values from \a v to this.
    Vector& operator=(const Vector &v);
    
    /// Adds and assigns with values of \a v.
    Vector& operator+=(const Vector &v);
    
    /// Subtracts and assigns with values of \a v.
    Vector& operator-=(const Vector &v);
    
    /// Divides and assigns with a value \a k.
    Vector& operator/=(double k);
    
    /// Multiplies and assigns with a value \a k.
    Vector& operator*=(double k);
    
    /// Returns symmetric.
    const Vector operator-() const;
    
    bool operator==(const Vector &v) const;
    
    bool operator!=(const Vector &v) const;

    /// Returns the dot product of v1 and v2.
    static double dot(const Vector &v1, const Vector &v2);

    /// Returns the cross product of v1 and v2.
    static const Vector cross(const Vector &v1, const Vector &v2);
};


/// Multiplies a vector by a scalar.
const Vector operator+(const Vector &v1, const Vector &v2);

/// Multiplies a vector by a scalar.
const Vector operator-(const Vector &v1, const Vector &v2);

/// Multiplies a vector by a scalar.
const Vector operator*(double k, const Vector &v);

/// Multiplies a vector by a scalar.
const Vector operator*(const Vector &v, double k);

/// Multiplies a vector by a scalar.
const Vector operator/(const Vector &v, double k);

/// Difference between two points.
const Vector operator-(const Point &p1, const Point &p2);

/// Translates a point using a vector.
const Point operator+(const Point &p, const Vector &v);

/// Translates a point using a vector.
const Point operator+(const Vector &v, const Point &p);

/// Translates a point using a vector.
const Point operator-(const Point &p, const Vector &v);


std::ostream& operator<<(std::ostream &out, const Vector &v);

std::istream& operator>>(std::istream &in, Vector &v);


#endif
