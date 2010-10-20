/*
 * Displayable.h
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#ifndef DISPLAYABLE_H_
#define DISPLAYABLE_H_


#include "BoundingSphere.h"
#include "Frustum.h"


class Displayable
{
public:
	virtual ~Displayable();
	
	// Test visibility in frustum and then call draw() if visible
	void display(const Frustum &frustum);
	
	// Actual drawing method
	virtual void draw(const Frustum &frustum) = 0;
	
	virtual bool testFrustum() const = 0;
	
	// Frustum culling test is done against this bounding sphere
	virtual Displayable& setBoundingSphere(const BoundingSphere &sphere);
	virtual const BoundingSphere& boundingSphere() const;
	
private:
	BoundingSphere _boundingSphere;
};


#endif /* DISPLAYABLE_H_ */
