#include "TokenNode.h"

#include <numbers>

namespace bcalc
{

	static std::complex<value_type> EvaluateConstant(Constant constant)
	{
		static_assert(static_cast<int>(Constant::Count) == 3);

		switch (constant)
		{
			case Constant::pi:
				return std::numbers::pi_v<value_type>;
			case Constant::e:
				return std::numbers::e_v<value_type>;
			case Constant::i:
				return std::complex<value_type>(0, 1);
		}

		throw;
	}

	static CalcResult EvaluateFunction(FunctionType function, const std::vector<TokenNode*>& nodes, const VariableList& variables, const FunctionList& functions)
	{
		static_assert(static_cast<int>(FunctionType::Count) == 18);

		CalcResult error { .has_error = true };
		
		std::vector<std::complex<value_type>> inputs;
		for (TokenNode* node : nodes)
		{
			auto result = node->approximate(variables, functions);
			if (result.has_error)
				return error;
			inputs.push_back(result.value);
		}

		switch (function)
		{
			case FunctionType::Sin:
				if (inputs.size() != 1)
					return error;
				return { .value = std::sin(inputs[0]) };
			case FunctionType::ArcSin:
				if (inputs.size() != 1)
					return error;
				return { .value = std::asin(inputs[0]) };
			case FunctionType::Sinh:
				if (inputs.size() != 1)
					return error;
				return { .value = std::sinh(inputs[0]) };
			case FunctionType::ArcSinh:
				if (inputs.size() != 1)
					return error;
				return { .value = std::asinh(inputs[0]) };
			case FunctionType::Cos:
				if (inputs.size() != 1)
					return error;
				return { .value = std::cos(inputs[0]) };
			case FunctionType::ArcCos:
				if (inputs.size() != 1)
					return error;
				return { .value = std::acos(inputs[0]) };
			case FunctionType::Cosh:
				if (inputs.size() != 1)
					return error;
				return { .value = std::cosh(inputs[0]) };
			case FunctionType::ArcCosh:
				if (inputs.size() != 1)
					return error;
				return { .value = std::acosh(inputs[0]) };
			case FunctionType::Tan:
				if (inputs.size() != 1)
					return error;
				return { .value = std::tan(inputs[0]) };
			case FunctionType::ArcTan:
				if (inputs.size() != 1)
					return error;
			case FunctionType::Tanh:
				if (inputs.size() != 1)
					return error;
				return { .value = std::tanh(inputs[0]) };
			case FunctionType::ArcTanh:
				if (inputs.size() != 1)
					return error;
				return { .value = std::atanh(inputs[0]) };
			case FunctionType::Sqrt:
				if (inputs.size() != 1)
					return error;
				return { .value = std::sqrt(inputs[0]) };
			case FunctionType::Log:
				if (inputs.size() == 1)
					return { .value = std::log(inputs[0]) };
				else if (inputs.size() == 2)
					return { .value = std::log(inputs[0]) / std::log(inputs[1]) };
				return error;
			case FunctionType::Exp:
				if (inputs.size() != 1)
					return error;
				return { .value = std::exp(inputs[0]) };
			case FunctionType::Round:
				if (inputs.size() != 1)
					return error;
				return { .value = std::complex<value_type>(std::round(inputs[0].real()), std::round(inputs[1].imag())) };
			case FunctionType::Floor:
				if (inputs.size() != 1)
					return error;
				return { .value = std::complex<value_type>(std::floor(inputs[0].real()), std::floor(inputs[1].imag())) };
			case FunctionType::Ceil:
				if (inputs.size() != 1)
					return error;
				return { .value = std::complex<value_type>(std::ceil(inputs[0].real()), std::ceil(inputs[1].imag())) };
			}

		return error;
	}

	TokenNode::TokenNode(Token token, std::vector<TokenNode*> nodes)
		: m_nodes(std::move(nodes))
		, m_token(token)
	{}

	CalcResult TokenNode::approximate(const VariableList& variables, const FunctionList& functions) const
	{
		CalcResult error { .has_error = true };

		if (m_token.Type() == TokenType::Value)
			return { .value = m_token.GetValue() };

		if (m_token.Type() == TokenType::Constant)
			return { .value = EvaluateConstant(m_token.GetConstant()) };

		if (m_token.Type() == TokenType::String)
		{
			if (auto it = variables.find(m_token.GetString()); it != variables.end())
				return { .value = it->second };

			if (auto it = functions.find(m_token.GetString()); it != functions.end())
			{
				const auto& func_overloads = it->second;

				auto overload_it = func_overloads.find(m_nodes.size());
				if (overload_it == func_overloads.end())
					return error;

				const auto& overload = overload_it->second;

				// Add function parameters to variables.
				VariableList parameters = variables;
				for (std::size_t i = 0; i < m_nodes.size(); i++)
				{
					auto result = m_nodes[i]->approximate(variables, functions);
					if (result.has_error)
						return error;
					parameters[overload.parameters[i]] = result.value;
				}

				auto result = overload.expression->approximate(parameters, functions);
				if (result.has_error)
					return error;
				return { .value = result.value };
			}

			return error;
		}

		if (m_token.Type() == TokenType::BuiltinFunction)
			return EvaluateFunction(m_token.GetBuiltinFunction(), m_nodes, variables, functions);
			
		if (m_nodes.size() != 2)
			return error;

		auto lhs = m_nodes[0]->approximate(variables, functions);
		auto rhs = m_nodes[1]->approximate(variables, functions);

		if (lhs.has_error || rhs.has_error)
			return error;

		switch (m_token.Type())
		{
			case TokenType::Add:	return { .value = lhs.value + rhs.value };
			case TokenType::Sub:	return { .value = lhs.value - rhs.value };
			case TokenType::Mult:	return { .value = lhs.value * rhs.value };
			case TokenType::Div:	return { .value = lhs.value / rhs.value };
			case TokenType::Power:	return { .value = std::pow(lhs.value, rhs.value) };
		}

		return error;
	}

	std::string TokenNode::to_string(uint64_t indent) const
	{
		std::string result;
		result.append(indent, ' ');
		result += m_token.to_string() + '\n';
		for (TokenNode* node : m_nodes)
			result += node->to_string(indent + 2);
		return result;
	}

}