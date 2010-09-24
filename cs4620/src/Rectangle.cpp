/*
 * Rectangle.cpp
 *
 *  Created on: Sep 24, 2010
 *      Author: Roberto
 */

#include "Rectangle.h"


Rectangle::Rectangle() { }

Rectangle::Rectangle(const Point &origin, const Vector &size)
	: _origin(origin), _size(size) { }

const Point Rectangle::origin() const
{
	return _origin;
}

Rectangle& Rectangle::setOrigin(const Point &origin)
{
	_origin = origin;

	return *this;
}

const Vector Rectangle::size() const
{
	return _size;
}

Rectangle& Rectangle::setSize(const Vector &size)
{
	_size = size;

	return *this;
}

const Point Rectangle::center() const
{
	return _origin + 0.5*_size;
}

Rectangle& Rectangle::setCenter(const Point &center)
{
	_origin = center - 0.5*_size;

	return *this;
}

const Vector Rectangle::offset() const
{
	return 0.5*_size;
}

Rectangle& Rectangle::setOffset(const Vector &offset)
{
	_size = 2*offset;

	return *this;
}

std::pair<Rectangle,Rectangle> splitX(double x)
{

}

std::pair<Rectangle,Rectangle> splitY(double x)
{
	
}
