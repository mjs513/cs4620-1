/*
 * Random.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#include "Random.h"

#include <cstdlib>
#include <cmath>


RandomDouble::RandomDouble()
	: _min(0), _max(1) { }

RandomDouble::RandomDouble(double min, double max)
	: _min(min), _max(max) { }

double RandomDouble::rand() const
{
	double alpha = double(std::rand())/RAND_MAX;
	
	return alpha*_min + (1 - alpha)*_max;
}

RandomNormal::RandomNormal()
	: _mean(0), _deviance(1) { }

RandomNormal::RandomNormal(double mean, double variance)
	: _mean(mean), _deviance(std::sqrt(variance)) { }

double RandomNormal::rand() const
{
	RandomDouble drand(0.001,1);
	double u = drand.rand(),v = drand.rand();
	double z = std::sqrt(-2*std::log(u))*std::cos(2*M_PI*v);
	
	return _deviance*z + _mean;
}
