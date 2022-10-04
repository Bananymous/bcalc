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

	static long double EvaluateFunction(FunctionType function, const std::vector<TokenNode*>& nodes)
	{
		static_assert(static_cast<int>(FunctionType::Count) == 9);

		switch (function)
		{
			case FunctionType::Sin:
				assert(nodes.size() == 1);
				return sinl(nodes[0]->approximate());
			case FunctionType::ArcSin:
				assert(nodes.size() == 1);
				return asinl(nodes[0]->approximate());
			case FunctionType::Cos:
				assert(nodes.size() == 1);
				return cosl(nodes[0]->approximate());
			case FunctionType::ArcCos:
				assert(nodes.size() == 1);
				return acosl(nodes[0]->approximate());
			case FunctionType::Tan:
				assert(nodes.size() == 1);
				return tanl(nodes[0]->approximate());
			case FunctionType::ArcTan:
				assert(nodes.size() == 1);
				return atanl(nodes[0]->approximate());
			case FunctionType::Sqrt:
				assert(nodes.size() == 1);
				return sqrtl(nodes[0]->approximate());
			case FunctionType::Log:
				if (nodes.size() == 1)
					return logl(nodes[0]->approximate());
				if (nodes.size() == 2)
					return logl(nodes[0]->approximate()) / logl(nodes[1]->approximate());
				assert(false);
			case FunctionType::Exp:
				assert(nodes.size() == 1);
				return expl(nodes[0]->approximate());
		}

		assert(false);
	}

	TokenNode::TokenNode(Token token, std::vector<TokenNode*> nodes)
		: m_nodes(std::move(nodes))
		, m_token(token)
	{}

	long double TokenNode::approximate() const
	{
		if (m_token.Type() == TokenType::Value)
			return m_token.GetValue();

		if (m_token.Type() == TokenType::Constant)
			return EvaluateConstant(m_token.GetConstant());

		if (m_token.Type() == TokenType::Function)
			return EvaluateFunction(m_token.GetFunction(), m_nodes);

		assert(m_nodes.size() == 2);
		auto lhs = m_nodes[0]->approximate();
		auto rhs = m_nodes[1]->approximate();

		switch (m_token.Type())
		{
			case TokenType::Add:	return lhs + rhs;
			case TokenType::Sub:	return lhs - rhs;
			case TokenType::Mult:	return lhs * rhs;
			case TokenType::Div:	return lhs / rhs;
			case TokenType::Power:	return powl(lhs, rhs);
		}

		assert(false);
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