#include "Token.h"

namespace bcalc
{

	Token::Token(TokenType type, std::any value)
		: m_type(type), m_value(value)
	{}

	Token Token::CreateValue(std::complex<value_type> value)
	{
		return Token { TokenType::Value, std::move(value) };
	}

	Token Token::CreateString(std::string string)
	{
		return Token { TokenType::String, std::move(string) };
	}

	Token Token::CreateBuiltinFunction(FunctionType function)
	{
		return Token { TokenType::BuiltinFunction, function };
	}

	Token Token::CreateConstant(Constant constant)
	{
		return Token { TokenType::Constant, constant };
	}

	Token Token::Create(TokenType type)
	{
		return Token { type };
	}

	std::string Token::to_string() const
	{
		static_assert(static_cast<int>(TokenType::Count) == 13);

		switch (m_type)
		{
			case TokenType::Value:
				return "Value, " + complex_to_string(GetValue());
			case TokenType::Constant:
				return "Constant, " + s_constant_to_string.at(GetConstant());
			case TokenType::String:
				return "String, " + std::any_cast<std::string>(m_value);
			case TokenType::Comma:
				return "Comma";
			case TokenType::Equals:
				return "Equals";
			case TokenType::BuiltinFunction:
				return "Function, " + s_function_to_string.at(GetBuiltinFunction());
			case TokenType::LParan:
				return "LParan";
			case TokenType::RParan:
				return "RParan";
			case TokenType::Mult:
				return "Mult";
			case TokenType::Div:
				return "Div";
			case TokenType::Add:
				return "Add";
			case TokenType::Sub:
				return "Sub";
			case TokenType::Power:
				return "Power";
		}

		return "";
	}

}