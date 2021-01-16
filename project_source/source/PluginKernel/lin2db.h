#pragma once
#include <cmath>

class lin2db
{
public:

	lin2db();
	~lin2db();

	float convert(float input);

private:
	float x = 0.0;
};
