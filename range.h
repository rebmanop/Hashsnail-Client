#ifndef _RANGE_H
#define _RANGE_H

#include <string>

class Range 
{
public:

	Range() = default;
	Range(const std::string endPermutation, const std::string startPermutation);
	
	const std::string& GetStartPermutation() const;
	const std::string& GetEndPermutation() const;
	bool StartNotSet() const;
	bool EndNotSet() const;

	~Range() = default;
	
private:
	std::string m_StartPermutation = "";
	std::string m_EndPermutation = "";
};

#endif

