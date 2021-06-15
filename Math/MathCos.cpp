#include "MathCos.h"

#define Pi 3.141592654
namespace math
{
	double cos(double num)
	{
		while (num > Pi || num < -Pi)
		{
			num = num > Pi ? num - 2 * Pi : num + 2 * Pi;
		}
		double n = 1, sum = 0;
		int i = 0;
		do
		{
			sum += n;
			i++;
			n = -n * num * num / (2.0 * i - 1) / (2.0 * i);
		} while (n < -0.000000001 || n>0.000000001);
		sum = 1.0 * (int)(sum * 1000000) / 1000000;
		return sum;
	}
}