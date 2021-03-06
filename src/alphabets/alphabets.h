#pragma once

namespace Alphabet {

	const std::string alphabetL = "abcdefghijklmnopqrstuvwxyz";
	
	static const std::string alphabetU = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	static const std::string alphabetD = "0123456789";
	
	static const std::string alphabetLU = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	
	static const std::string alphabetLUD = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	
	static const std::string alphabetTest = "abc";
	
	const std::string& GetAlphabet(const std::string& token);
}




