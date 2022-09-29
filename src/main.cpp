#include "Lexer.h"
#include "Parser.h"

#include <cstdio>
#include <cstring>
#include <iostream>

int main(int argc, char** argv)
{
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
		return 1;

#if 0
	for (const auto& token : tokens)
		printf("%s\n", token.to_string().c_str());
#endif

	bcalc::TokenNode* root = bcalc::Parser::BuildTokenTree(tokens);
	if (root == nullptr)
		return 1;

	if (dump_tree)
		printf("%s", root->to_string().c_str());

	long double result = root->approximate();
	delete root;
	
#if 0
	long double limit = 1e-20L;
	if (-limit <= result && result <= limit)
		result = 0.0L;
#endif

	std::cout << result << std::endl;

	return 0;
}