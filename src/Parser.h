#pragma once

#include "TokenNode.h"
#include "Token.h"

#include <vector>

namespace bcalc::Parser
{
	
	TokenNode* BuildTokenTree(std::vector<Token>::const_iterator begin, std::vector<Token>::const_iterator end, bool errors = false);

}