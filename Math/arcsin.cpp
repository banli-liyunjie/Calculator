#include "arcsin.h"
#include <string>
using namespace std;

#include <sstream>

#define PI 3.14159265358979323846

//ArcSin::ArcSin()
//{
//}

string ArcSin::ArcsinTest(double doubleIn)
{
    /*
     * 输入为QString字符串，转化为double类型后正常情况下取值范围为[-1,1]
     * 输出为QString字符串，正常输出结果用角度制来表示就是[-90°,90°]
     * 该函数包含输入输入的异常监测
     * 若是输入数据正常，则输出正确数据结果，
     * 否则，输入error!，并输出对应的error类型
    */

    //0.定义输出
    string ans;

    //1.将输入sIn转化为double类型
    //double doubleIn=sIn.toDouble();
   /* stringstream tmps;
    tmps << sIn;
    double doubleIn;
    tmps >> doubleIn;*/

    //2.判断QString数据是否异常，例如
    /*
     * -----6.32-67【非首字符含有负号】
     * ----6.34.89【含有多个小数点】
     * -----6.---8654【含有多个负号】
     * ------000.00000【只有输入0才有效，多个0和小数点组成的0无效】
     */
    //if(doubleIn==0)
    //{
    //    //排除输入0和0.0的正常情况，其余皆是异常输入
    //    if(sIn !=QString::fromLocal8Bit("0"))
    //    {
    //        if(sIn !=QString::fromLocal8Bit("0.0"))
    //        {
    //            ans="error!--输入数据异常";
    ////            qDebug()<<ans;//测试用
    //            return ans;
    //        }
    //    }
    //}

    //3.判断输入是否超出范围
    if((doubleIn < -1) | (doubleIn > 1))
    {
        ans="error!";
        return ans;
    }

    //4.边界值直接输出
    //由于边界值要想更精确需要展开更多，降低运算速度，因此直接return
    if(doubleIn==-1)
    {
        ans="-90°";//输出直接加上角度符号，表示输出角度制
        return ans;
    }
    if(doubleIn==1)
    {
        ans="90°";//输出直接加上角度符号，表示输出角度制
        return ans;
    }

    //5.普通值的arcsin通过Tayler展开得到
    double result = doubleIn;//最终输出结果
    double result_a = 1.0;//递归初始值，tmp值
    int i;//展开项的项数

    //arcsin展开原理如下
    //i=3时，result={(1/2)dous*dous*(3/4)*dous*dous*(5/6)*dous*dous}*dous/7
    for(i=1;i<800;i++)
    {
        for(int j=1;j<2*i;j=j+2)
        {
            result_a*=(double)j/(double)(j+1)*doubleIn*doubleIn;//1/2*3/4*5/6
        }
        result_a*=doubleIn/(2*i+1);
        result+=result_a;
        result_a=1;
    }
    result=result*180/PI;//输出角度值
    //ans=QString("%6").arg(result);//将角度值转换为QString类型
    ans = to_string(result);

    return ans;
}
