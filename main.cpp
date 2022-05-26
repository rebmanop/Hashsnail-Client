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

            spdlog::trace("Starting multi thread benchmark. ({}s)", benchmark.m_BenchTimeSeconds);
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


int main(int argc, char* argv[])
{
    //3 %L%L%L%L%L%L 0 aaaaaa zzzzzz

    //7 anMD5hash canterbury regismustdie reichenburg databases fingerprint EngIneER 


    spdlog::set_level(spdlog::level::level_enum::trace);
    spdlog::set_pattern(DEFAULT_LOG_PATTERN);
    
    std::set<std::string> hashSet;
    std::vector<std::string> dictionary;

    std::string addressAndPort;
    std::cout << "Hashsnail::Enter ip address and port (x.x.x.x:port): ";
    std::cin >> addressAndPort;


    std::vector <std::string> tokens;

    if (addressAndPort.contains(':'))
         tokens = MessageParser::Split(addressAndPort, ':');
    else
        spdlog::critical("Incorrect ip address.");

    if (TCPClient::IsValidIp(tokens[0]) && TCPClient::IsValidPort(tokens[1]))
    {
        std::string ip = tokens[0];
        int port = std::stoi(tokens[1]);
        TCPClient client(ip, port);
        client.OnMessage = GetMessageHandler(client, hashSet, dictionary);
        client.Run();
    }
    else
    {
        spdlog::critical("Incorrect ip address.");
        exit(-1);
    }

    return 0;
}



 