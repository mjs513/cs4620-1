/*
 * Random.h
 *
 *  Created on: Sep 25, 2010
 *      Author: Roberto
 */

#ifndef RANDOM_H_
#define RANDOM_H_


class RandomDouble
{
public:
	RandomDouble();
	RandomDouble(double min, double max);
	
	double rand() const;
	
private:
	double _min,_max;
};


class RandomNormal
{
public:
	RandomNormal();
	RandomNormal(double mean, double variance);
	
	double rand() const;
	
private:
	double _mean,_deviance;
};


#endif /* RANDOM_H_ */
