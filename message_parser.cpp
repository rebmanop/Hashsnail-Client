#include "message_parser.h"


namespace MessageParser {

    mask_based_attack_params MessageParser::ParseMaskBasedAttackMassage(std::string message)
    {
        std::vector<std::string> tokens = Split(message);

        spdlog::trace("[MESSAGE PARSER] Mask: {}", tokens[1]);
        std::vector<std::string> mask = Split(tokens[1], '%');

        std::vector<std::string> alphabets;
        for (auto& c : mask)
            alphabets.push_back(GetAlphabet(c));

        std::shared_ptr<AlgorithmHandler> algorithm = DetermineAlgorithm(tokens[2][0]);
        spdlog::trace("[MESSAGE PARSER] Agorithm: {}", algorithm->GetAlgorithmName());

        Range range(tokens[3], tokens[4]);
        spdlog::trace("[MESSAGE PARSER] Range: {}-{}", range.GetStartPermutation(), range.GetEndPermutation());

        return {alphabets, range, algorithm};
    }


    Benchmark ParseBenchmarkRequestMessage(const std::string& message)
    {
        int benchTime = 5;
        try { benchTime = std::stoi(message.substr(1)); }
        catch (std::invalid_argument) {}
        catch (std::out_of_range) {}

        Benchmark benchmark(benchTime);
        return benchmark;
    }

    void ParseHashSetMessage(const std::string& message, std::set<std::string>& hashSet)
    {
         std::stringstream ss(message.substr(2));
         std::string hash;
         while (ss >> hash)
             hashSet.insert(hash);
    }

    std::string AssembleCrackedPasswordsMessage(const std::vector<hash_password_pair>& crackedPasswords, Timer timer)
    {
        std::string messageToSend;
        messageToSend.push_back(MessageType::CrackedPasswords);
        messageToSend += " " + std::to_string(timer.GetTime("s"));

        for (auto& hp : crackedPasswords)
            messageToSend += " " + hp.second + " " + hp.first;

        messageToSend += '\n';

        return messageToSend;
    }

    std::string AssembleBenchmarkResultString(Benchmark benchmark)
    {
        std::string messageToSend;
        messageToSend.push_back(MessageType::BenchmarkResult);
        messageToSend += " " + std::to_string(benchmark.GetResults()) + '\n';

        return messageToSend;
    }


    std::vector<std::string> MessageParser::Split(std::string& str, char delimiter)
    {
        std::vector<std::string> tokens;
        if (delimiter != ' ')
            std::replace(str.begin(), str.end(), delimiter, ' ');

        std::stringstream ss(str);
        std::string token;
        while (ss >> token)
            tokens.push_back(token);

        return tokens;
    }

}



