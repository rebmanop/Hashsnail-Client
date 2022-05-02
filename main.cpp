#include <sstream>
#include <fstream>
#include "timer.h"
#include "range.h"
#include "alphabets.h"
#include "attack_modes.h"
#include "algorithms.h"

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
   Range range = Range("00001", "00005");


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

   timer.Start();
   crackedPasswords = DictionaryAttack(hashSet, dict, md5Algorithm);
   timer.End();
   std::cout << timer.GetTime("s")<< "s" << std::endl;
   
   std::cout << "done." << std::endl;
   return 0;
}
/*

std::vector<char> vBuffer(20 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
        [&](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::cout << "\n\nRead " << length << "bytes\n\n";

                for (int i = 0; i < length; i++)
                    std::cout << vBuffer[i];

                GrabSomeData(socket);
            }

        }
    );
}

int main()
{
    asio::error_code ec;

    asio::io_context context;

    //asio::io_context::work idleWork(context);

    //std::thread thrContext = std::thread([&]() { context.run(); });

    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 8000);

    asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if (!ec)
    {
        std::cout << "Connected!" << std::endl;
    }
    else
    {
        std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
    }

    if (socket.is_open())
    {
        while (true)
        {
            size_t bytes = socket.available();
               
            if (bytes > 0)
                break;
        }

        size_t bytes = socket.available();
        std::cout << "Bytes Available: " << bytes << std::endl;

        std::vector<char> vBuffer(bytes);
        socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);
        
        for (auto c : vBuffer)
            std::cout << c;

        std::cout << std::endl;
    }

    return 0;
}
    */



