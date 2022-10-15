#pragma once

#include <TokenNode.h>

namespace bcalc
{

	struct UserFunction
	{
		std::vector<std::string> parameters;
		TokenNode* expression;
	};

	class Program
	{
	public:
		Program();
		~Program();

		CalcResult Process(std::string_view input);

	private:
		std::unordered_map<std::string, value_type>		m_variables;
		std::unordered_map<std::string, UserFunction>	m_functions;
	};

}