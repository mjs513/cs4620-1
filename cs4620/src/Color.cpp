/*
 * Color.cpp
 *
 *  Created on: 30/03/2010
 *      Author: user
 */

#include "Color.h"


Color::Color()
    : r(0), g(0), b(0), a(1) { }

Color::Color(float r, float g, float b)
    : r(r), g(g), b(b), a(1) { }

Color::Color(float r, float g, float b, float a)
    : r(r), g(g), b(b), a(a) { }

const Color Color::black()
{
    return Color();
}

const Color Color::white()
{
    return Color(1,1,1);
}

const Color Color::gray()
{
    return Color(0.5,0.5,0.5);
}

const Color Color::blue()
{
    return Color(0,0,1);
}

const Color Color::red()
{
    return Color(1,0,0);
}

const Color Color::green()
{
    return Color(0,1,0);
}

const Color Color::yellow()
{
    return Color(1,1,0);
}

const Color Color::brown()
{
    return Color(0.5,0.3,0.1);
}

const Color Color::orange()
{
    return Color(1,0.5,0);
}

const Color Color::magenta()
{
    return Color(1,0,1);
}

const Color Color::cyan()
{
    return Color(0,1,1);
}

const Color Color::clear()
{
    return Color(0,0,0,0);
}

std::ostream& operator<<(std::ostream &out, const Color &c)
{
    return out << "{ r=" << c.r << ",g=" << c.g << ",b=" << c.b << ",a=" << c.a << " }";
}
