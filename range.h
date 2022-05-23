#pragma once
#include <string>

class Range 
{
public:

	Range() = default;
	Range(const std::string endPermutation, const std::string startPermutation);
	Range(long long endPermutation, long long startPermutation);

	
	const std::string& GetStartPermutation() const;
	const std::string& GetEndPermutation() const;
	
	
	long long GetStartPermutationNumber() const;
	long long GetEndPermutationNumber() const;

	bool StartNotSet() const;
	bool EndNotSet() const;

	~Range() = default;
	
private:
	std::string m_StartPermutation = "";
	std::string m_EndPermutation = "";
	long long m_StartPermutationNumber = -1;
	long long m_EndPermutationNumber = -1;
};


