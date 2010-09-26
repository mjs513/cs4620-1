/*
 * Building.h
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#ifndef BUILDING_H_
#define BUILDING_H_


#include "Rectangle.h"
#include "Displayable.h"


class Building : public Displayable
{
public:
	Building(const Geo::Rectangle &base, double height = 0);

	const Geo::Rectangle& base() const;
	double height() const;
	
	void draw(const Frustum &frustum);
	
	bool testFrustum() const;
	
private:
	Geo::Rectangle _base;
	double _height;
	int _numberOfFloors,_numberOfColumnsX,_numberOfColumnsY;
};

#endif /* BUILDING_H_ */
