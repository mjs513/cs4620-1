/*
 * Building.h
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#ifndef BUILDING_H_
#define BUILDING_H_


#include "Rectangle.h"


class Building
{
public:
	Building(const Geo::Rectangle &base, double height = 0);

	const Geo::Rectangle& base() const;
	double height() const;
	
	void display();
	
private:
	Geo::Rectangle _base;
	double _height;
};

#endif /* BUILDING_H_ */
