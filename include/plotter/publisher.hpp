#pragma once

#include "plotter/jsonize.hpp"

#include <algorithm>
#include <netinet/in.h> // For sockaddr_in
#include <optional>
#include <string>

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
   * @param timestamp The timestamp to include in the JSON string.
   * @throw std::runtime_error If the sending process fails.
   */
  template <typename T> void publish(const T& data, std::optional<double> timestamp = std::nullopt)
  {
    std::string data_as_json = toJson(data, timestamp, m_reserve);
    m_reserve = std::max(data_as_json.size(), m_reserve);
    send(data_as_json);
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
  unsigned long m_reserve{64};      ///< The reserved size for the JSON string.
};

} // namespace Plotter