/*
    Copyright (C) 2008 Roberto Ramalho Fischer
*/

#include "Vector.h"
#include "Point.h"

#include <cmath>


Vector::Vector()
    : x(0), y(0), z(0), w(0) { }

Vector::Vector(double x, double y)
    : x(x), y(y), z(0), w(0) { }

Vector::Vector(double x, double y, double z)
    : x(x), y(y), z(z), w(0) { }

Vector::Vector(double *p)
    : x(p[0]), y(p[1]), z(p[2]), w(0) { }

Vector::Vector(float *p)
    : x(p[0]), y(p[1]), z(p[2]), w(0) { }

Vector::Vector(const Point &p)
    : x(p.x), y(p.y), z(p.z), w(0) { }

void Vector::floatV(float v[], int n) const
{
    for(int i = 0; i < n; ++i) {
        v[i] = this->v[i];
    }
}

double Vector::length() const
{
    return std::sqrt(squaredLength());
}

double Vector::squaredLength() const
{
    return x*x + y*y + z*z;
}

void Vector::normalize()
{
    *this /= this->length();
}

const Vector Vector::normalized() const
{
    Vector v = *this;
    
    v.normalize();
    
    return v;
}

const Vector Vector::projection(const Vector &v) const
{
    return v*Vector::dot(*this,v)/v.squaredLength();
}

Vector& Vector::operator=(const Vector &v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    
    return *this;
}

Vector& Vector::operator+=(const Vector &v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    
    return *this;
}

Vector& Vector::operator-=(const Vector &v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    
    return *this;
}

Vector& Vector::operator/=(double k)
{
    return *this *= 1/k;
}

Vector& Vector::operator*=(double k)
{
    this->x *= k;
    this->y *= k;
    this->z *= k;
    
    return *this;
}

const Vector Vector::operator-() const
{
    return Vector(-x,-y,-z);
}

bool Vector::operator==(const Vector &v) const
{
    return (this->x == v.x) && (this->y == v.y) && (this->z == v.z);
}

bool Vector::operator!=(const Vector &v) const
{
    return !(*this == v);
}

double Vector::dot(const Vector &v1, const Vector &v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

const Vector Vector::cross(const Vector &v1, const Vector &v2)
{
    return Vector(v1.y*v2.z - v1.z*v2.y,v1.z*v2.x - v1.x*v2.z,v1.x*v2.y - v1.y*v2.x);
}

const Vector operator+(const Vector &v1, const Vector &v2)
{
	return Vector(v1) += v2;
}

const Vector operator-(const Vector &v1, const Vector &v2)
{
	return Vector(v1) -= v2;
}

const Vector operator*(double k, const Vector &v)
{
    return Vector(v.x*k,v.y*k,v.z*k);
}

const Vector operator*(const Vector &v, double k)
{
    return k*v;
}

const Vector operator/(const Vector &v, double k)
{
    return v*(1/k);
}

const Vector operator-(const Point &p1, const Point &p2)
{
    return Vector(p1.x - p2.x,p1.y - p2.y,p1.z - p2.z);
}

const Point operator+(const Point &p, const Vector &v)
{
    return Point(p.x + v.x,p.y + v.y,p.z + v.z);
}

const Point operator+(const Vector &v, const Point &p)
{
    return p + v;
}

const Point operator-(const Point &p, const Vector &v)
{
    return operator+(p,-v);
}

std::ostream& operator<<(std::ostream &out, const Vector &v)
{
    out << "[ ";
    
    for(int i = 0; i < 3; ++i) {
        out << v.v[i] << " ";
    }
    
    out << "0 ]";
    
    return out;
}

std::istream& operator>>(std::istream &in, Vector &v)
{
    in >> v.x >> v.y >> v.z;
    
    return in;
}
