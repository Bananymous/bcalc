#include "TokenNode.h"

#include <cmath>

namespace bcalc
{

	static long double EvaluateConstant(Constant constant)
	{
		static_assert(static_cast<int>(Constant::Count) == 2);

		switch (constant)
		{
			case Constant::pi:
				return M_PIl;
			case Constant::e:
				return M_El;
		}

		assert(false);
	}

	static bool EvaluateFunction(FunctionType function, const std::vector<TokenNode*>& nodes, const std::unordered_map<std::string, long double>& variables, long double& out_result)
	{
		static_assert(static_cast<int>(FunctionType::Count) == 12);

		std::vector<long double> inputs;
		for (TokenNode* node : nodes)
		{
			inputs.push_back(0.0L);
			if (!node->approximate(variables, inputs.back()))
				return false;
		}

		switch (function)
		{
			case FunctionType::Sin:
				if (inputs.size() != 1)
					return false;
				out_result = sinl(inputs[0]);
				return true;
			case FunctionType::ArcSin:
				if (inputs.size() != 1)
					return false;
				out_result = asinl(inputs[0]);
				return true;
			case FunctionType::Cos:
				if (inputs.size() != 1)
					return false;
				out_result = cosl(inputs[0]);
				return true;
			case FunctionType::ArcCos:
				if (inputs.size() != 1)
					return false;
				out_result = acosl(inputs[0]);
				return true;
			case FunctionType::Tan:
				if (inputs.size() != 1)
					return false;
				out_result = tanl(inputs[0]);
				return true;
			case FunctionType::ArcTan:
				if (inputs.size() != 1)
					return false;
				out_result = atanl(inputs[0]);
				return true;
			case FunctionType::Sqrt:
				if (inputs.size() != 1)
					return false;
				out_result = sqrtl(inputs[0]);
				return true;
			case FunctionType::Log:
				if (inputs.size() == 1)
					out_result = logl(inputs[0]);
				else if (inputs.size() == 2)
					out_result = logl(inputs[0]) / logl(inputs[1]);
				else
					return false;
				return true;
			case FunctionType::Exp:
				if (inputs.size() != 1)
					return false;
				out_result = expl(inputs[0]);
				return true;
			case FunctionType::Round:
				if (inputs.size() != 1)
					return false;
				out_result = roundl(inputs[0]);
				return true;
			case FunctionType::Floor:
				if (inputs.size() != 1)
					return false;
				out_result = floorl(inputs[0]);
				return true;
			case FunctionType::Ceil:
				if (inputs.size() != 1)
					return false;
				out_result = ceill(inputs[0]);
				return true;
			}

		return false;
	}

	TokenNode::TokenNode(Token token, std::vector<TokenNode*> nodes)
		: m_nodes(std::move(nodes))
		, m_token(token)
	{}

	bool TokenNode::approximate(const std::unordered_map<std::string, long double>& variables, long double& out_result) const
	{
		if (m_token.Type() == TokenType::Value)
		{
			out_result = m_token.GetValue();
			return true;
		}

		if (m_token.Type() == TokenType::Constant)
		{
			out_result = EvaluateConstant(m_token.GetConstant());
			return true;
		}

		if (m_token.Type() == TokenType::String)
		{
			if (variables.find(m_token.GetString()) == variables.end())
				return false;
			out_result = variables.at(m_token.GetString());
			return true;
		}

		if (m_token.Type() == TokenType::Function)
		{
			return EvaluateFunction(m_token.GetFunction(), m_nodes, variables, out_result);
		}

		if (m_nodes.size() != 2)
			return false;

		long double lhs, rhs;
		m_nodes[0]->approximate(variables, lhs);
		m_nodes[1]->approximate(variables, rhs);

		switch (m_token.Type())
		{
			case TokenType::Add:	out_result = lhs + rhs;			return true;
			case TokenType::Sub:	out_result = lhs - rhs;			return true;
			case TokenType::Mult:	out_result = lhs * rhs;			return true;
			case TokenType::Div:	out_result = lhs / rhs;			return true;
			case TokenType::Power:	out_result = powl(lhs, rhs);	return true;
		}

		return false;
	}

	std::string TokenNode::to_string(uint64_t indent) const
	{
		std::string result;
		result.append(indent, ' ');
		result += m_token.to_string();
		result += '\n';
		for (TokenNode* node : m_nodes)
			result += node->to_string(indent + 2);
		return result;
	}

}