/*
 * Rectangle.h
 *
 *  Created on: Sep 24, 2010
 *      Author: Roberto
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_


#include "Vector.h"
#include "Point.h"


namespace Geo {


class Rectangle
{
public:
	Rectangle();
	Rectangle(const Point &origin, const Vector &size);

	const Point origin() const;
	Rectangle& setOrigin(const Point &origin);

	const Vector size() const;
	Rectangle& setSize(const Vector &size);
	
	double area() const;

	std::pair<Rectangle,Rectangle> splitX(double x) const;
	std::pair<Rectangle,Rectangle> splitY(double y) const;

	const Rectangle inset(double border) const;
	
	void draw() const;

private:
	Point _origin;
	Vector _size;
};


} // namespace Geo


std::ostream& operator<<(std::ostream &out, const Geo::Rectangle &rect);


#endif /* RECTANGLE_H_ */
