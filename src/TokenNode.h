#pragma once

#include "Token.h"

#include <vector>

namespace bcalc
{

	class TokenNode
	{
	public:
		using value_type = long double;

		struct ApproximateResult
		{
			bool has_error = false;
			value_type value = value_type(0);
		};

	public:
		TokenNode(Token token, std::vector<TokenNode*> nodes = {});
		~TokenNode()
		{
			for (TokenNode* node : m_nodes)
				delete node;
		}

		ApproximateResult approximate(const std::unordered_map<std::string, value_type>& variables) const;

		std::string to_string(uint64_t indent = 0) const;

	private:
		std::vector<TokenNode*>	m_nodes;
		Token					m_token;
	};

}
