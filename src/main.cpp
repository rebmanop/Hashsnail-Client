#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>
#include "utils/timer.h"
#include "range/range.h"
#include "spdlog/spdlog.h"
#include "alphabets/alphabets.h"
#include "utils/benchmark.h"
#include "tcp_client/tcp_client.h"
#include "algorithms/algorithms.h"
#include "core/attack_modes.h"
#include "message_parser/message_parser.h"
#include "spdlog/sinks/stdout_color_sinks.h"


constexpr auto DEFAULT_LOG_PATTERN = "%^%v%$";
constexpr auto WHILE_ATTACKING_LOG_PATTERN = "%^[%T]%$ %v";


void StartAttack(AttackMode& am, bool multithreaded = true)
{
    spdlog::set_pattern(WHILE_ATTACKING_LOG_PATTERN);
    unsigned int nThreads = 1;
    if (multithreaded) 
         nThreads = std::thread::hardware_concurrency();
       
    std::vector<Range> ranges = am.SubdivideRange(nThreads);

    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < nThreads; i++)
        threads.emplace_back(&AttackMode::StartThread, std::ref(am), ranges[i]);

    for (auto& thread : threads)
        thread.join();
    spdlog::set_pattern(DEFAULT_LOG_PATTERN);
}


MessageHandler GetMessageHandler(TCPClient& client, std::set<std::string>& hashSet, std::vector<std::string>& dictionary)
{
    MessageHandler OnMessage = [&](const std::string& message)
    {
        if (message[0] == MessageType::BenchmarkRequest)
        {
            spdlog::trace("Received benchmark request.");

            Benchmark benchmark = MessageParser::ParseBenchmarkRequestMessage(message);

            spdlog::trace("Starting multi thread benchmark. ({}s)", benchmark.GetBenchmarkRunTime());
            benchmark.RunMultiThread();
            spdlog::info("Multi thread benchmark result: {:03.2f}MH/s", benchmark.GetResults());

            spdlog::trace("Sending back benchmark results."); 
            client.Post(MessageParser::AssembleBenchmarkResultMessage(benchmark));

            spdlog::info("Waiting for a job from the server...");
        }

        else if (message[0] == MessageType::StartMaskBasedBruteForce)
        {
            Timer timer;
            spdlog::trace("Received mask based brute force request...");

            auto[alphabets, range, algorithm] = MessageParser::ParseMaskBasedAttackMassage(message);
            
            spdlog::warn("Hashed passwords to crack: {}", hashSet.size());
            MaskBasedBruteForceAttack am(std::move(hashSet), std::move(alphabets), range, algorithm);

            spdlog::info("Started mask based brute force attack...");
            timer.Start();
            StartAttack(am);
            timer.End();
            spdlog::trace("Finished in {:03.2f}s", timer.GetTime("s"));
            std::vector<hash_password_pair> crackedPasswords = am.GetCrackedPasswords();
            spdlog::warn("Cracked {} passwords", crackedPasswords.size());

            spdlog::info("Sending results to the server...");
            client.Post(MessageParser::AssembleCrackedPasswordsMessage(crackedPasswords, timer));
        }

        else if (message[0] == MessageType::StartDictionaryAttack)
        {
            Timer timer;
            spdlog::trace("Received dictionary attack request...");

            std::shared_ptr<AlgorithmHandler> algorithm = MessageParser::ParseDictionaryAttackStartMessage(message);
            spdlog::warn("Hashed passwords to crack: {}", hashSet.size());
            spdlog::warn("Dictionary size: {}", dictionary.size());

            DictionaryAttack am(std::move(hashSet), std::move(dictionary), Range(), algorithm);
            spdlog::info("Started dictionary attack...");
            timer.Start();
            StartAttack(am);
            timer.End();
            spdlog::trace("Finished in {:03.2f}s", timer.GetTime("s"));
            std::vector<hash_password_pair> crackedPasswords = am.GetCrackedPasswords();
            spdlog::warn("Cracked {} passwords", crackedPasswords.size());

            spdlog::info("Sending results to the server...");
            client.Post(MessageParser::AssembleCrackedPasswordsMessage(crackedPasswords, timer));
        }

        else if (message[0] == MessageType::HashSet)
        {
            spdlog::trace("Receiving hash list from the server...");
            MessageParser::ParseHashSetMessage(message, hashSet);
        }

        else if (message[0] == MessageType::Dictionary)
        {
            spdlog::trace("Receiving dictionary from the server...");
            MessageParser::ParseDictionaryMessage(message, dictionary);
        }

        else if (message[0] == MessageType::StopClient)
            client.Stop();

    };

    return OnMessage;
}


int main()
{
    spdlog::set_level(spdlog::level::level_enum::trace);
    spdlog::set_pattern(DEFAULT_LOG_PATTERN);
    
    std::set<std::string> hashSet;
    std::vector<std::string> dictionary;

    std::string addressAndPort;
    std::cout << "Enter ip address and port (x.x.x.x:port): ";
    std::cin >> addressAndPort;

    auto[ip, port, inputIsCorrect] = MessageParser::ParseIpFromKeyboard(addressAndPort);

    if (inputIsCorrect)
    {
        TCPClient client(ip, port);
        client.OnMessage = GetMessageHandler(client, hashSet, dictionary);
        client.Run();
    }
   
    std::cin.get();
    std::cin.get();

    return 0;
}
