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

	static TokenNode::ApproximateResult EvaluateFunction(FunctionType function, const std::vector<TokenNode*>& nodes, const std::unordered_map<std::string, TokenNode::value_type>& variables)
	{
		static_assert(static_cast<int>(FunctionType::Count) == 12);

		auto error = TokenNode::ApproximateResult { .has_error = true };

		std::vector<long double> inputs;
		for (TokenNode* node : nodes)
		{
			auto result = node->approximate(variables);
			if (result.has_error)
				return error;
			inputs.push_back(result.value);
		}

		switch (function)
		{
			case FunctionType::Sin:
				if (inputs.size() != 1)
					return error;
				return TokenNode::ApproximateResult { .value = sinl(inputs[0]) };
			case FunctionType::ArcSin:
				if (inputs.size() != 1)
					return error;
				return TokenNode::ApproximateResult { .value = asinl(inputs[0]) };
			case FunctionType::Cos:
				if (inputs.size() != 1)
					return error;
				return TokenNode::ApproximateResult { .value = cosl(inputs[0]) };
			case FunctionType::ArcCos:
				if (inputs.size() != 1)
					return error;
				return TokenNode::ApproximateResult { .value = acosl(inputs[0]) };
			case FunctionType::Tan:
				if (inputs.size() != 1)
					return error;
				return TokenNode::ApproximateResult { .value = tanl(inputs[0]) };
			case FunctionType::ArcTan:
				if (inputs.size() != 1)
					return error;
				return TokenNode::ApproximateResult { .value = atanl(inputs[0]) };
			case FunctionType::Sqrt:
				if (inputs.size() != 1)
					return error;
				return TokenNode::ApproximateResult { .value = sqrtl(inputs[0]) };
			case FunctionType::Log:
				if (inputs.size() == 1)
					return TokenNode::ApproximateResult { .value = logl(inputs[0]) };
				else if (inputs.size() == 2)
					return TokenNode::ApproximateResult { .value = logl(inputs[0]) / logl(inputs[1]) };
				return error;
			case FunctionType::Exp:
				if (inputs.size() != 1)
					return error;
				return TokenNode::ApproximateResult { .value = expl(inputs[0]) };
			case FunctionType::Round:
				if (inputs.size() != 1)
					return error;
				return TokenNode::ApproximateResult { .value = roundl(inputs[0]) };
			case FunctionType::Floor:
				if (inputs.size() != 1)
					return error;
				return TokenNode::ApproximateResult { .value = floorl(inputs[0]) };
			case FunctionType::Ceil:
				if (inputs.size() != 1)
					return error;
				return TokenNode::ApproximateResult { .value = ceill(inputs[0]) };
			}

		return error;
	}

	TokenNode::TokenNode(Token token, std::vector<TokenNode*> nodes)
		: m_nodes(std::move(nodes))
		, m_token(token)
	{}

	TokenNode::ApproximateResult TokenNode::approximate(const std::unordered_map<std::string, value_type>& variables) const
	{
		auto error = ApproximateResult { .has_error = true };

		if (m_token.Type() == TokenType::Value)
			return ApproximateResult { .value = m_token.GetValue() };

		if (m_token.Type() == TokenType::Constant)
			return ApproximateResult { .value = EvaluateConstant(m_token.GetConstant()) };

		if (m_token.Type() == TokenType::String)
		{
			if (variables.find(m_token.GetString()) == variables.end())
				return error;
			return ApproximateResult { .value = variables.at(m_token.GetString()) };
		}

		if (m_token.Type() == TokenType::BuiltinFunction)
			return EvaluateFunction(m_token.GetBuiltinFunction(), m_nodes, variables);
			
		if (m_nodes.size() != 2)
			return error;

		auto lhs = m_nodes[0]->approximate(variables);
		auto rhs = m_nodes[1]->approximate(variables);

		if (lhs.has_error || rhs.has_error)
			return error;

		switch (m_token.Type())
		{
			case TokenType::Add:	return ApproximateResult { .value = lhs.value + rhs.value };
			case TokenType::Sub:	return ApproximateResult { .value = lhs.value - rhs.value };
			case TokenType::Mult:	return ApproximateResult { .value = lhs.value * rhs.value };
			case TokenType::Div:	return ApproximateResult { .value = lhs.value / rhs.value };
			case TokenType::Power:	return ApproximateResult { .value = powl(lhs.value, rhs.value) };
		}

		return error;
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