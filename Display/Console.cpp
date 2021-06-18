#include "Console.h"

namespace scr_print
{
	using std::mutex;
	using std::string;
	using std::stringstream;
	mutex Screen::_muStream;

	Screen::Screen(pos sr, pos sc, pos ht, pos wd, WORD bc, WORD fc, bool cur) :start_c(sc), start_r(sr),
		height(ht), width(wd),
		backColor(bc), fontColor(fc) {
		hout = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleCursorInfo(hout, &CursorInfo);
		CursorInfo.bVisible = cur;
		Clear();
		coord.X = sc;
		coord.Y = sr;
	}
	Screen& Screen::display(const char* s, ...)
	{
		va_list vag;
		va_start(vag, s);
		_muStream.lock();
		SetScreenFeature();
		for (int i = 0; s[i] != '\0'; i++)
		{
			if (s[i] == '\%')
			{
				do_display('%');
				continue;
			}
			if (s[i] != '%')
				do_display(s[i]);
			else
			{
				stringstream ss;
				i++;
				switch (s[i])
				{
				case 'd':
				case 'D':
					ss << va_arg(vag, int);
					break;
				case 'f':
				case 'F':
					ss << va_arg(vag, double);
					break;
				case 'c':
				case 'C':
					ss << va_arg(vag, char);
					break;
				case 's':
				case 'S':
					ss << va_arg(vag, char*);
					break;
				default:
					ss << '%';
					i--;
					break;
				}
				string str = ss.str();
				for (auto& c : str)
				{
					do_display(c);
				}
			}
		}
		va_end(vag);

		_muStream.unlock();
		return *this;
	}
	Screen& Screen::display(const string& s)
	{
		_muStream.lock();
		SetScreenFeature();
		for (auto& c : s)
		{
			do_display(c);
		}
		_muStream.unlock();
		return *this;
	}
	Screen& Screen::SetPos(pos r, pos c)
	{
		coord.X = start_c + c;
		coord.Y = start_r + r;
		return *this;
	}
	Screen& Screen::SetColor(WORD bc, WORD fc)
	{
		backColor = bc;
		fontColor = fc;
		return *this;
	}
	Screen& Screen::SetToNextLine()
	{
		coord.X = start_c;
		coord.Y += 1;
		if (coord.Y >= start_r + height)
			coord.Y = start_r;
		return *this;
	}
	Screen& Screen::Clear()
	{
		_muStream.lock();
		SetPos(0, 0);
		SetScreenFeature();
		for (int i = 0; i < height * width; i++)
		{
			do_display(' ');
		}
		_muStream.unlock();
		return *this;
	}


	COORD GetConsoleCursorPosition()
	{
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO bInfo;
		GetConsoleScreenBufferInfo(hout, &bInfo);
		COORD coord;
		coord.X = bInfo.dwCursorPosition.X;
		coord.Y = bInfo.dwCursorPosition.Y;
		return coord;
	}

	void ProgressBar(COORD coord, WORD bc, WORD fc, pos width, unsigned int completed, unsigned int totalprogress
		, char c, char nu)
	{
		unsigned int progress = (unsigned int)(100 * (1.0 * completed / totalprogress));
		if (progress > 100)
			return;
		unsigned int mid_num = width / 2;
		unsigned int com_num = width * (1.0 * completed / totalprogress);
		char str_pro[5];
		sprintf_s(str_pro, "%d%%", progress);
		Screen::_muStream.lock();
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hout, coord);
		SetConsoleTextAttribute(hout, bc | fc);
		if (com_num < mid_num - 1)
		{
			string pro(com_num, c);
			string mid(mid_num - 2 - com_num, nu);
			string fin(progress < 10 ? width - mid_num : width - mid_num - 1, nu);
			std::cout<< pro << mid << str_pro << fin << std::endl;
		}
		else if (com_num > mid_num + 1)
		{
			string pro(mid_num - 2, c);
			string mid(progress == 100 ? com_num - mid_num - 2 : com_num - mid_num - 1, c);
			string fin(progress == 100 ? 0 : width - com_num, nu);
			std::cout << pro << str_pro << mid << fin << std::endl;
		}
		else
		{
			string pro(mid_num - 2, c);
			string fin(width - mid_num - 1, nu);
			std::cout << pro << str_pro << fin << std::endl;
		}
		Screen::_muStream.unlock();
	}


	void ProgressBar(Screen& screen, pos sr, pos sc, pos width, unsigned int completed, unsigned int totalprogress
		, char c, char nu)
	{
		unsigned int progress =(unsigned int)( 100 * (1.0 * completed / totalprogress));
		if (progress > 100)
			return;
		unsigned int mid_num = width / 2;
		unsigned int com_num = width * (1.0 * completed / totalprogress);
		char str_pro[5];
		sprintf_s(str_pro, "%d%%", progress);
		if (com_num < mid_num - 1)
		{
			string pro(com_num, c);
			string mid(mid_num - 2 - com_num, nu);
			string fin(progress < 10 ? width - mid_num : width - mid_num - 1, nu);
			screen.SetPos(sr, sc) << pro << mid << str_pro << fin << endl;
		}
		else if (com_num > mid_num + 1)
		{
			string pro(mid_num - 2, c);
			string mid(progress == 100 ? com_num - mid_num - 2 : com_num - mid_num - 1, c);
			string fin(progress == 100 ? 0 : width - com_num, nu);
			screen.SetPos(sr, sc) << pro << str_pro << mid << fin << endl;
		}
		else
		{
			string pro(mid_num - 2, c);
			string fin(width - mid_num - 1, nu);
			screen.SetPos(sr, sc) << pro << str_pro << fin << endl;
		}
	}







}