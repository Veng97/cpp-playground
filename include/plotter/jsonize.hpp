#pragma once

#include <concepts>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace Plotter
{

/**
 * @brief Represents a key-value pair for JSON conversion.
 */
class KeyValuePair
{
public:
  explicit KeyValuePair(std::string name) : m_key(std::move(name))
  {
  }
  virtual ~KeyValuePair() = default;

  virtual std::string getValue() const = 0;

  std::string_view getKey() const
  {
    return m_key;
  }

private:
  const std::string m_key;
};

// Concept to check for `jsonize` method returning valid types
template <typename T>
concept HasJsonize = requires(T obj) {
  { obj.jsonize() } -> std::same_as<std::vector<std::shared_ptr<KeyValuePair>>>;
} || requires(T obj) {
  { obj.jsonize() } -> std::same_as<std::shared_ptr<KeyValuePair>>;
};

/**
 * @brief Converts key-value pairs to a JSON-formatted string.
 * @param data A vector of shared_ptr<KeyValuePair>.
 * @param timestamp Optional timestamp to include in the JSON string.
 * @return A JSON-formatted string representation.
 */
inline std::string toJson(const std::vector<std::shared_ptr<KeyValuePair>>& data, std::optional<double> timestamp = std::nullopt, std::optional<uint64_t> reserve = std::nullopt)
{
  std::string str;
  if (reserve) {
    str.reserve(*reserve);
  } else {
    str.reserve(256);
  }

  str.append("{");

  if (timestamp) {
    str.append("\"timestamp\":").append(std::to_string(*timestamp)).append(",");
  }

  bool first = true;
  for (const auto& kvp : data) {
    if (!first) {
      str.append(",");
    }
    first = false;
    str.append("\"").append(kvp->getKey()).append("\":").append(kvp->getValue());
  }

  str.append("}");

  return str;
}

/**
 * @brief Converts a key-value pair to a JSON-formatted string.
 * @param data A vector of shared_ptr<KeyValuePair>.
 * @param timestamp Optional timestamp to include in the JSON string.
 * @return A JSON-formatted string representation.
 */
inline std::string toJson(const std::shared_ptr<KeyValuePair>& data, std::optional<double> timestamp = std::nullopt, std::optional<uint64_t> reserve = std::nullopt)
{
  std::string str;
  if (reserve) {
    str.reserve(*reserve);
  } else {
    str.reserve(256);
  }

  str.append("{");

  if (timestamp) {
    str.append("\"timestamp\":").append(std::to_string(*timestamp)).append(",");
  }

  str.append("\"").append(data->getKey()).append("\":").append(data->getValue());

  str.append("}");

  return str;
}

/**
 * @brief Converts a JSON-izable object to a JSON-formatted string.
 * @param data The JSON-izable object.
 * @param timestamp Optional timestamp to include in the JSON string.
 * @return A JSON-formatted string representation.
 */
template <HasJsonize T> inline std::string toJson(const T& data, std::optional<double> timestamp = std::nullopt, std::optional<uint64_t> reserve = std::nullopt)
{
  return toJson(data.jsonize(), timestamp, reserve);
}

} // namespace Plotter
