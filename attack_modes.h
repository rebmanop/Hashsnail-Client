#ifndef _ATTACK_MODES_H
#define _ATTACK_MODES_H


#include<string>
#include<set>
#include<vector>
#include <iostream>
#include "range.h"
#include "algorithms.h"


typedef std::tuple<std::string, std::string> hash_password_pair;


void BruteForceRec(const std::string& alphabet, size_t maxPasswordLength, const std::string& currentPermutation, const std::set<std::string>& hashSet,
                    std::vector<hash_password_pair>& crackedPasswords, const std::string endPermutation, bool& reachedEndPermutation, AlgorithmHandler& currentAlgorithm);

std::vector<hash_password_pair> BruteForce(const std::string& alphabet, size_t maxLength,
                                                                const std::set<std::string>& hashSet, AlgorithmHandler& currentAlgorithm, const Range& range = Range());

size_t CalculatePermutationNumber(const std::string& permutation, size_t period[], const std::vector<std::string>& alphabets);

std::vector<hash_password_pair> MaskBasedBruteForce(const std::set<std::string>& hashSet, const std::vector<std::string>& alphabets, AlgorithmHandler& currentAlgorithm,
                                                       const Range& range = Range());

std::pair<int, int> FindIndexRange(const std::vector<std::string> dictionary, const Range& range);


std::vector<hash_password_pair> DictionaryAttack(const std::set<std::string>& hashSet, const std::vector<std::string>& dictionary, AlgorithmHandler& currentAlgorithm, 
                                                    const Range& range = Range());

#endif