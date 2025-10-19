#include "String.h"

namespace Core::String
{
	std::string Replace(std::string base, char remove, std::string replacement)
	{
		std::string result;
		for (char c : base) {
			if (c == remove)
				result += replacement;
			else
				result += c;
		}
		return result;
	}
}