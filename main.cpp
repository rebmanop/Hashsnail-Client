#include <sstream>
#include <fstream>
#include "timer.h"
#include "range.h"
#include "alphabets.h"
#include "attack_modes.h"
#include "algorithms.h"
#include <thread>

#include "benchmark.h"


#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


int main()
{
   Timer timer;
   MD5Handler md5Algorithm;
   SHA1Handler sha1Algorithm;


   unsigned int n = std::thread::hardware_concurrency();
   std::cout << n << " concurrent threads are supported.\n";

   Range range0 = Range("aaaaaa", "dgnaaa");
   Range range1 = Range("dgnaaa", "gnaaaa");
   Range range2 = Range("gnaaaa", "jtnaaa");
   Range range3 = Range("jtnaaa", "naaaaa");
   Range range4 = Range("naaaaa", "qgmzzs");
   Range range5 = Range("qgmzzs", "tnaaaa");
   Range range6 = Range("tnaaaa", "wtnaai");
   Range range7 = Range("wtnaai", "zzzzzz");



   std::vector<std::string> alphabets;
   alphabets.push_back(alphabetL);
   alphabets.push_back(alphabetL);
   alphabets.push_back(alphabetL);
   alphabets.push_back(alphabetL);
   alphabets.push_back(alphabetL);
   alphabets.push_back(alphabetL);




   std::vector<hash_password_pair> crackedPasswords;
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

   //Benchmark benchmark;
   //
   //benchmark.RunBenchmark();
   //std::cout << benchmark.GetResults() << std::endl;

   timer.Start();
   std::thread thread0 = std::thread(MaskBasedBruteForce, std::ref(hashSet), std::ref(alphabets), std::ref(md5Algorithm), std::ref(range0));
   std::thread thread1 = std::thread(MaskBasedBruteForce, std::ref(hashSet), std::ref(alphabets), std::ref(md5Algorithm), std::ref(range1));
   std::thread thread2 = std::thread(MaskBasedBruteForce, std::ref(hashSet), std::ref(alphabets), std::ref(md5Algorithm), std::ref(range2));
   std::thread thread3 = std::thread(MaskBasedBruteForce, std::ref(hashSet), std::ref(alphabets), std::ref(md5Algorithm), std::ref(range3));
   std::thread thread4 = std::thread(MaskBasedBruteForce, std::ref(hashSet), std::ref(alphabets), std::ref(md5Algorithm), std::ref(range4));
   std::thread thread5 = std::thread(MaskBasedBruteForce, std::ref(hashSet), std::ref(alphabets), std::ref(md5Algorithm), std::ref(range5));
   std::thread thread6 = std::thread(MaskBasedBruteForce, std::ref(hashSet), std::ref(alphabets), std::ref(md5Algorithm), std::ref(range6));
   std::thread thread7 = std::thread(MaskBasedBruteForce, std::ref(hashSet), std::ref(alphabets), std::ref(md5Algorithm), std::ref(range7));
   
   thread0.join();
   thread1.join();
   thread2.join();
   thread3.join();
   thread4.join();
   thread5.join();
   thread6.join();
   thread7.join();


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
//    if (socket.is_open())
//    {
//        GrabSomeData(socket);
//        context.stop();
//        if (thrContext.joinable()) thrContext.join();
//    }
//
//    return 0;
//}



