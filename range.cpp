#include "range.h"

Range::Range(const std::string startPermutation, const std::string endPermutation)
{
	m_StartPermutation = startPermutation;
	m_EndPermutation = endPermutation;
}

const std::string& Range::GetStartPermutation() const
{
	return m_StartPermutation;
}

const std::string& Range::GetEndPermutation() const
{
	return m_EndPermutation;
}

bool Range::StartNotSet() const
{
	if (m_StartPermutation == "")
		return true;
	else
		return false;
}

bool Range::EndNotSet() const
{
	if (m_EndPermutation == "")
		return true;
	else
		return false;
}