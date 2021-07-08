#include "sin.h"
#define Pi 3.14159265358979323846
//Sin::Sin()
//{
//
//}

string Sin::SinTest(double num)
{
    //0.定义输出
    string ans;
    //1.将输入sIn转化为double类型
    //double num=sIn.toDouble();

    //2.判断QString数据是否异常
    //if(num==0)
    //{
    //    //排除输入0和0.0的正常情况，其余皆是异常输入
    //    if(sIn !=QString::fromLocal8Bit("0"))
    //    {
    //        if(sIn !=QString::fromLocal8Bit("0.0"))
    //        {
    //            ans="error!--输入数据异常";
    //            return ans;
    //        }
    //    }
    //}

    //3.通过Tayler展开得到
    while (num > 180 || num < -180)
    {
        num = num > 180 ? num - 2 * 180 : num + 2 * 180;
    }
    num=num/180*Pi;
    double n = num, sum = 0;
    int i = 0;
    do
    {
        sum += n;
        i++;
        n = -n * num * num / (2.0 * i) / (2.0 * i + 1);
    } while (n < -0.000000001 || n>0.000000001);
    sum = 1.0 * (int)(sum * 1000000) / 1000000;
    //ans=QString("%6").arg(sum);
    ans = to_string(sum);
    return ans;
}
