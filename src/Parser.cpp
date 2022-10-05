#include "Parser.h"


#define BCALC_PRINT_ERROR(show, begin, end, ...) if (show) { fprintf(stderr, __VA_ARGS__); dump_tokens(begin, end); }

namespace bcalc
{

	using it = std::vector<Token>::const_iterator;

	static bool IsValid(it begin, it end)
	{
		int64_t depth = 0;
		for (auto it = begin; it != end; it++)
		{
			if (it->Type() == TokenType::LParan)
				depth++;
			else if (it->Type() == TokenType::RParan)
			{
				if (depth == 0)
					return false;
				depth--;
			}
		}
		return depth == 0;
	}

	static bool IsInParenthesis(it begin, it end)
	{
		if (begin->Type() != TokenType::LParan)
			return false;

		int64_t depth = 0;
		for (auto it = begin + 1; it != end - 1; it++)
		{
			if (it->Type() == TokenType::LParan)
				depth++;
			else if (it->Type() == TokenType::RParan)
			{
				if (depth == 0)
					return false;
				depth--;
			}
		}

		return depth == 0;
	}

	static it FindZeroDepth(it begin, it end, TokenType type)
	{
		uint64_t depth = 0;
		for (auto it = end - 1; it >= begin; it--)
		{
			if (it->Type() == type && depth == 0)
				return it;
			else if (it->Type() == TokenType::RParan)
				depth++;
			else if (it->Type() == TokenType::LParan)
				depth--;
		}
		return end;
	}

	static it LastOOO(it begin, it end)
	{
		// Addition / Substraction
		auto add	= FindZeroDepth(begin, end, TokenType::Add);
		auto sub	= FindZeroDepth(begin, end, TokenType::Sub);
		if (add != end && sub != end)
			return add > sub ? add : sub;
		else if (add != end)
			return add;
		else if (sub != end)
			return sub;

		// Multiplication / Division
		auto mult 	= FindZeroDepth(begin, end, TokenType::Mult);
		auto div 	= FindZeroDepth(begin, end, TokenType::Div);
		if (mult != end && div != end)
			return mult > div ? mult : div;
		else if (mult != end)
			return mult;
		else if (div != end)
			return div;

		auto power	= FindZeroDepth(begin, end, TokenType::Power);
		if (power != end)
			return power;

		return end;
	}

	static void dump_tokens(it begin, it end)
	{
		for (auto it = begin; it != end; it++)
			fprintf(stderr, "%s\n", it->to_string().c_str());
	}

	static TokenNode* BuildTokenTree(it begin, it end, bool errors)
	{
		if (!IsValid(begin, end))
		{
			BCALC_PRINT_ERROR(errors, begin, end, "Invalid parenthesis\n");
			return nullptr;
		}

		while (begin < end && IsInParenthesis(begin, end))
		{
			begin++;
			end--;
		}

		if (begin == end)
		{
			BCALC_PRINT_ERROR(errors, begin, end, "No tokens\n");
			return nullptr;
		}

		if (std::distance(begin, end) == 1)
		{
			if (begin->Type() == TokenType::Value || begin->Type() == TokenType::Constant)
				return new TokenNode(*begin);

			BCALC_PRINT_ERROR(errors, begin, end, "Invalid input\n");
			return nullptr;
		}

		if (begin->Type() == TokenType::Function && IsInParenthesis(begin + 1, end))
		{
			std::vector<TokenNode*> inputs;

			it comma = begin + 1;

			do
			{
				it start = ++comma;
				while (comma + 1 != end && comma->Type() != TokenType::Comma)
					comma++;

				TokenNode* input = BuildTokenTree(start, comma, errors);
				if (!input)
				{
					BCALC_PRINT_ERROR(errors, begin, end, "Could not build function input\n");
					return nullptr;
				}

				inputs.push_back(input);
			}
			while (comma + 1 != end);

			return new TokenNode(*begin, inputs);
		}

		auto op = LastOOO(begin, end);
		if (op == end)
		{
			BCALC_PRINT_ERROR(errors, begin, end, "No operators found\n\n");
			return nullptr;
		}

		TokenNode* lhs = nullptr;
		if (begin == op)
		{
			if (op->Type() == TokenType::Add || op->Type() == TokenType::Sub)
				lhs = new TokenNode(Token::CreateValue(0));
		}
		else
		{
			lhs = BuildTokenTree(begin, op, errors);
		}

		if (!lhs) 
		{
			BCALC_PRINT_ERROR(errors, begin, end, "Could not build left node\n");
			return nullptr;
		}

		TokenNode* rhs = BuildTokenTree(op + 1, end, errors);
		if (!rhs) 
		{
			BCALC_PRINT_ERROR(errors, begin, end, "Could not build right node\n");
			return nullptr;
		}
		
		return new TokenNode(*op, { lhs, rhs });
	}


	TokenNode* Parser::BuildTokenTree(const std::vector<Token>& tokens, bool errors)
	{
		return BuildTokenTree(tokens.begin(), tokens.end(), errors);
	}

}