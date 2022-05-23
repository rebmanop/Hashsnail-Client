#include <ranges>
#include <sstream>
#include <fstream>
#include <thread>
#include "timer.h"
#include "range.h"
#include "alphabets.h"
#include "benchmark.h"
#include "TCPClient.h"
#include "algorithms.h"
#include "attack_modes.h"
#include "spdlog/spdlog.h"
#include "message_parser.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#define TESTING_LOCALLY 

#define DEFAULT_LOG_PATTERN "%^%v%$"
#define WHILE_ATTACKING_LOG_PATTERN "%^[%T]%$ %v"


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
            
            spdlog::trace("Starting benchmark. ({}s)", benchmark.m_BenchTimeSeconds);
            benchmark.RunMultiThread();

            spdlog::trace("Sending back benchmark results."); 
            client.Post(MessageParser::AssembleBenchmarkResultString(benchmark));

            spdlog::info("Waiting for a job from the server...");
        }

        else if (message[0] == MessageType::StartMaskBasedBruteForce)
        {
            Timer timer;
            spdlog::trace("Received mask based brute force request...");

            auto[alphabets, range, algorithm] = MessageParser::ParseMaskBasedAttackMassage(message);

            MaskBasedBruteForceAttack am(std::move(hashSet), std::move(alphabets), range, algorithm);

            spdlog::info("Started mask based brute force attack...");
            timer.Start();
            StartAttack(am);
            timer.End();
            spdlog::trace("Finished in {:03.2f}s", timer.GetTime("s"));
            std::vector<hash_password_pair> crackedPasswords = am.GetCrackedPasswords();
            spdlog::info("Sending back results...");
            client.Post(MessageParser::AssembleCrackedPasswordsMessage(crackedPasswords, timer));
        }

        else if (message[0] == MessageType::HashSet)
        {
            spdlog::trace("Receiving hash list from the server...");
            MessageParser::ParseHashSetMessage(message, hashSet);
        }

        else if (message[0] == MessageType::StopClient)
            client.Stop();

    };

    return OnMessage;
}


int main(int argc, char* argv[])
{
    //3 %L%L%L%L%L%L 0 aaaaaa zzzzzz
    spdlog::set_level(spdlog::level::level_enum::trace);
    spdlog::set_pattern(DEFAULT_LOG_PATTERN);
    #ifdef TESTING_LOCALLY 
        
        TCPClient client("127.0.0.1", 21);
    #else
        TCPClient client("26.197.251.191", 8900);

    #endif

    std::set<std::string> hashSet;
    std::vector<std::string> dictionary;

    #ifdef TESTING_LOCALLY
    {
        std::string line;
        std::ifstream infile("example0.hash");
        while (std::getline(infile, line))
            hashSet.insert(line);
        infile.close();
    }
    #endif

    client.OnMessage = GetMessageHandler(client, hashSet, dictionary);
    client.Run();

    return 0;
}



