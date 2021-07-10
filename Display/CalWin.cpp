#include "CalWin.h"
#include <thread>
//#include "MathSin.h"
//#include "MathCos.h"
//#include "MathAsin.h"
//#include "MathAtan.h"
#include "CalWin.h"

#include "sin.h"
#include "cos.h"
#include "arcsin.h"
#include  "arctan.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <sstream>
using scr_print::Screen;
using scr_print::endl;
using scr_print::ends;

#include <string>
using namespace std;

/*
* REG: Screenע��������Զ����ɣ���չ����ΪScreenָ��Ķ��塢��ʼ���������Ӧ���������������
* ÿһ�е�һ����ϢΪ���֣�����7������ΪScreen��ʼ��������
* ��9������ΪScreen��ʼ��ʾ�ַ�������10������Ϊ���������Ӧ����
* *********************************************************************************************
* ����Screenֻ��Ҫ�����ע������ʵ�ֶ�Ӧ�ĵ����Ӧ������
* ����ָ��������壬��ʼ������Ӧ�������ã����ں�չ�����Զ���ɡ�
*/
#define REG(x)\
x(Enter, 0, 0, 1, 70, BACKGROUND_WHITE, 0, false,"",EnterClick)\
x(Result,1, 0, 1, 70, BACKGROUND_WHITE, 0, false,"",ResultClick)\
\
x(Num7,  3,  1, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    7",  NumClick)\
x(Num8,  3, 11, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    8",  NumClick)\
x(Num9,  3, 21, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    9",  NumClick)\
x(Sin,   3, 31, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n   sin", SinClick)\
x(Num4,  7,  1, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    4",  NumClick)\
x(Num5,  7, 11, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    5",  NumClick)\
x(Num6,  7, 21, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    6",  NumClick)\
x(Cos,   7, 31, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n   cos", CosClick)\
x(Num1, 11,  1, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    1",  NumClick)\
x(Num2, 11, 11, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    2",  NumClick)\
x(Num3, 11, 21, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    3",  NumClick)\
x(ASin, 11, 31, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n   asin",AsinClick)\
x(Neg,  15, 1,  3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    -",  NegClick)\
x(Num0, 15, 11, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    0",  NumClick)\
x(Point,15, 21, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    .",  PointClick)\
x(ATan, 15, 31, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n   atan",AtanClick)\
x(Back, 19, 11, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    ��", BackClick)\
x(Clr,  19, 21, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    AC", ClrClick)\
x(Equal,19, 31, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    =",  EqualClick)\
x(Test,19, 1, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    test",  TestClick)


/*
* REG(DEF): ��չ��Ϊ
*		Screen* pScreenEnter=NULL;
*		void EnterClick(Screen* pSc);
*		...
*		Screen* pScreenEqual=NULL;
*		void EqualClick(Screen* pSc);
*/
#define DEF(Name,sr,sc,hg,wt,bc,fc,vis,str,fun)\
	Screen* pScreen##Name=NULL;\
	void fun(Screen* pSc);
REG(DEF)
#undef DEF

/*
* REG(INIT): ��չ��Ϊ
*		pScreenEnter=new Screen(0, 0, 1, 41, BACKGROUND_WHITE, 0, false);
*		pScreenEnter->operator<< ("");
*		...
*		pScreenEqual=new Screen(19, 31, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false);
*		pScreenEqual->operator<< ("\n    =");
*/
#define INIT(Name,sr,sc,hg,wt,bc,fc,vis,str,fun)\
	pScreen##Name=new Screen(sr,sc,hg,wt,bc,fc,vis);\
	pScreen##Name->operator<< (str);

int posX;
int posY;

/*
* CLICK_CALLBACK: ��չ��Ϊ
*		while(true){
*			if(posX>=0&&posX<0+41&&posY>=0&&posY<0+1)
*				{EnterClick(pScreenEnter);break;}
*			...
*			if(posX>=31&&posX<31+9&&posY>=19&&posY<19+3)
*				{EqualClick(pScreenEqual);break;}
*			break;
*		}
* ***********************************************************
* ����λ�õ��ö�Ӧ�ĵ����Ӧ������������ʵ��δʵ�֣����뱨��
*/
#define POSCLICK(Name,sr,sc,hg,wt,bc,fc,vis,str,fun)\
	if(posX>=sc&&posX<sc+wt&&posY>=sr&&posY<sr+hg)\
		{fun(pScreen##Name);break;}

#define CLICK_CALLBACK \
	while(true){\
		REG(POSCLICK)\
		break;\
    }

/*
* REG(DEL): ��չ��Ϊ
*		delete(pScreenEnter);
*		...
*		delete(pScreenEqual);
*/
#define DEL(Name)\
	delete(pScreen##Name);

std::string sEnter = "";
std::string sTriangle = "";
std::string sNum = "";
std::string sResult;
//��ӽǶ�ֵ
string sAngle = "";



void CalWinInit()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
#if 0
	int width = 41, height = 23;
	COORD coord = { width ,height };
	SMALL_RECT wrt = { 0,0,width - 1,height - 1 };
	SetConsoleScreenBufferSize(hStdin, coord);//���û������ߴ�
	SetConsoleWindowInfo(hStdin, TRUE, &wrt);//���ô���ߴ�
#endif 
	SetConsoleTitle("������");
	
	SetWindowLongPtrA(
		GetConsoleWindow(),
		GWL_STYLE,
		GetWindowLongPtrA(GetConsoleWindow(),
			GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX
	);
#if 0
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_MOUSE_INPUT;
	SetConsoleMode(hStdin, mode);
#endif
}

bool stopflag = false;
void DealTask()
{
	while (!stopflag)
	{
		if (sTriangle != "")
			sEnter = sTriangle + '(' + sNum +sAngle+ ')';
		else if (sNum != "")
			sEnter = sNum;
		else
			sEnter = "";
		pScreenEnter->Clear() << sEnter;
		if (sResult != "")
			pScreenResult->Clear().SetPos(0, pScreenResult->GetWidth() - sResult.size()) << sResult;
		Sleep(20);
	}
}

void MainTask()
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE); // ��ȡ��׼�����豸���
	INPUT_RECORD inRec;
	DWORD res;
	std::thread dealT(DealTask);
	while (!stopflag)
	{
		ReadConsoleInput(hInput, &inRec, 1, &res);
		if (inRec.EventType == MOUSE_EVENT && inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) //������
		{
			posX = inRec.Event.MouseEvent.dwMousePosition.X;
			posY = inRec.Event.MouseEvent.dwMousePosition.Y;
			CLICK_CALLBACK;
		}
		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27)
			{
				stopflag = true;
				break;
			}
		}
		Sleep(50);
	}
	dealT.join();
}

void ReleaseWin()
{
	REG(DEL);
}

void StartCalWin()
{
	CalWinInit();
	REG(INIT);
	std::thread mT(MainTask);
	mT.join();
	ReleaseWin();
}

char GetChar(Screen* pSc)
{
	if (pSc == pScreenNum0)
		return '0';
	if (pSc == pScreenNum1)
		return '1';
	if (pSc == pScreenNum2)
		return '2';
	if (pSc == pScreenNum3)
		return '3';
	if (pSc == pScreenNum4)
		return '4';
	if (pSc == pScreenNum5)
		return '5';
	if (pSc == pScreenNum6)
		return '6';
	if (pSc == pScreenNum7)
		return '7';
	if (pSc == pScreenNum8)
		return '8';
	if (pSc == pScreenNum9)
		return '9';
}

void EnterClick(Screen* pSc) {}
void ResultClick(Screen* pSc) {}
void NumClick(Screen* pSc) 
{
	sNum += GetChar(pSc);
}

bool pointCheck = false;

void PointClick(Screen* pSc)
{
	if (!pointCheck)
	{
		sNum += '.';
		pointCheck = true;
	}
}
void SinClick(Screen* pSc)
{
	sTriangle = "sin";
	sAngle = "��";
}
void CosClick(Screen* pSc)
{
	sTriangle = "cos";
	sAngle = "��";
}
void AsinClick(Screen* pSc)
{
	sTriangle = "asin";
}
void AtanClick(Screen* pSc)
{
	sTriangle = "atan";
}
void NegClick(Screen* pSc)
{
	if (sNum != "")
	{
		if (sNum[0] == '-')
			sNum = sNum.substr(1);
		else
			sNum = '-' + sNum;
	}
	else
		sNum += '-';
}
void BackClick(Screen* pSc)
{
	if (sNum != "")
	{
		if (sNum[sNum.size() - 1] == '.')
			pointCheck = false;
		sNum = sNum.substr(0, sNum.size() - 1);
	}
	else if (sTriangle != "")
	{
		sTriangle = "";
	}
}
void ClrClick(Screen* pSc)
{
	sTriangle = "";
	sNum = "";
	pointCheck = false;
}
void EqualClick(Screen* pSc)
{
	if (sTriangle != "")
	{
		if (sNum != "" && sNum != "-")
		{
			std::stringstream si(sNum);
			std::stringstream so;
			double Num;
			si >> Num;
			if (sTriangle == "sin")
			{
				//so << math::sin(Num);
				string ans;
				ans = Sin::SinTest(Num);
				//ans += "��";//���ϽǶȷ���
				//4����ʾ�ڽ�����
				so << ans;
			}
			else if (sTriangle == "cos")
			{
				//so << math::cos(Num);
				string ans;
				ans = Cos::CinTest(Num);
				//ans += "��";//���ϽǶȷ���
				//4����ʾ�ڽ�����
				so << ans;
			}
			else if (sTriangle == "asin")
			{
				//so << math::asin(Num);
				 
				//1)ans-----arcsin������
				//ArcSin* arcsinTest = NULL;
				string ans;
				//ans= arcsinTest->ArcsinTest(Num);
				ans = ArcSin::ArcsinTest(Num);
				ans += "��";//���ϽǶȷ���
				//4����ʾ�ڽ�����
				so << ans;
			}
			else if (sTriangle == "atan")
			{
				//so << math::atan(Num);
				string ans;
				//ans= arcsinTest->ArcsinTest(Num);
				ans = ArcTan::ArctanTest(Num);
				ans += "��";//���ϽǶȷ���
				//4����ʾ�ڽ�����
				so << ans;
			}

			sResult = so.str();
		}
		else
		{
			sResult = "Error!";
		}
	}
	else
	{
		if (sNum != "")
		{
			if (sNum == "-")
				sResult = "0";
			else
			{
				std::stringstream si(sNum);
				std::stringstream so;
				double Num;
				si >> Num;
				so << Num;
				sResult = so.str();
			}
		}
		else
		{
			sResult = "0";
		}
	}
}


double strDou(string str) {	//string ת�� double
	char* ch = new char[0];
	double d;
	for (int i = 0; i != str.length(); i++)
		ch[i] = str[i];
	d = atof(ch);
	return d;
}


std::string testAll()
{
	std::string answer="";//�����ʾ���Խ��
	std::string sIn;//���Tayler������
	double doubleIn;//Tayler������double����
	double compareIn;//����math����������
	double diff;//�������ֵ

	double solveSin[3] = { 0,0,0 };
	double solveCin[3] = { 0,0,0 };
	double solveArcsin[4] = { 0,0,0,0 };
	double solveArctan[3] = { 0,0,0 };


	 //----------1.sin��������-----------------
	//1����������
	sIn = Sin::SinTest(-30.5);//-30.5����
	//doubleIn = sIn.toDouble();//ת��Ϊdouble
	doubleIn = strDou(sIn);
	compareIn = sin(-30.5 * M_PI / 180);
	
	diff = abs(doubleIn - compareIn);

	if (diff < 0.01)
	{
		solveSin[0] = 1;//1��ʾ��������ͨ��
		//answer += "  1)��������ͨ����\n";
	}
	else {
		//answer += "  1)�������Բ�ͨ����\n";
		//qDebug() << diff;
	}

	//2��0����
	sIn = Sin::SinTest(0);//0����
	doubleIn = strDou(sIn);
	compareIn = sin(0 * M_PI / 180);
	diff = abs(doubleIn - compareIn);
	if (diff < 0.01)
	{
		solveSin[1] = 1;//1��ʾ0����ͨ��
	}

	//3����������
	sIn = Sin::SinTest(562.98);//562.98����
	doubleIn = strDou(sIn);
	compareIn = sin(562.98 * M_PI / 180);
	diff = abs(doubleIn - compareIn);
	if (diff < 0.01)
	{
		solveSin[2] = 1;//1��ʾ562.98����ͨ��
	}

	//------------------2.cos��������------------------
	//1����������
	sIn = Cos::CinTest(-30.5);//-30.5����
	doubleIn = strDou(sIn);
	compareIn = cos(-30.5 * M_PI / 180);
	diff = abs(doubleIn - compareIn);
	if (diff < 0.01)
	{
		solveCin[0] = 1;//1��ʾ-30.5����ͨ��
	}
	//2��0����
	sIn = Cos::CinTest(0);//0����
	doubleIn = strDou(sIn);
	compareIn = cos(0 * M_PI / 180);
	diff = abs(doubleIn - compareIn);
	if (diff < 0.01)
	{
		solveCin[1] = 1;//1��ʾ0����ͨ��
	}

	//3����������
	sIn = Cos::CinTest(562.98);//562.98����
	doubleIn = strDou(sIn);
	compareIn = cos(562.98 * M_PI / 180);
	diff = abs(doubleIn - compareIn);
	if (diff < 0.01)
	{
		solveCin[2] = 1;//1��ʾ562.98����ͨ��
	}

	//---------------3.arcsin��������--------------------------
	//1��ȡֵ��Χ�ڸ�������
	sIn = ArcSin::ArcsinTest(-0.922);//-0.922����
	doubleIn = strDou(sIn);
	compareIn = asin(-0.922) * 180 / M_PI;
	diff = abs(doubleIn - compareIn);
	if (diff < 0.01)
	{
		solveArcsin[0] = 1;//1��ʾ-0.922����ͨ��
	}
	//2��0����
	sIn = ArcSin::ArcsinTest(0);//0����
	doubleIn = strDou(sIn);
	compareIn = asin(0) * 180 / M_PI;
	diff = abs(doubleIn - compareIn);
	if (diff < 0.01)
	{
		solveArcsin[1] = 1;//1��ʾ0����ͨ��
	}

	//3��ȡֵ��Χ����������
	sIn = ArcSin::ArcsinTest(0.326);//0.326����
	doubleIn = strDou(sIn);
	compareIn = asin(0.326) * 180 / M_PI;
	diff = abs(doubleIn - compareIn);
	if (diff < 0.01)
	{
		solveArcsin[2] = 1;//1��ʾ0.326����ͨ��
	}

	//4)ȡֵ��Χ���쳣ֵ����
	sIn = ArcSin::ArcsinTest(-1.3);//����

	if (sIn._Equal("error!") )
	{
		solveArcsin[3] = 1;
	}

	//---------------4.arctan��������---------------------------
	//1����������
	sIn = ArcTan::ArctanTest(-9.22);//-0.922����
	doubleIn = strDou(sIn);
	compareIn = atan(-9.22) * 180 / M_PI;
	diff = abs(doubleIn - compareIn);
	if (diff < 0.01)
	{
		solveArctan[0] = 1;//1��ʾ-0.922����ͨ��
	}
	//2��0����
	sIn = ArcTan::ArctanTest(0);//0����
	doubleIn = strDou(sIn);
	compareIn = atan(0) * 180 / M_PI;
	diff = abs(doubleIn - compareIn);
	if (diff < 0.01)
	{
		solveArctan[1] = 1;//1��ʾ0����ͨ��
	}

	//3����������
	sIn = ArcTan::ArctanTest(34.56);//34.56����
	doubleIn = strDou(sIn);
	compareIn = atan(34.56) * 180 / M_PI;
	diff = abs(doubleIn - compareIn);
	if (diff < 0.01)
	{
		solveArctan[2] = 1;//1��ʾ34.56����ͨ��
	}

	//�����Խ������result��
	if ((solveSin[0] == 1) & (solveSin[1] == 1) & (solveSin[2] == 1))
		answer += "1)sinTest�̣�";
	else
	{
		answer += "1)sinTest����";
	}

	if ((solveCin[0] == 1) & (solveCin[1] == 1) & (solveCin[2] == 1))
		answer += " 2)cosTest�̣�";
	else
	{
		answer += " 2)cosTest����";
	}

	if ((solveArcsin[0] == 1) & (solveArcsin[1] == 1) & (solveArcsin[2] == 1) & (solveArcsin[3] == 1))
		answer += " 3)arcsinTest�̣�";
	else
	{
		answer += " 3)arcsinTest����";
	}

	if ((solveArctan[0] == 1) & (solveArctan[1] == 1) & (solveArctan[2] == 1))
		answer += " 4)arctanTest�̣�";
	else
	{
		answer += " 4)arctanTest����";
	}

	//answer = "ͨ�����ԣ�";
	return answer;
}


void TestClick(Screen* pSc)
{
	//Test֮�󲻿��ٽ������Ǻ����ļ��㣬��������Զ��˳�
	//sNum = "";
	sEnter = "";
	sResult= testAll();
	//sTriangle = "test";//����֮����뵥����ֵ�ļ��㣬�����Զ��˳�
}



