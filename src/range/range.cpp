#include <string>
#include "range.h"


Range::Range(const std::string startPermutation, const std::string endPermutation)
{
	m_StartPermutation = startPermutation;
	m_EndPermutation = endPermutation;
}


Range::Range(long long startPermutation, long long endPermutation)
{
	m_StartPermutationNumber = startPermutation;
	m_EndPermutationNumber = endPermutation;

}

const std::string& Range::GetStartPermutation() const
{
	return m_StartPermutation;
}

const std::string& Range::GetEndPermutation() const
{
	return m_EndPermutation;
}

long long Range::GetStartPermutationNumber() const
{
	return m_StartPermutationNumber;
}

long long Range::GetEndPermutationNumber() const
{
	return m_EndPermutationNumber;
}

bool Range::StartNotSet() const
{
	if (m_StartPermutation == "" && m_StartPermutationNumber == -1)
		return true;
	else
		return false;
}

bool Range::EndNotSet() const
{
	if (m_EndPermutation == "" && m_EndPermutationNumber == -1)
		return true;
	else
		return false;
}