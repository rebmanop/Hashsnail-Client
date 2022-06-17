#include <map>
#include <string>
#include "alphabets.h"

namespace Alphabet {

	const std::string& GetAlphabet(const std::string& token)
	{
		if (token == "F")
			return alphabetLUD;
		else if (token == "D")
			return alphabetD;
		else if (token == "L")
			return alphabetL;
		else if (token == "U")
			return alphabetU;
		else
			return alphabetLUD;
	}
}