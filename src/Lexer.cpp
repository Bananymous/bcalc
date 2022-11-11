#include "Lexer.h"

#include <cctype>
#include <charconv>

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
				value_type value;
				auto [ptr, _] = std::from_chars(data.data() + i, data.data() + data.size(), value);
				result.push_back(Token::CreateValue(value));
				i = ptr - data.data() - 1;
				continue;
			}

			if (isalpha(data[i]))
			{
				uint64_t len = 1;
				while (i + len < data.size() && (isalpha(data[i + len]) || isdigit(data[i + len]))) len++;
				std::string val(data.data() + i, len);

				if (!result.empty())
				{
					auto last = result.back().Type();
					if (last == TokenType::Value || last == TokenType::String || last == TokenType::Constant || last == TokenType::BuiltinFunction)
						result.push_back(Token::Create(TokenType::Mult));
				}

				if (auto it = s_string_to_function.find(val); it != s_string_to_function.end())
					result.push_back(Token::CreateBuiltinFunction(it->second));
				else if (auto it = s_string_to_constant.find(val); it != s_string_to_constant.end())
					result.push_back(Token::CreateConstant(it->second));
				else
					result.push_back(Token::CreateString(val));
				i += len - 1;
				continue;
			}

			if (auto it = s_char_to_token.find(data[i]); it != s_char_to_token.end())
				result.push_back(Token::Create(it->second));
			else
			{
				return {};
			}
		}

		return result;
	}

}
