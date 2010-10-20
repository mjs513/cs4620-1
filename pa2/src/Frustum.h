/*
 * Frustum.h
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#ifndef FRUSTUM_H_
#define FRUSTUM_H_


#include "Plane.h"
#include "BoundingSphere.h"

#include <vector>


class Frustum
{
public:
	Frustum();

	float pctVisible() const;
	
	bool includes(const BoundingSphere &sphere) const;
	
private:
	double _frustum[6][4];
	mutable int _totalTests,_passedTests;
};


#endif /* FRUSTUM_H_ */
