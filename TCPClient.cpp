#include "TCPClient.h"

TCPClient::TCPClient(const std::string& address, int port)
	:m_Socket(m_IoContext)
{
	io::ip::tcp::resolver resolver{ m_IoContext };
	m_Endpoints = resolver.resolve(address, std::to_string(port));
	
}

void TCPClient::Run()
{
	io::async_connect(m_Socket, m_Endpoints, [this](std::error_code ec, io::ip::tcp::endpoint ep) 
	{
			if (!ec)
			{
				AsyncRead();
				is_Connected = true;
				spdlog::info("[CONNECTION]: Successfully connected to {}:{}", ep.address().to_string(), ep.port());
				spdlog::info("Waiting for a job from the server...");
			}
			else 
			{
				spdlog::critical("[CONNECTION]: {} ({})", ec.message(), ec.value());
			}
	});

	m_IoContext.run();
}

void TCPClient::Stop()
{
	std::error_code ec;
	m_Socket.close(ec);
	m_IoContext.stop();
	is_Connected = false;

	if (ec)
	{
	}
}


void TCPClient::Post(const std::string& message)
{
	bool queueIdle = m_OutgoingMessages.empty();
	m_OutgoingMessages.push(message);

	if (queueIdle)
	{
		AsyncWrite(); 
	}
}

void TCPClient::AsyncRead()
{
	io::async_read_until(m_Socket, m_StreamBuffer, '\n', [this](std::error_code ec, size_t bytesTransferred)
	{
			OnRead(ec, bytesTransferred);
	});
}

void TCPClient::OnRead(std::error_code ec, size_t bytesTransferred)
{
	if (ec)
	{
		Stop();
		std::cout << "[ERROR]: " << ec << std::endl;
		return;
	}

	std::stringstream message;
	message << std::istream{ &m_StreamBuffer }.rdbuf();
	OnMessage(message.str());
	AsyncRead();
}

void TCPClient::AsyncWrite()
{
	io::async_write(m_Socket, io::buffer(m_OutgoingMessages.front()), [this](std::error_code ec, size_t bytesTransferred)
	{
			OnWrite(ec, bytesTransferred);
	});
}

void TCPClient::OnWrite(std::error_code ec, size_t bytesTransferred)
{
	if (ec)
	{
		Stop();
		return;
	}

	m_OutgoingMessages.pop();

	if (!m_OutgoingMessages.empty())
	{
		AsyncWrite();
	}

}
