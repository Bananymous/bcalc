#pragma once

#include "Token.h"

#include <vector>

namespace bcalc
{

	class TokenNode
	{
	public:
		TokenNode(Token token, std::vector<TokenNode*> nodes = {});
		~TokenNode()
		{
			for (TokenNode* node : m_nodes)
				delete node;
		}

		bool approximate(const std::unordered_map<std::string, long double>& variables, long double& out_result) const;

		std::string to_string(uint64_t indent = 0) const;

	private:
		std::vector<TokenNode*>	m_nodes;
		Token					m_token;
	};

}
