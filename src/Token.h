#pragma once

#include <any>
#include <cassert>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>

namespace bcalc
{

	enum class FunctionType
	{
		Sin,
		Cos,
		Tan,
		ArcSin,
		ArcCos,
		ArcTan,
		Sqrt,
		Log,
		Exp,
		Round,
		Floor,
		Ceil,
		Count
	};
	static const std::unordered_map<std::string, FunctionType> s_string_to_function
	{
		{ "sin",    FunctionType::Sin    },
		{ "arcsin", FunctionType::ArcSin },
		{ "cos",    FunctionType::Cos    },
		{ "arccos", FunctionType::ArcCos },
		{ "tan",    FunctionType::Tan    },
		{ "arctan", FunctionType::ArcTan },
		{ "sqrt",   FunctionType::Sqrt   },
		{ "log",    FunctionType::Log    },
		{ "exp",    FunctionType::Exp    },
		{ "round",  FunctionType::Round  },
		{ "floor",  FunctionType::Floor  },
		{ "ceil",   FunctionType::Ceil   },
	};
	static const std::unordered_map<FunctionType, std::string> s_function_to_string
	{
		{ FunctionType::Sin,    "sin"    },
		{ FunctionType::ArcSin, "arcsin" },
		{ FunctionType::Cos,    "cos"    },
		{ FunctionType::ArcCos, "arccos" },
		{ FunctionType::Tan,    "tan"    },
		{ FunctionType::ArcTan, "arctan" },
		{ FunctionType::Sqrt,   "sqrt"   },
		{ FunctionType::Log,    "log"    },
		{ FunctionType::Exp,    "exp"    },
		{ FunctionType::Round,  "round"  },
		{ FunctionType::Floor,  "floor"  },
		{ FunctionType::Ceil,   "ceil"   },
	};

	enum class Constant
	{
		pi,
		e,
		Count
	};
	static const std::unordered_map<std::string, Constant> s_string_to_constant
	{
		{ "pi", Constant::pi },
		{ "e",  Constant::e  }
	};
	static const std::unordered_map<Constant, std::string> s_constant_to_string
	{
		{ Constant::pi, "pi" },
		{ Constant::e,  "e"  },
	};

	enum class TokenType
	{
		Value,
		Constant,
		String,
		Comma,
		Function,
		LParan,
		RParan,
		Mult,
		Div,
		Add,
		Sub,
		Power,
		Count
	};
	static const std::unordered_map<char, TokenType> s_char_to_token
	{
		{ ',', TokenType::Comma },
		{ '(', TokenType::LParan },
		{ ')', TokenType::RParan },
		{ '*', TokenType::Mult },
		{ '/', TokenType::Div },
		{ '+', TokenType::Add },
		{ '-', TokenType::Sub },
		{ '^', TokenType::Power },
	};

	class Token
	{
	public:
		using value_type = long double;

	public:
		static Token CreateValue(value_type value)
		{
			return Token { TokenType::Value, value };
		}

		static Token CreateString(std::string string)
		{
			return Token { TokenType::String, std::move(string) };
		}

		static Token CreateFunction(FunctionType function)
		{
			return Token { TokenType::Function, function };
		}

		static Token CreateConstant(Constant constant)
		{
			return Token { TokenType::Constant, constant };
		}

		static Token Create(TokenType type)
		{
			assert( type != TokenType::Count &&
					type != TokenType::Value &&
					type != TokenType::String &&
					type != TokenType::Function
			);

			return Token { type };
		}

		std::string to_string() const
		{
			static_assert(static_cast<int>(TokenType::Count) == 12);

			switch (m_type)
			{
				case TokenType::Value:
					return "Value, " + std::to_string(GetValue());
				case TokenType::Constant:
					return "Constant, " + s_constant_to_string.at(GetConstant());
				case TokenType::String:
					return "String, " + std::any_cast<std::string>(m_value);
				case TokenType::Comma:
					return "Comma";
				case TokenType::Function:
					return "Function, " + s_function_to_string.at(GetFunction());
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

		TokenType Type() const { return m_type; }
		value_type GetValue() const
		{
			assert(m_type == TokenType::Value);
			return std::any_cast<value_type>(m_value);
		}
		Constant GetConstant() const
		{
			assert(m_type == TokenType::Constant);
			return std::any_cast<Constant>(m_value);
		}
		FunctionType GetFunction() const
		{
			assert(m_type == TokenType::Function);
			return std::any_cast<FunctionType>(m_value);
		}

	private:
		Token(TokenType type, std::any value = std::any())
			: m_type(type), m_value(value)
		{}

	private:
		TokenType	m_type	= TokenType::Count;
		std::any 	m_value;
	};

}