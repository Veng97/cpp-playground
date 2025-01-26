#pragma once

#include "plotter/types.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace Plotter
{

class Publisher
{
public:
	Publisher(std::string ip, int port) : m_server_ip(std::move(ip)), m_server_port(port)
	{
		// Create a UDP socket
		m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (m_sockfd < 0)
		{
			throw std::runtime_error("[Plotter] Failed to create socket");
		}

		// Setup server address structure
		memset(&m_server_addr, 0, sizeof(m_server_addr));
		m_server_addr.sin_family = AF_INET;
		m_server_addr.sin_port = htons(m_server_port);
		if (inet_pton(AF_INET, m_server_ip.c_str(), &m_server_addr.sin_addr) <= 0)
		{
			throw std::runtime_error("[Plotter] Invalid IP address");
		}
	}

	~Publisher()
	{
		if (m_sockfd >= 0)
		{
			close(m_sockfd);
		}
	}

	static std::string valuesToJson(const std::vector<std::shared_ptr<Plotter::KeyValuePair>>& values)
	{
		std::ostringstream oss;
		oss << "{";
		bool first = true;
		for (const auto& value : values)
		{
			if (!first)
			{
				oss << ", ";
			}
			first = false;
			oss << "\"" << value->getName() << "\": " << value->getValue();
		}

		oss << "}";
		return oss.str();
	}

	template <typename T>
		requires HasJsonize<T>
	static std::string structToJson(const T& obj)
	{
		const auto& members = obj.jsonize();
		return valuesToJson(obj.jsonize());
	}

	void publishValues(const std::vector<std::shared_ptr<Plotter::KeyValuePair>>& values)
	{
		send(valuesToJson(values));
	}

	template <typename T> void publishStruct(const T& obj)
	{
		send(structToJson(obj));
	}

private:
	void send(const std::string& data)
	{
		int sent_bytes = sendto(m_sockfd, data.c_str(), data.size(), 0, reinterpret_cast<struct sockaddr*>(&m_server_addr), sizeof(m_server_addr));
		if (sent_bytes < 0)
		{
			throw std::runtime_error("[Plotter] Failed to publish data");
		}
	}

	std::string m_server_ip;					///< The server's IP address.
	int m_server_port;								///< The server's port number.
	int m_sockfd;											///< The socket file descriptor.
	struct sockaddr_in m_server_addr; ///< The server's address structure.
};

} // namespace Plotter