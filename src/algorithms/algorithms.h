#pragma once

struct Algorithms
{
	const static char MD5 = '0';
	const static char SHA1 = '1';
};


class AlgorithmHandler
{
public:
	virtual std::string HashPermutation(const std::string&) const = 0;
	virtual std::string GetAlgorithmName() const  = 0;
};


class MD5Handler : public AlgorithmHandler
{
public:
	std::string HashPermutation(const std::string&) const override;
	std::string GetAlgorithmName() const override;
};


class SHA1Handler : public AlgorithmHandler
{
public:
	std::string HashPermutation(const std::string&) const override;
	std::string GetAlgorithmName() const override;

};


std::shared_ptr<AlgorithmHandler> GetCorrectAlgorithm(char token);

