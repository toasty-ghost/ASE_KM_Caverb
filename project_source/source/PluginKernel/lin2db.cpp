#include "lin2db.h"

lin2db::lin2db()
{
	x = 0.0;
}

lin2db::~lin2db() {}

/**
 * \brief Converts a linear scale value to decibels
 * \param input Input value in linear scale
 * \return Outputs level in decibels
 */
float lin2db::convert(float input)
{
	x = input;
	float output = 20 * log10(x);
	return output;
}