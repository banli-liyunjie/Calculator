#include "MathASin.h"

namespace math
{
	double asin(double num)
	{
		if(num < -1 || num > 1)
			return 0;
		double n = num, sum = 0;
		int i = 0;
		do
		{
			sum += n;
			i++;
			n = n * num * num * (2.0 * i - 1) * (2.0 * i - 1) / (2.0 * i) / (2.0 * i + 1);
		} while (n < -0.000000001 || n>0.000000001);
		sum = 1.0 * (int)(sum * 1000000) / 1000000;
		return sum;
	}
}
