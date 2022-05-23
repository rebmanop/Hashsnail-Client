#pragma once
#include <tuple>
#include <vector>
#include <string>
#include "range.h"
#include "algorithms.h"
#include <algorithm>
#include <spdlog/spdlog.h>
#include "alphabets.h"
#include <memory>
#include "benchmark.h"
#include <set>
#include "attack_modes.h"
#include "timer.h"

struct MessageType
{
    const static char BenchmarkRequest = '0';
    const static char BenchmarkResult = '1';
    const static char HashSet = '2';
    const static char StartMaskBasedBruteForce = '3';
    const static char StartDictionaryAttack = '4';
    const static char StartBruteForceAttack = '5';
    const static char CrackedPasswords = '6';
    const static char StopClient = 's';
};

namespace MessageParser
{
	typedef std::tuple<std::vector<std::string>, Range, std::shared_ptr<AlgorithmHandler>> mask_based_attack_params;
	
    mask_based_attack_params ParseMaskBasedAttackMassage(std::string message);
	Benchmark ParseBenchmarkRequestMessage(const std::string&  message);
	void ParseHashSetMessage(const std::string& message, std::set<std::string>& hashSet);
    
	std::string AssembleCrackedPasswordsMessage(const std::vector<hash_password_pair>& crackedPasswords, Timer timer);
    std::string AssembleBenchmarkResultString(Benchmark benchmark);
    
    std::vector<std::string> Split(std::string& str, char delimiter = ' ');
}


