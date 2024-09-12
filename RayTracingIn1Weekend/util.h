#ifndef __UTIL__
#define __UTIL__
#include <iostream>
#include <random>

inline double randomF64()
{
	return rand() / (RAND_MAX + 1.0);
}

template <typename T>
bool isInRange(T t, T min, T max)
{
	return (t >= min) && (t <= max);
}


#endif __UTIL__