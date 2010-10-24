/*
 * RectSplitter.cpp
 *
 *  Created on: Sep 24, 2010
 *      Author: Roberto
 */

#include "RectSplitter.h"
#include "Random.h"

#include <cstdlib>


namespace Geo {


RectSplitter::RectSplitter(const Vector &minSize)
	: _minSize(minSize) { }

const std::vector<Rectangle>& RectSplitter::split(const Rectangle &rect)
{
	_rects.clear();
	
	_split(rect);
	
	return _rects;
}

bool RectSplitter::_split(const Rectangle &rect)
{
	RandomDouble drand;
	
	// Allow for bigger buildings than the minimum
	if((rect.area() < 4*_minSize.x*_minSize.y) && (drand.rand() < 0.2)) {
		return false;
	}
	
	// X first -- if it fails, then Y
	if(std::rand()%2) {
		if(_trySplitX(rect)) {
			return true;
		}
		
		if(_trySplitY(rect)) {
			return true;
		}
	}
	
	// Y first -- if it fails, then Y
	else {
		if(_trySplitY(rect)) {
			return true;
		}
		
		if(_trySplitX(rect)) {
			return true;
		}
	}
	
	return false;
}

bool RectSplitter::_trySplitX(const Rectangle &rect)
{
	if(rect.size().x > 2*_minSize.x) {
		// Set splitting boundaries on X
		double left = rect.origin().x + _minSize.x,right = rect.origin().x + rect.size().x - _minSize.x;
		double alpha = double(std::rand())/RAND_MAX;
		double x = alpha*left + (1 - alpha)*right;
		
		std::pair<Rectangle,Rectangle> splitted = rect.splitX(x);
		
		// Recur
		if(!_split(splitted.first)) {
			_rects.push_back(splitted.first);
		}

		// Recur
		if(!_split(splitted.second)) {
			_rects.push_back(splitted.second);
		}
		
		return true;
	}
	
	return false;
}

bool RectSplitter::_trySplitY(const Rectangle &rect)
{
	if(rect.size().y > 2*_minSize.y) {
		// Set splitting boundaries on X
		double bottom = rect.origin().y + _minSize.y,top = rect.origin().y + rect.size().y - _minSize.y;
		double alpha = double(std::rand())/RAND_MAX;
		double y = alpha*bottom + (1 - alpha)*top;
		
		std::pair<Rectangle,Rectangle> splitted = rect.splitY(y);

		// Recur
		if(!_split(splitted.first)) {
			_rects.push_back(splitted.first);
		}

		// Recur
		if(!_split(splitted.second)) {
			_rects.push_back(splitted.second);
		}
		
		return true;
	}
	
	return false;
}


} // namespace Geo