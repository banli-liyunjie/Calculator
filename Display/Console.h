#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <mutex>

#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_WHITE     0x0070
#define BACKGROUND_GRAY      0x0080 // background color contains intensified.

#define FONT_BLUE            0x0001
#define FONT_GREEN           0x0002
#define FONT_RED             0x0004
#define FONT_PURPLE          0x0005
#define FONT_YELLOW          0x0006
#define FONT_WHITE           0x0007
#define FONT_GRAY            0x0008
//the Color+8 will be brighter.



namespace scr_print
{
	typedef unsigned int pos;
	class Screen
	{
	public:
		//构造函数，sr:起点行数 sc:起点列数 ht:屏幕高度 wd:屏幕宽度 bc:背景颜色 fc:字体颜色 cur:是否显示光标
		Screen(pos sr, pos sc, pos ht, pos wd, WORD bc, WORD fc, bool cur);
		//类printf功能，调用如: display("a=%d,b=%f",2,3.5); 显示为 a=2,b=3.5
		Screen& display(const char* s, ...);
		Screen& display(const std::string& s);
		//设置光标位置
		Screen& SetPos(pos r, pos c);
		//设置背景与字体颜色
		Screen& SetColor(WORD bc, WORD fc);
		//换行
		Screen& SetToNextLine();
		Screen& Clear();
		pos GetPosX() { return start_c; }
		pos GetPosY() { return start_r; }
		pos GetWidth() { return width; }
		pos GetHeight() { return height; }
		inline Screen& SetCurVisiable(bool visiable)
		{
			cursorInfo.bVisible = visiable;
			return *this;
		}
		//屏幕内输入功能 重置运算符>> 类似与cin
		template<typename _T>
		inline Screen& operator >>(_T& _type)
		{
			_muStream.lock();
			cursorInfo.bVisible = true;
			SetConsoleCursorInfo(hout, &cursorInfo);
			SetConsoleCursorPosition(hout, coord);
			SetColor();
			std::cin >> _type;
			SetToNextLine();
			cursorInfo.bVisible = false;
			SetConsoleCursorPosition(hout, coord); // 回车后位置不可知,移动至Screen下一行
			SetConsoleCursorInfo(hout, &cursorInfo);
			_muStream.unlock();
			return *this;
		}
		// 屏幕内输出，重置运算符<< 类似与cout
		template<typename _T>
		inline Screen& operator <<(const _T& _type)
		{
			std::stringstream ss;
			ss << _type;
			return display(ss.str());
		}
		// 屏幕内输出，重置运算符<< 类似与cout,参数为endl，ends(函数指针，该endl,ends与标准函数库中不同,功能相同)
		inline Screen& operator <<(Screen& (*_Pfn)(Screen&))
		{
			return _Pfn(*this);
		}
		/// <summary>
		/// 线程锁，避免屏幕混乱
		/// Screen类本质为在控制台中划分区域，修改光标位置实现屏幕输出功能。
		/// 为避免多线程使用时屏幕混乱，加锁
		/// </summary>
		static std::mutex _muStream;
	private:
		// 默认初始化参数都为0，无意义，故不允许调用默认构造函数
		Screen() = default;
		inline void SetColor()
		{
			SetConsoleTextAttribute(hout, backColor | fontColor);
		};
		inline void SetScreenFeature()
		{
			SetConsoleCursorInfo(hout, &cursorInfo);
			SetConsoleCursorPosition(hout, coord);
			SetColor();
		}
		inline void do_display(const char& c)
		{
			if (c == '\n')
			{
				SetToNextLine();
				SetConsoleCursorPosition(hout, coord);
				return;
			}
			std::cout << c;
			coord.X++;
			if (coord.X >= start_c + width)
			{
				coord.X = start_c;
				coord.Y++;
				if (coord.Y >= start_r + height)
				{
					coord.Y = start_r;
				}
				SetConsoleCursorPosition(hout, coord);
			}
		};
		HANDLE hout;
		CONSOLE_CURSOR_INFO cursorInfo;
		COORD coord;
		WORD backColor;
		WORD fontColor;
		const pos height, width;
		const pos start_r, start_c;
	};
	inline Screen& endl(Screen& _screen)
	{
		return _screen.SetToNextLine();
	}
	inline Screen& ends(Screen& _screen)
	{
		return _screen.display(" ");
	}
	COORD GetConsoleCursorPosition();
	void ProgressBar(COORD coord, WORD bc, WORD fc, pos width,
		unsigned int completed, unsigned int totalprogress, char c, char nu);
	void ProgressBar(Screen& screen, pos sr, pos sc, pos width, 
		unsigned int completed, unsigned int totalprogress, char c, char nu);
}
