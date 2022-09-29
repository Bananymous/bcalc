#pragma once

#include "TokenNode.h"
#include "Token.h"

#include <vector>

namespace bcalc::Parser
{
	
	TokenNode* BuildTokenTree(const std::vector<Token>&);

}