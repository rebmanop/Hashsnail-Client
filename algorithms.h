#ifndef _ALGORITHMS_H
#define _ALGORITHMS_H

#include <string>
#include "md5.h"
#include "sha1.h"


class AlgorithmHandler
{
public:
	virtual std::string HashPermutation(const std::string&) const = 0;
};


class MD5Handler : public AlgorithmHandler
{
public:
	MD5Handler() = default;
	std::string HashPermutation(const std::string&) const override;
};


class SHA1Handler : public AlgorithmHandler
{
public:
	SHA1Handler() = default;
	std::string HashPermutation(const std::string&) const override;
};

#endif
