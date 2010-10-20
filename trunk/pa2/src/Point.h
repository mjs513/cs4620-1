/*
    Copyright (C) 2008 Roberto Ramalho Fischer
*/

#ifndef RRF_MATH_POINT_H
#define RRF_MATH_POINT_H


#include <ostream>
#include <istream>


class Vector;


/// Bidmensional point.
struct Point
{
    union {
        struct { double x,y,z; const double w; };
        double v[4];
    };
    
    
    /// Creates the origin point (0,0,0).
    Point();
    
    /** \brief  Creates point with given values, z coordinate equal to 0.
        \param  x X coordinate.
        \param  y Y coordinate.
    */
    Point(double x, double y);
    
    /** \brief  Creates point with given values.
        \param  x X coordinate.
        \param  y Y coordinate.
        \param  z Z coordinate.
    */
    Point(double x, double y, double z);
    
    /** \brief  Creates point with given values.
        \param  p Array of 3 values in order: x, y and z.
    */
    Point(double *p);
    
    /** \brief  Creates point with given values.
        \param  p Array of 3 values in order: x, y and z.
    */
    Point(float *p);

    /// Creates point with coordinates from a vector.
    explicit Point(const Vector &v);
    
    /// Fills an array of floats using the double values from this.
    void floatV(float v[], int n = 4) const;
    
    /// Translates this point by v.
    Point& operator+=(const Vector &v);

    /// Translates this point by -v.
    Point& operator-=(const Vector &v);
    
    /// Attributes values from \a v to this.
    Point& operator=(const Point &v);
    
    /// Returns symmetric.
    const Point operator-() const;
    
    bool operator==(const Point &v) const;
    
    bool operator!=(const Point &v) const;
};


std::ostream& operator<<(std::ostream &out, const Point &p);

std::istream& operator>>(std::istream &in, Point &p);


#endif
