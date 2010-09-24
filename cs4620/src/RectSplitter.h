/*
 * RectSplitter.h
 *
 *  Created on: Sep 24, 2010
 *      Author: Roberto
 */

#ifndef RECTSPLITTER_H_
#define RECTSPLITTER_H_


#include "Rectangle.h"
#include "Vector.h"
#include <vector>


namespace Geo {


class RectSplitter
{
public:
	RectSplitter(const Vector &minSize);
	
	const std::vector<Rectangle>& split(const Rectangle &rect);
	
private:
	bool _split(const Rectangle &rect);
	bool _trySplitX(const Rectangle &rect);
	bool _trySplitY(const Rectangle &rect);
	
private:
	Vector _minSize;
	std::vector<Rectangle> _rects;
};


} // namespace Geo


#endif /* RECTSPLITTER_H_ */
