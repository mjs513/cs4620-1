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


class Rectangle
{
public:
	Rectangle();
	Rectangle(const Point &origin, const Vector &size);

	const Point origin() const;
	Rectangle& setOrigin(const Point &origin);

	const Vector size() const;
	Rectangle& setSize(const Vector &size);

	const Point center() const;
	Rectangle& setCenter(const Point &center);

	const Vector offset() const;
	Rectangle& setOffset(const Vector &offset);

	std::pair<Rectangle,Rectangle> splitX(double x);
	std::pair<Rectangle,Rectangle> splitY(double x);

private:
	Point _origin;
	Vector _size;
};


#endif /* RECTANGLE_H_ */
