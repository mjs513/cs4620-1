/*
 * WalkingBug.h
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#ifndef WALKINGBUG_H_
#define WALKINGBUG_H_


#include "Character.h"

#include <vector>
#include <QtGui>


class WalkingBug : public Character
{
public:
	WalkingBug();
	
	void update(GLWidget &glWidget);
	
private:
	Point _updateLeg(Joint *joint, const Point &original, float tOffset);
	
private:
	std::vector< std::pair<Joint*,Point> > _originalLegPositions;
	QTime _beginTime;
};

#endif /* WALKINGBUG_H_ */
