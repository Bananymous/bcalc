#pragma once

#include <any>
#include <complex>
#include <string>
#include <unordered_map>

namespace bcalc
{
	using value_type = long double;

	template<typename T>
	static std::string complex_to_string(const std::complex<T>& complex)
	{
		std::stringstream ss;

		if (complex.real() != 0)
		{
			if (complex.imag() != 0)
				ss << complex.real() << (complex.imag() < 0 ? " - " : " + ") << std::abs(complex.imag()) << " i";
			else
				ss << complex.real();
		}
		else
		{
			if (complex.imag() != 0)
				ss << complex.imag() << " i";
			else
				ss << "0";
		}

		return ss.str();
	}

	enum class FunctionType
	{
		Sin, ArcSin, Sinh, ArcSinh,
		Cos, ArcCos, Cosh, ArcCosh,
		Tan, ArcTan, Tanh, ArcTanh,
		Sqrt,
		Log,
		Exp,
		Round, Floor, Ceil,
		Count
	};
	static const std::unordered_map<std::string, FunctionType> s_string_to_function
	{
		{ "sin",     FunctionType::Sin     },
		{ "sinh",    FunctionType::Sinh    },
		{ "asin",    FunctionType::ArcSin  },
		{ "asinh",   FunctionType::ArcSinh },
		{ "arcsin",  FunctionType::ArcSin  },
		{ "arcsinh", FunctionType::ArcSinh },

		{ "cos",     FunctionType::Cos     },
		{ "cosh",    FunctionType::Cosh    },
		{ "acos",    FunctionType::ArcCos  },
		{ "acosh",   FunctionType::ArcCosh },
		{ "arccos",  FunctionType::ArcCos  },
		{ "arccosh", FunctionType::ArcCosh },

		{ "tan",     FunctionType::Tan     },
		{ "tanh",    FunctionType::Tanh    },
		{ "atan",    FunctionType::ArcTan  },
		{ "atanh",   FunctionType::ArcTanh },
		{ "arctan",  FunctionType::ArcTan  },
		{ "arctanh", FunctionType::ArcTanh },

		{ "sqrt",    FunctionType::Sqrt    },
		{ "log",     FunctionType::Log     },
		{ "exp",     FunctionType::Exp     },

		{ "round",   FunctionType::Round   },
		{ "floor",   FunctionType::Floor   },
		{ "ceil",    FunctionType::Ceil    },
	};
	static const std::unordered_map<FunctionType, std::string> s_function_to_string
	{
		{ FunctionType::Sin,     "sin"     },
		{ FunctionType::ArcSin,  "arcsin"  },
		{ FunctionType::Sinh,    "sinh"    },
		{ FunctionType::ArcSinh, "arcsinh" },

		{ FunctionType::Cos,     "cos"     },
		{ FunctionType::ArcCos,  "arccosh" },
		{ FunctionType::Cosh,    "cosh"    },
		{ FunctionType::ArcCosh, "arccos"  },

		{ FunctionType::Tan,     "tan"     },
		{ FunctionType::ArcTan,  "arctanh" },
		{ FunctionType::Tanh,    "tanh"    },
		{ FunctionType::ArcTanh, "arctan"  },

		{ FunctionType::Sqrt,    "sqrt"    },
		{ FunctionType::Log,     "log"     },
		{ FunctionType::Exp,     "exp"     },
		
		{ FunctionType::Round,   "round"   },
		{ FunctionType::Floor,   "floor"   },
		{ FunctionType::Ceil,    "ceil"    },
	};

	enum class Constant
	{
		pi,
		e,
		i,
		Count
	};
	static const std::unordered_map<std::string, Constant> s_string_to_constant
	{
		{ "pi", Constant::pi },
		{ "e",  Constant::e  },
		{ "i",  Constant::i  },
	};
	static const std::unordered_map<Constant, std::string> s_constant_to_string
	{
		{ Constant::pi, "pi" },
		{ Constant::e,  "e"  },
		{ Constant::i,  "i"  },
	};

	enum class TokenType
	{
		Value,
		Constant,
		String,
		Comma,
		Equals,
		BuiltinFunction,
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
		{ ',', TokenType::Comma  },
		{ '=', TokenType::Equals },
		{ '(', TokenType::LParan },
		{ ')', TokenType::RParan },
		{ '*', TokenType::Mult   },
		{ '/', TokenType::Div    },
		{ '+', TokenType::Add    },
		{ '-', TokenType::Sub    },
		{ '^', TokenType::Power  },
	};

	class Token
	{
	public:
		static Token CreateValue(std::complex<value_type> value);
		static Token CreateString(std::string string);
		static Token CreateBuiltinFunction(FunctionType function);
		static Token CreateConstant(Constant constant);
		static Token Create(TokenType type);

		std::string to_string() const;

		TokenType Type()					const { return m_type; }
		std::complex<value_type> GetValue()	const { return std::any_cast<std::complex<value_type>>(m_value); }
		Constant GetConstant()				const { return std::any_cast<Constant>(m_value); }
		FunctionType GetBuiltinFunction()	const { return std::any_cast<FunctionType>(m_value); }
		std::string GetString()				const { return std::any_cast<std::string>(m_value); }

	private:
		Token(TokenType type, std::any value = std::any());

	private:
		TokenType	m_type	= TokenType::Count;
		std::any 	m_value;
	};

}