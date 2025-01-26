#pragma once

#include "plotter/types.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

namespace Plotter
{

class Publisher
{
public:
  /**
   * @brief Constructs a Publisher object with the specified server IP and port.
   * @param ip The server's IP address.
   * @param port The server's port number.
   * @throws std::runtime_error If the socket creation or IP setup fails.
   */
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

  /**
   * @brief Destroys the Publisher object, closing the socket if open.
   */
  ~Publisher()
  {
    if (m_sockfd >= 0)
    {
      close(m_sockfd);
    }
  }

  /**
   * @brief Publishes data to the server, automatically handling the conversion to JSON format.
   *
   * This function supports publishing data in the form of a vector of `KeyValuePair` objects
   * or any type that provides a `jsonize` method. The data is converted to JSON format
   * internally before being sent.
   *
   * @tparam T The type of the data to be published. It must either be:
   *           - `std::vector<std::shared_ptr<Plotter::KeyValuePair>>` or
   *           - A type that provides a `jsonize` method.
   *
   * @param data The data to publish. If `data` is a vector of `KeyValuePair` objects, it is
   *             converted using `valuesToJson()`. If `data` provides a `jsonize` method, it is
   *             converted using `structToJson()`.
   *
   * @throw std::runtime_error If the sending process fails.
   *
   */
  template <typename T> void publish(const T& data)
  {
    if constexpr (std::is_same_v<T, std::vector<std::shared_ptr<Plotter::KeyValuePair>>>)
    {
      // If the type is a vector of KeyValuePair, convert to JSON and send.
      send(valuesToJson(data));
    }
    else
    {
      // If the type has a jsonize method, use it to convert to JSON and send.
      send(structToJson(data));
    }
  }

  /**
   * @brief Publishes data to the server with a timestamp. See `publish()` for more details.
   *
   * @tparam T The type of the data to be published.
   * @param data The data to publish.
   * @param timestamp The timestamp to include in the JSON string.
   *
   * @throw std::runtime_error If the sending process fails.
   */
  template <typename T> void publishWithTimestamp(const T& data, const double& timestamp)
  {
    if constexpr (std::is_same_v<T, std::vector<std::shared_ptr<Plotter::KeyValuePair>>>)
    {
      // If the type is a vector of KeyValuePair, convert to JSON and send.
      send(valuesToJson(data, timestamp));
    }
    else
    {
      // If the type has a jsonize method, use it to convert to JSON and send.
      send(structToJson(data, timestamp));
    }
  }

  /**
   * @brief Sends the specified data to the server.
   * @param data The data to send.
   * @throws std::runtime_error If the sending process fails.
   */
  void send(const std::string& data)
  {
    int sent_bytes = sendto(m_sockfd, data.c_str(), data.size(), 0, reinterpret_cast<struct sockaddr*>(&m_server_addr), sizeof(m_server_addr));
    if (sent_bytes < 0)
    {
      throw std::runtime_error("[Plotter] Failed to publish data");
    }
  }

private:
  std::string m_server_ip;          ///< The server's IP address.
  int m_server_port;                ///< The server's port number.
  int m_sockfd;                     ///< The socket file descriptor.
  struct sockaddr_in m_server_addr; ///< The server's address structure.
};

} // namespace Plotter