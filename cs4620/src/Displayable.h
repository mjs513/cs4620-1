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
	
	void display(const Frustum &frustum);
	
	virtual void draw(const Frustum &frustum) = 0;

	virtual bool testFrustum() const = 0;

	virtual Displayable& setBoundingSphere(const BoundingSphere &sphere);
	virtual const BoundingSphere& boundingSphere() const;
	
private:
	BoundingSphere _boundingSphere;
};


#endif /* DISPLAYABLE_H_ */
