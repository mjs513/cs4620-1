/*
    Copyright (C) 2008 Roberto Ramalho Fischer
*/

#include "Point.h"
#include "Vector.h"


Point::Point()
    : x(0), y(0), z(0), w(1) { }

Point::Point(double x, double y)
    : x(x), y(y), z(0), w(1) { }

Point::Point(double x, double y, double z)
    : x(x), y(y), z(z), w(1) { }

Point::Point(double *p)
    : x(p[0]), y(p[1]), z(p[2]), w(1) { }

Point::Point(float *p)
    : x(p[0]), y(p[1]), z(p[2]), w(1) { }

Point::Point(const Vector &v)
    : x(v.x), y(v.y), z(v.z), w(1) { }

void Point::floatV(float v[], int n) const
{
    for(int i = 0; i < n; ++i) {
        v[i] = this->v[i];
    }
}

Point& Point::operator=(const Point &v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    
    return *this;
}

const Point Point::operator-() const
{
    return Point(-x,-y,-z);
}

Point& Point::operator+=(const Vector &v)
{
    return *this = *this + v;
}

Point& Point::operator-=(const Vector &v)
{
    return *this = *this - v;
}

bool Point::operator==(const Point &v) const
{
    return (this->x == v.x) && (this->y == v.y) && (this->z == v.z);
}

bool Point::operator!=(const Point &v) const
{
    return !(*this == v);
}

std::ostream& operator<<(std::ostream &out, const Point &p)
{
    out << "[ ";
    
    for(int i = 0; i < 3; ++i) {
        out << p.v[i] << " ";
    }
    
    out << "1 ]";
    
    return out;
}

std::istream& operator>>(std::istream &in, Point &p)
{
    in >> p.x >> p.y >> p.z;
    
    return in;
}
