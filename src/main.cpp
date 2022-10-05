#include "Lexer.h"
#include "Parser.h"

#include <cstdio>
#include <cstring>
#include <iostream>

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

		auto tokens = bcalc::Lexer::Tokenize(input);
		if (!tokens.empty())
		{
			bcalc::TokenNode* root = bcalc::Parser::BuildTokenTree(tokens);
			if (root)
				printw("= %Lf\n", root->approximate());
			else
				printw("Invalid input\n");
			delete root;
		}
		else
			printw("Invalid input\n");

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

	if (argc < 2)
		return 1;

	bool dump_tree = false;

	for (int i = 1; i < argc - 1; i++)
	{
		if (strncmp(argv[i], "-d", 3) == 0)
			dump_tree = true;
		else
			return 1;
	}

	auto tokens = bcalc::Lexer::Tokenize(argv[argc - 1]);
	if (tokens.empty())
	{
		fprintf(stderr, "Invalid input\n");
		return 1;
	}

#if 0
	for (const auto& token : tokens)
		printf("%s\n", token.to_string().c_str());
#endif

	bcalc::TokenNode* root = bcalc::Parser::BuildTokenTree(tokens);
	if (root == nullptr)
	{
		fprintf(stderr, "Invalid input\n");
		return 1;
	}

	if (dump_tree)
		printf("%s", root->to_string().c_str());

	long double result = root->approximate();
	delete root;

	std::cout << result << std::endl;

	return 0;
}