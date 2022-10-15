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
		using value_type = TokenNode::value_type;

		struct ProcessResult
		{
			bool has_error = false;
			bool has_value = false;
			value_type value = value_type(0);
		};

	public:
		Program();
		~Program();

		ProcessResult Process(std::string_view input);

	private:
		std::unordered_map<std::string, value_type>		m_variables;
		std::unordered_map<std::string, UserFunction>	m_functions;
	};

}