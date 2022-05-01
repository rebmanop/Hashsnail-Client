#ifndef _ATTACK_MODES_H
#define _ATTACK_MODES_H

#include<string>
#include<set>
#include<vector>
#include <iostream>
#include "algorithms.h"



void BruteForceRec(const std::string& alphabet, size_t maxPasswordLength, const std::string& currentPermutation, const std::set<std::string>& hashSet,
                    std::vector<std::tuple<std::string, std::string>>& crackedPasswords, const std::string endPermutation, bool& reachedEndPermutation, AlgorithmHandler& currentAlgorithm);


std::vector<std::tuple<std::string, std::string>> BruteForce(const std::string& alphabet, size_t maxLength,
                                                                const std::set<std::string>& hashSet, AlgorithmHandler& currentAlgorithm, std::string startPermutation = "", std::string endPermutation = "");


size_t CalculatePermutationNumber(const std::string& permutation, size_t period[], const std::vector<std::string>& alphabets);


std::vector<std::tuple<std::string, std::string>> MaskBasedBruteForce(const std::set<std::string>& hashSet, const std::vector<std::string>& alphabets, AlgorithmHandler& currentAlgorithm,
                                                                        const std::string& startPermutation = "", const std::string& endPermutation = "");

#endif