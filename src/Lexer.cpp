#include "Lexer.h"

#include <cstdlib>
#include <cstdio>

namespace bcalc
{

	std::vector<Token> Lexer::Tokenize(std::string_view data)
	{
		std::vector<Token> result;

		for (uint64_t i = 0; i < data.size(); i++)
		{
			if (isspace(data[i]))
				continue;

			if (isdigit(data[i]))
			{
				std::size_t len;

				long double val = std::stold(data.data() + i, &len);
				//uint64_t val = std::stoull(data.data() + i, &len);
				result.push_back(Token::CreateValue(val));
				i += len - 1;
				continue;
			}

			if (isalpha(data[i]))
			{
				uint64_t len = 1;
				while (i + len < data.size() && isalpha(data[i + len])) len++;
				std::string val(data.data() + i, len);

				if (s_string_to_function.find(val) != s_string_to_function.end())
					result.push_back(Token::CreateFunction(s_string_to_function.at(val)));
				else if (s_string_to_constant.find(val) != s_string_to_constant.end())
					result.push_back(Token::CreateConstant(s_string_to_constant.at(val)));
				else
					result.push_back(Token::CreateString(val));
				i += len - 1;
				continue;
			}

			if (s_char_to_token.find(data[i]) != s_char_to_token.end())
				result.push_back(Token::Create(s_char_to_token.at(data[i])));
			else
			{
				fprintf(stderr, "Unknown character\n", data[i]);
				return {};
			}
		}

		return result;
	}

}