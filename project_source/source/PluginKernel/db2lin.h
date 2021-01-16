#pragma once
#include <valarray>

class db2lin
{
public:

	db2lin();
	~db2lin();

	float convert(float input);

private:
	float x = 0.0;
};