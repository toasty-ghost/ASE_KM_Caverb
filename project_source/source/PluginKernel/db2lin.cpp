#include "db2lin.h"

db2lin::db2lin()
{
	x = 0.0;
}

db2lin::~db2lin()
{
}

/**
 * \brief Converts a value in decibels to a linear value
 * \param input Input value in decibels
 * \return Linear scale value for given input level in decibels
 */
float db2lin::convert(float input)
{
	x = input;
	float output = std::pow(10, x / 20.0);
	return output;
}