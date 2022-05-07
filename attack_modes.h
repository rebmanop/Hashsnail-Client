#ifndef _ATTACK_MODES_H
#define _ATTACK_MODES_H

#include<string>
#include<set>
#include<vector>
#include <iostream>
#include "range.h"
#include "algorithms.h"


typedef std::tuple<std::string, std::string> hash_password_pair;


struct AmSpecificParams
{
    std::string alphabet;
    size_t maxLength;
    std::vector<std::string> alphabets;
    std::vector<std::string> dictionary;
};


class AttackMode
{
public:
    virtual void Start(const std::set<std::string>& hashSet, const AlgorithmHandler& currentAlgorithm, const Range& range, const AmSpecificParams& params) = 0;
    virtual std::vector<Range> SubdivideRange(const Range& initialRange, const AmSpecificParams& params, int numberOfDivisions)  = 0;

protected:
    std::vector<hash_password_pair> m_CrackedPasswords;
};


class MaskBasedBruteForceAttack : public AttackMode
{
public:
    void Start(const std::set<std::string>& hashSet, const AlgorithmHandler& currentAlgorithm, const Range& range, const AmSpecificParams& params) override;
    std::vector<Range> SubdivideRange(const Range& initialRange, const AmSpecificParams& params, int numberOfDivisions)  override;

private:
    size_t CalculatePermutationNumber(const std::string& permutation, size_t period[], const std::vector<std::string>& alphabets);
};


class BruteForceAttack : public AttackMode
{
public:
    void Start(const std::set<std::string>& hashSet, const AlgorithmHandler& currentAlgorithm, const Range& range, const AmSpecificParams& params) override;
    std::vector<Range> SubdivideRange(const Range& initialRange, const AmSpecificParams& params, int numberOfDivisions)  override;

};

class DictionaryAttack : public AttackMode
{
public:
    void Start(const std::set<std::string>& hashSet, const AlgorithmHandler& currentAlgorithm, const Range& range, const AmSpecificParams& params) override;
    std::vector<Range> SubdivideRange(const Range& initialRange, const AmSpecificParams& params, int numberOfDivisions)  override;

};


#if 0

void BruteForceRec(const std::string& alphabet, size_t maxPasswordLength, const std::string& currentPermutation, const std::set<std::string>& hashSet,
                    std::vector<hash_password_pair>& crackedPasswords, const std::string endPermutation, bool& reachedEndPermutation, const AlgorithmHandler& currentAlgorithm);

std::vector<hash_password_pair> BruteForce(const std::string& alphabet, size_t maxLength,
                                                                const std::set<std::string>& hashSet, AlgorithmHandler& currentAlgorithm, const Range& range = Range());

size_t CalculatePermutationNumber(const std::string& permutation, size_t period[], const std::vector<std::string>& alphabets);

std::vector<hash_password_pair> MaskBasedBruteForce(const std::set<std::string>& hashSet, const std::vector<std::string>& alphabets, const AlgorithmHandler& currentAlgorithm,
                                                       const Range& range);

std::pair<int, int> FindIndexRange(const std::vector<std::string> dictionary, const Range& range);


std::vector<hash_password_pair> DictionaryAttack(const std::set<std::string>& hashSet, const std::vector<std::string>& dictionary, AlgorithmHandler& currentAlgorithm, 
                                                    const Range& range = Range());

#endif

#endif