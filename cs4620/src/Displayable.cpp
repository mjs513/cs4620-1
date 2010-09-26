/*
 * Displayable.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#include "Displayable.h"

#include <iostream>


Displayable::~Displayable() { }

void Displayable::display(const Frustum &frustum)
{
	bool doDraw = true;
	
	if(testFrustum()) {
		doDraw = frustum.includes(_boundingSphere);
	}
	
	if(doDraw) {
		draw(frustum);
	}
	else {
		std::cout << "dont display" << std::endl;
	}
}

Displayable& Displayable::setBoundingSphere(const BoundingSphere &sphere)
{
	_boundingSphere = sphere;
	
	return *this;
}

const BoundingSphere& Displayable::boundingSphere() const
{
	return _boundingSphere;
}
