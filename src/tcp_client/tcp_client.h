#pragma once
#include <queue>
#include <asio.hpp>

namespace io = asio;
typedef  std::function<void(const std::string&)> MessageHandler;

class TCPClient
{
public:
	TCPClient(const std::string& address, int port);

	void Run();
	void Stop();
	void Post(const std::string& message);

	static bool IsValidIp(const std::string& ip);
	static bool IsValidPort(const std::string& port);


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