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
	
	bool includes(const BoundingSphere &sphere) const;
	
private:
	double _frustum[6][4];
};


#endif /* FRUSTUM_H_ */
