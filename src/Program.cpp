#include "Program.h"

#include "Lexer.h"
#include "Parser.h"

namespace bcalc
{

	Program::Program()
	{

	}

	Program::~Program()
	{
		for (auto& [name, func] : m_functions)
			delete func.expression;
	}

	CalcResult Program::Process(std::string_view input)
	{
		CalcResult error { .has_error = true };

		auto tokens = Lexer::Tokenize(input);
		if (tokens.empty())
			return error;

		// Assignment
		if (auto eq_it = std::find_if(tokens.begin(), tokens.end(), [](const auto& token) { return token.Type() == TokenType::Equals; }); eq_it != tokens.end())
		{
			if (tokens.size() <= 2 || tokens.front().Type() != TokenType::String || eq_it == tokens.begin())
				return error;
			
			// Variable
			if (eq_it == tokens.begin() + 1)
			{
				TokenNode* root = Parser::BuildTokenTree(eq_it + 1, tokens.end());
				if (!root)
					return error;
				
				auto result = root->approximate(m_variables, m_functions);
				delete root;

				if (result.has_error)
					return error;
				
				m_variables[tokens[0].GetString()] = result.value;
				return { .value = result.value };
			}
			// Function
			else
			{
				if (tokens[1].Type() != TokenType::LParan || (eq_it - 1)->Type() != TokenType::RParan)
					return error;

				std::vector<std::string> parameters;
				auto it = tokens.begin() + 2;
				while (true)
				{
					if (it->Type() == TokenType::RParan)
					{
						if (it != eq_it - 1)
							return error;
						break;
					}

					if (it->Type() != TokenType::String)
						return error;
					parameters.push_back(it->GetString());

					it++;

					if (it->Type() == TokenType::Comma)
						it++;
				}

				TokenNode* root = Parser::BuildTokenTree(eq_it + 1, tokens.end());
				if (!root)
					return error;

				m_functions[tokens[0].GetString()] = { .parameters = std::move(parameters), .expression = root };

				return { .has_value = false };
			}
		}
		// Expression
		else
		{
			TokenNode* root = Parser::BuildTokenTree(tokens.begin(), tokens.end());
			if (!root)
				return error;
			
			auto result = root->approximate(m_variables, m_functions);
			delete root;

			if (result.has_error)
				return error;
			
			return { .value = result.value };
		}
	}

}