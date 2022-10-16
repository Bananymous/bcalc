#include "Program.h"

#include <cstdio>
#include <cstring>

#include <sstream>

#include <ncurses.h>

int GetChar()
{
	int c = getch();

	if (isprint(c))
		return c;

	if (c == 10)
		return KEY_ENTER;

	if (c == 127)
		return KEY_BACKSPACE;

	if (c != 27)
		return ERR;

	switch (c = getch())
	{
		case 91: // Arrow
			switch (c = getch())
			{
				case 65: return KEY_UP;
				case 66: return KEY_DOWN;
				case 67: return KEY_RIGHT;
				case 68: return KEY_LEFT;
				default: break;
			}
			break;
	}

	return ERR;
}

int ProgramLoop()
{
	WINDOW* window = initscr();
	if (!window || noecho() == ERR)
	{
		fprintf(stderr, "Could not initialize ncurses.\n");
		return 1;
	}

	std::vector<std::string> inputs;
	
	bcalc::Program program;

	while (true)
	{
		std::vector<std::string> copy = inputs;
		std::size_t index = copy.size();
		std::size_t pos = 0;
		copy.push_back("");

		while (true)
		{
			int c = GetChar();

			if (c == KEY_ENTER)
			{
				move(getcury(window) + 1, 0);
				break;
			}

			if (isprint(c))
			{
				copy[index].insert(pos++, 1, c);
			}
			else if (c == KEY_BACKSPACE)
			{
				if (pos > 0)
					copy[index].erase(--pos, 1);
			}
			else if (c == KEY_UP || c == KEY_DOWN)
			{
				if (c == KEY_UP && index > 0)
					index--;
				else if (c == KEY_DOWN && index < copy.size() - 1)
					index++;
				else
					continue;
				pos = copy[index].size();
			}
			else if (c == KEY_LEFT || c == KEY_RIGHT)
			{
				if (c == KEY_LEFT && pos > 0)
					pos--;
				else if (c == KEY_RIGHT && pos < copy[index].size())
					pos++;
			}

			int y = getcury(window);

			deleteln();
			mvaddstr(y, 0, copy[index].c_str());
			move(y, pos);
		}

		const std::string& input = copy[index];

		if (input.empty())
			continue;

		if (input == "exit")
			break;

		if (input == "clear")
		{
			clear();
			move(0, 0);
			continue;
		}

		auto result = program.Process(input);
		if (result.has_error)
			printw("Invalid input\n");	
		else if (result.has_value)
			printw(" = %s\n", bcalc::complex_to_string(result.value).c_str());

		index++;
		inputs.push_back(input);
	}

	if (endwin() == ERR)
	{
		fprintf(stderr, "Could not close ncurses.\n");
		return 1;
	}

	return 0;
}

int main(int argc, char** argv)
{
	if (argc == 1)
		return ProgramLoop();

	std::string input_str;
	for (int i = 1; i < argc; i++)
		input_str += argv[i];
	std::string_view input = input_str;

	bcalc::Program program;

	std::size_t s = 0;
	while (true)
	{
		std::size_t e = input.find(';', s);

		auto expr = input.substr(s, e - s);
		auto result = program.Process(expr);
		if (result.has_error)
			printf("Invalid input\n");
		else if (result.has_value && expr.find('=') == std::string_view::npos)
			printf(" = %s\n", bcalc::complex_to_string(result.value).c_str());

		if (e == std::string_view::npos)
			break;
		s = e + 1;
	}

	return 0;
}