#include "plotter/publisher.hpp"

#include <cstring>
#include <stdexcept>

namespace Plotter
{

Publisher::Publisher(std::string ip, int port) : m_server_ip(std::move(ip)), m_server_port(port)
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

Publisher::~Publisher()
{
  if (m_sockfd >= 0)
  {
    close(m_sockfd);
  }
}

void Publisher::send(const std::string& data)
{
  int sent_bytes = sendto(m_sockfd, data.c_str(), data.size(), 0, reinterpret_cast<struct sockaddr*>(&m_server_addr), sizeof(m_server_addr));
  if (sent_bytes < 0)
  {
    throw std::runtime_error("[Plotter] Failed to publish data");
  }
}

} // namespace Plotter
