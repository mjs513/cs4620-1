/*
 * Character.h
 *
 *  Created on: Oct 24, 2010
 *      Author: Roberto
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_


#include "Joint.h"

#include <QtGui>


class GLWidget;


class Character
{
public:
	Character();
	virtual ~Character();
	
	virtual void update(GLWidget &glWidget);
	
	void startAnimation();
	
	Joint* root();
	
protected:
	const QTime& animationStartTime();
	double animationCycleTime(double cycleDuration = 1);
	
	virtual void resetState();
	
protected:
	Joint *_root;
	
private:
	QTime _animationStartTime;
};

#endif /* CHARACTER_H_ */
