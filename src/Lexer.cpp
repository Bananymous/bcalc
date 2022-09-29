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
				result.push_back(Token::CreateValue(val));
				i += len - 1;
				continue;
			}

			if (isalpha(data[i]))
			{
				uint64_t len = 1;
				while (i + len < data.size() && isalpha(data[i + len])) len++;
				std::string val(data.data() + i, len);

				if (decltype(s_string_to_function)::const_iterator it = s_string_to_function.find(val); it != s_string_to_function.end())
					result.push_back(Token::CreateFunction(it->second));
				else if (decltype(s_string_to_constant)::const_iterator it = s_string_to_constant.find(val); it != s_string_to_constant.end())
					result.push_back(Token::CreateConstant(it->second));
				else
					result.push_back(Token::CreateString(val));
				i += len - 1;
				continue;
			}

			if (decltype(s_char_to_token)::const_iterator it = s_char_to_token.find(data[i]); it != s_char_to_token.end())
				result.push_back(Token::Create(it->second));
			else
			{
				fprintf(stderr, "Unknown character\n", data[i]);
				return {};
			}
		}

		return result;
	}

}