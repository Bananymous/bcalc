#pragma once

#include "Token.h"

#include <vector>

namespace bcalc
{
	using value_type = long double;

	class TokenNode;

	struct CalcResult
	{
		bool has_error = false;
		bool has_value = true; // only used in return value of 'Program::Process()'
		value_type value = value_type(0);
	};

	struct UserFunction
	{
		std::vector<std::string> parameters;
		TokenNode* expression;
	};

	using VariableList = std::unordered_map<std::string, value_type>;
	using FunctionList = std::unordered_map<std::string, UserFunction>;

	class TokenNode
	{
	public:
		TokenNode(Token token, std::vector<TokenNode*> nodes = {});
		~TokenNode()
		{
			for (TokenNode* node : m_nodes)
				delete node;
		}

		CalcResult approximate(const VariableList& variables, const FunctionList& functions) const;

		std::string to_string(uint64_t indent = 0) const;

	private:
		std::vector<TokenNode*>	m_nodes;
		Token					m_token;
	};

}
