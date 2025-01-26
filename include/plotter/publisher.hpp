#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class Publisher {
public:
  Publisher(std::string ip, int port)
      : m_server_ip(std::move(ip)), m_server_port(port) {
    // Create a UDP socket
    m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_sockfd < 0) {
      throw std::runtime_error("[Plotter] Failed to create socket");
    }

    // Setup server address structure
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(m_server_port);
    if (inet_pton(AF_INET, m_server_ip.c_str(), &m_server_addr.sin_addr) <= 0) {
      throw std::runtime_error("[Plotter] Invalid IP address");
    }
  }

  ~Publisher() {
    if (m_sockfd >= 0) {
      close(m_sockfd);
    }
  }

  void publish(const std::string &data) {
    int sent_bytes = sendto(m_sockfd, data.c_str(), data.size(), 0,
                            reinterpret_cast<struct sockaddr *>(&m_server_addr),
                            sizeof(m_server_addr));
    if (sent_bytes < 0) {
      throw std::runtime_error("[Plotter] Failed to publish data");
    }
  }

private:
  std::string m_server_ip;
  int m_server_port;
  int m_sockfd;
  sockaddr_in m_server_addr;
};
