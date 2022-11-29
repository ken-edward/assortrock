#include "stdafx.h"
#include <math.h>


bool fequal(float a, float b)
{
	if (fabsf(a - b) < 0.001f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

