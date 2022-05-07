#include <sstream>
#include <fstream>
#include <thread>
#include "timer.h"
#include "range.h"
#include "alphabets.h"
#include "attack_modes.h"
#include "algorithms.h"
#include "benchmark.h"


#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif


#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


void StartMultiThreadedAttack(AttackMode& am, const std::set<std::string>& hashSet, const AlgorithmHandler& a, const AmSpecificParams& op, const Range& initialRange)
{
    unsigned int nThreads = std::thread::hardware_concurrency();
    std::vector<Range> ranges = am.SubdivideRange(initialRange, op, nThreads);

    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < nThreads; i++)
    {
        threads.emplace_back(&AttackMode::Start, std::ref(am), std::ref(hashSet), std::ref(a), std::ref(ranges[i]), std::ref(op));
    }

    for (auto& thread : threads)
        thread.join();
}


int main()
{
   Timer timer;
   MD5Handler md5Algorithm;
   SHA1Handler sha1Algorithm;
   
   std::set<std::string> hashSet;
   std::vector<std::string> dict;

   std::string line;
   std::ifstream infile("example0.hash");
   while (std::getline(infile, line))
        hashSet.insert(line);
   infile.close();


   std::ifstream infile1("example.dict.txt");
   while (std::getline(infile1, line))
      dict.push_back(line);
   infile1.close();

   MaskBasedBruteForceAttack mbba;
   
   AmSpecificParams op;
   op.alphabets.push_back(alphabetL);
   op.alphabets.push_back(alphabetL);
   op.alphabets.push_back(alphabetL);
   op.alphabets.push_back(alphabetL);
   op.alphabets.push_back(alphabetL);
   op.alphabets.push_back(alphabetL);
   
   
   timer.Start();
   StartMultiThreadedAttack(mbba, hashSet, md5Algorithm, op, Range());

   timer.End();
   std::cout << timer.GetTime("s")<< "s" << std::endl;
   
   return 0;
}


//std::vector<char> vBuffer(20 * 1024);
//
//void GrabSomeData(asio::ip::tcp::socket& socket)
//{
//    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
//        [&](std::error_code ec, std::size_t length)
//        {
//            if (!ec)
//            {
//                std::cout << "Read " << length << " bytes\n";
//
//                for (int i = 0; i < length; i++)
//                    std::cout << vBuffer[i];
//
//                GrabSomeData(socket);
//            }
//
//        }
//    );
//}
//
//
//int main()
//{
//    asio::error_code ec;
//
//    asio::io_context context;
//
//    asio::io_context::work idleWork(context);
//
//    std::thread thrContext = std::thread([&]() { context.run(); });
//
//    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 8000);
//
//    asio::ip::tcp::socket socket(context);
//
//    socket.connect(endpoint, ec);
//
//    if (!ec)
//    {
//        std::cout << "Connected!" << std::endl;
//    }
//    else
//    {
//        std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
//    }
//
//    while (socket.is_open())
//    {
//
//      while (socket.available() == 0);
//      GrabSomeData(socket);
//    }
//    context.stop();
//    if (thrContext.joinable()) thrContext.join();
//    return 0;
//}


