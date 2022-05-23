#include "algorithms.h"


std::string MD5Handler::HashPermutation(const std::string& str) const
{
	return md5(str);
}

std::string MD5Handler::GetAlgorithmName() const
{
	return "MD5";
}

std::string SHA1Handler::HashPermutation(const std::string& str) const
{ 
	return sha1(str);
}

std::string SHA1Handler::GetAlgorithmName() const
{
	return "SHA1";
}

std::shared_ptr<AlgorithmHandler> DetermineAlgorithm(char token)
{
	std::shared_ptr<AlgorithmHandler> algorithm = nullptr;

	if (token == Algorithms::MD5)
		algorithm.reset(new MD5Handler);
	else if (token == Algorithms::SHA1)
		algorithm.reset(new SHA1Handler);
	else
		algorithm.reset(new MD5Handler);

	return algorithm;
}
