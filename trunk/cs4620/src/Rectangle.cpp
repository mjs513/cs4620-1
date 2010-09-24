/*
 * Rectangle.cpp
 *
 *  Created on: Sep 24, 2010
 *      Author: Roberto
 */

#include "Rectangle.h"


namespace Geo {


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

std::pair<Rectangle,Rectangle> Rectangle::splitX(double x) const
{
	std::pair<Rectangle,Rectangle> rects;
	Vector size = this->size();
	
	size.x = x - this->origin().x;
	
	rects.first.setOrigin(this->origin());
	rects.first.setSize(size);
	
	Point origin = this->origin();
	
	origin.x = x;
	size.x = this->origin().x + this->size().x - x;
	
	rects.second.setOrigin(origin);
	rects.second.setSize(size);
	
	return rects;
}

std::pair<Rectangle,Rectangle> Rectangle::splitY(double y) const
{
	std::pair<Rectangle,Rectangle> rects;
	Vector size = this->size();
	
	size.y = y - this->origin().y;
	
	rects.first.setOrigin(this->origin());
	rects.first.setSize(size);
	
	Point origin = this->origin();
	
	origin.y = y;
	size.y = this->origin().y + this->size().y - y;
	
	rects.second.setOrigin(origin);
	rects.second.setSize(size);
	
	return rects;
}

const Rectangle Rectangle::inset(double border) const
{
	Rectangle rect = *this;
	
	rect.setSize(rect.size() - 2*border*Vector(1,1));
	rect.setOrigin(rect.origin() + border*Vector(1,1));
	
	return rect;
}


} // namespace Geo


std::ostream& operator<<(std::ostream &out, const Geo::Rectangle &rect)
{
	return out << "[" << rect.origin() << ", " << rect.size() << "]";
}
