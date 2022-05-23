#pragma once
#include <iostream>
#include <functional>
#include <queue>
#include "spdlog/spdlog.h"


#ifdef _WIN32 
#define _WIN32_WINNT 0x0A00
#endif


#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


namespace io = asio;
using MessageHandler = std::function<void(const std::string&)>;

class TCPClient
{
public:
	TCPClient(const std::string& address, int port);

	void Run();
	void Stop();
	void Post(const std::string& message);


private:
	void AsyncRead();
	void OnRead(std::error_code ec, size_t bytesTransferred);

	void AsyncWrite();
	void OnWrite(std::error_code ec, size_t bytesTransferred);



public:
	MessageHandler OnMessage;
	bool is_Connected = false;

private:
	io::io_context m_IoContext{};
	io::ip::tcp::socket m_Socket;

	io::ip::tcp::resolver::results_type m_Endpoints;

	io::streambuf m_StreamBuffer{65536};


	std::queue<std::string> m_OutgoingMessages{};
};