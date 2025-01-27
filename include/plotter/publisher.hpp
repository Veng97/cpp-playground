#pragma once

#include "plotter/types.hpp"

#include <arpa/inet.h>
#include <memory>
#include <string>
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
  Publisher(std::string ip, int port);

  /**
   * @brief Destroys the Publisher object, closing the socket if open.
   */
  ~Publisher();

  /**
   * @brief Publishes data to the server, automatically handling the conversion to JSON format.
   *
   * @tparam T The type of the data to be published. It must either be:
   *           - `std::vector<std::shared_ptr<Plotter::KeyValuePair>>` or
   *           - A type that provides a `jsonize` method.
   * @param data The data to publish.
   * @throw std::runtime_error If the sending process fails.
   */
  template <typename T> void publish(const T& data)
  {
    if constexpr (std::is_same_v<T, std::vector<std::shared_ptr<Plotter::KeyValuePair>>>)
    {
      send(valuesToJson(data));
    }
    else
    {
      send(structToJson(data));
    }
  }

  /**
   * @brief Publishes data to the server with a timestamp. See `publish()` for more details.
   *
   * @tparam T The type of the data to be published.
   * @param data The data to publish.
   * @param timestamp The timestamp to include in the JSON string.
   * @throw std::runtime_error If the sending process fails.
   */
  template <typename T> void publishWithTimestamp(const T& data, const double& timestamp)
  {
    if constexpr (std::is_same_v<T, std::vector<std::shared_ptr<Plotter::KeyValuePair>>>)
    {
      send(valuesToJson(data, timestamp));
    }
    else
    {
      send(structToJson(data, timestamp));
    }
  }

  /**
   * @brief Sends the specified data to the server.
   * @param data The data to send.
   * @throws std::runtime_error If the sending process fails.
   */
  void send(const std::string& data);

private:
  std::string m_server_ip;          ///< The server's IP address.
  int m_server_port;                ///< The server's port number.
  int m_sockfd;                     ///< The socket file descriptor.
  struct sockaddr_in m_server_addr; ///< The server's address structure.
};

} // namespace Plotter