#include "CalWin.h"
#include <thread>
#include "MathSin.h"
#include "MathCos.h"
#include "MathAsin.h"
#include "MathAtan.h"


using scr_print::Screen;
using scr_print::endl;
using scr_print::ends;

/*
* REG: Screen注册表，用于自动生成（宏展开）为Screen指针的定义、初始化、点击响应函数的声明与调用
* 每一行第一个信息为名字，接着7个参数为Screen初始化参数，
* 第9个参数为Screen初始显示字符串，第10个参数为按键点击响应函数
* *********************************************************************************************
* 增加Screen只需要增添该注册表项，并实现对应的点击响应函数。
* 其余指针变量定义，初始化，响应函数调用，均在宏展开中自动完成。
*/
#define REG(x)\
x(Enter, 0, 0, 1, 41, BACKGROUND_WHITE, 0, false,"",EnterClick)\
x(Result,1, 0, 1, 41, BACKGROUND_WHITE, 0, false,"",ResultClick)\
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
x(Back, 19, 11, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    ←", BackClick)\
x(Clr,  19, 21, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    AC", ClrClick)\
x(Equal,19, 31, 3, 9, BACKGROUND_GRAY, FONT_WHITE + 8, false,"\n    =",  EqualClick)


/*
* REG(DEF): 宏展开为
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
* REG(INIT): 宏展开为
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
* CLICK_CALLBACK: 宏展开为
*		while(true){
*			if(posX>=0&&posX<0+41&&posY>=0&&posY<0+1)
*				{EnterClick(pScreenEnter);break;}
*			...
*			if(posX>=31&&posX<31+9&&posY>=19&&posY<19+3)
*				{EqualClick(pScreenEqual);break;}
*			break;
*		}
* ***********************************************************
* 根据位置调用对应的点击响应函数，若函数实体未实现，编译报错
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
* REG(DEL): 宏展开为
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

void CalWinInit()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
#if 0
	int width = 41, height = 23;
	COORD coord = { width ,height };
	SMALL_RECT wrt = { 0,0,width - 1,height - 1 };
	SetConsoleScreenBufferSize(hStdin, coord);//设置缓冲区尺寸
	SetConsoleWindowInfo(hStdin, TRUE, &wrt);//设置窗体尺寸
#endif 
	SetConsoleTitle("计算器");
	
	SetWindowLongPtrA(
		GetConsoleWindow(),
		GWL_STYLE,
		GetWindowLongPtrA(GetConsoleWindow(),
			GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX
	);
#if 0
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;  //移除快速编辑模式
	mode &= ~ENABLE_INSERT_MODE;      //移除插入模式
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
			sEnter = sTriangle + '(' + sNum + ')';
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
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE); // 获取标准输入设备句柄
	INPUT_RECORD inRec;
	DWORD res;
	std::thread dealT(DealTask);
	while (!stopflag)
	{
		ReadConsoleInput(hInput, &inRec, 1, &res);
		if (inRec.EventType == MOUSE_EVENT && inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) //鼠标左键
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
}
void CosClick(Screen* pSc)
{
	sTriangle = "cos";
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
				so << math::sin(Num);
			}
			else if (sTriangle == "cos")
			{
				so << math::cos(Num);
			}
			else if (sTriangle == "asin")
			{
				if (Num >= -1 && Num <= 1)
					so << math::asin(Num);
				else
					so << "Error!";
			}
			else if (sTriangle == "atan")
			{
				so << math::atan(Num);
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




