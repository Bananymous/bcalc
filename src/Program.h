#pragma once

#include <TokenNode.h>

namespace bcalc
{

	class Program
	{
	public:
		Program();
		~Program();

		CalcResult Process(std::string_view input);

	private:
		VariableList m_variables;
		FunctionList m_functions;
	};

}