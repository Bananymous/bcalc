#pragma once

#include "Token.h"

#include <string_view>
#include <vector>

namespace bcalc::Lexer
{

	std::vector<Token> Tokenize(std::string_view);

}