#include "algorithms.h"

std::string AlgorithmHandler::HashPermutation(const std::string& str) const
{
	return str;
}


std::string MD5Handler::HashPermutation(const std::string& str) const
{
	return md5(str);
}


std::string SHA1Handler::HashPermutation(const std::string& str) const
{ 
	return sha1(str);
}
