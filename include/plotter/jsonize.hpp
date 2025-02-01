#pragma once

#include <concepts>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace Plotter
{

/**
 * @brief Represents a key-value pair for JSON conversion.
 */
class KeyValuePair
{
public:
  explicit KeyValuePair(std::string name);
  virtual ~KeyValuePair() = default;

  virtual std::string getValue() const = 0;

  std::string_view getKey() const;

private:
  const std::string m_key;
};

// Concept to check for `jsonize` method returning valid types
template <typename T>
concept HasJsonize = requires(T obj) {
  { obj.jsonize() } -> std::same_as<std::shared_ptr<KeyValuePair>>;
} || requires(T obj) {
  { obj.jsonize() } -> std::same_as<std::vector<std::shared_ptr<KeyValuePair>>>;
};

/**
 * @brief Converts a key-value pair to a JSON-formatted string.
 * @param data A vector of shared_ptr<KeyValuePair>.
 * @param timestamp Optional timestamp to include in the JSON string.
 * @return A JSON-formatted string representation.
 */
std::string toJson(const std::shared_ptr<KeyValuePair>& data, std::optional<double> timestamp = std::nullopt, std::optional<uint64_t> reserve = std::nullopt);

/**
 * @brief Converts key-value pairs to a JSON-formatted string.
 * @param data A vector of shared_ptr<KeyValuePair>.
 * @param timestamp Optional timestamp to include in the JSON string.
 * @return A JSON-formatted string representation.
 */
std::string toJson(const std::vector<std::shared_ptr<KeyValuePair>>& data, std::optional<double> timestamp = std::nullopt, std::optional<uint64_t> reserve = std::nullopt);

/**
 * @brief Converts a JSON-izable object to a JSON-formatted string.
 * @param data The JSON-izable object.
 * @param timestamp Optional timestamp to include in the JSON string.
 * @return A JSON-formatted string representation.
 */
template <HasJsonize T> std::string toJson(const T& data, std::optional<double> timestamp = std::nullopt, std::optional<uint64_t> reserve = std::nullopt)
{
  return toJson(data.jsonize(), timestamp, reserve);
}

} // namespace Plotter
