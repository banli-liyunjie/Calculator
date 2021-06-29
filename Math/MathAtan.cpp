#include "MathAtan.h"
#define Pi 3.141592654

namespace math
{
	double atan(double num)
	{
		double numt = num > 1 ? 1 / num //arctan(x)+arctan(1/x)=Pi/2
			: num < -1 ? -(1/num)
			: num < 0 ? -num : num;
		double n = numt, sum = 0;
		if (numt >= 0.999999)//arctan(x)x->1 收敛慢
			sum = 0.785398;
		else
		{
			int i = 0;
			do
			{
				sum += n;
				i++;
				n = -n * numt * numt * (2.0 * i - 1) / (2.0 * i + 1);
			} while (n < -0.000000001 || n > 0.000000001);
		}
		sum = num > 1 ? Pi / 2 - sum
			: num < -1 ? -(Pi / 2 - sum)
			: num < 0 ? -sum : sum;
		sum = 1.0 * (int)(sum * 1000000) / 1000000;
		return sum;
	}
}
