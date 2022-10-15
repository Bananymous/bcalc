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

	Program::ProcessResult Program::Process(std::string_view input)
	{
		auto tokens = Lexer::Tokenize(input);
		if (tokens.empty())
			return ProcessResult { .has_error = true };

		// Assignment
		if (auto it = std::find_if(tokens.begin(), tokens.end(), [](const auto& token) { return token.Type() == TokenType::Equals; }); it != tokens.end())
		{
			if (tokens.size() <= 2 && tokens.front().Type() != TokenType::String)
				return ProcessResult { .has_error = true };
			
			// Variable
			if (it == tokens.begin() + 1)
			{
				TokenNode* root = Parser::BuildTokenTree(it + 1, tokens.end());
				if (!root)
					return ProcessResult { .has_error = true };
				
				auto result = root->approximate(m_variables);
				delete root;

				if (result.has_error)
					return ProcessResult { .has_error = true };
				
				m_variables[tokens[0].GetString()] = result.value;
				return ProcessResult { .has_value = true, .value = result.value };
			}
			// Function
			else
			{
				assert(false && "Not implemented");
			}
		}
		// Expression
		else
		{
			TokenNode* root = Parser::BuildTokenTree(tokens.begin(), tokens.end());
			if (!root)
				return ProcessResult { .has_error = true };
			
			auto result = root->approximate(m_variables);
			delete root;

			if (result.has_error)
				return ProcessResult { .has_error = true };
			return ProcessResult { .has_value = true, .value = result.value };
		}
	}

}