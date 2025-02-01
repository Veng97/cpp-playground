#pragma once

#include <concepts>
#include <memory>
#include <sstream>
#include <string>
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
  /**
   * @brief Constructs a KeyValuePair with a specified name.
   * @param name The name of the key.
   */
  explicit KeyValuePair(std::string name) : m_name(std::move(name))
  {
  }

  /**
   * @brief Virtual destructor for polymorphic behavior.
   */
  virtual ~KeyValuePair() = default;

  /**
   * @brief Gets the value of the key-value pair as a string.
   * @return The value as a string.
   */
  virtual std::string getValue() const = 0;

  /**
   * @brief Gets the name of the key-value pair.
   * @return The name of the key.
   */
  std::string getName() const
  {
    return m_name;
  }

private:
  const std::string m_name; ///< Name of the key.
};

/**
 * @brief Concept to check for the presence of a `jsonize` method in a type.
 */
template <typename T>
concept HasJsonize = requires(T obj) {
  { obj.jsonize() } -> std::same_as<std::vector<std::shared_ptr<KeyValuePair>>>; // Checks if jsonize returns a vector
} || requires(T obj) {
  { obj.jsonize() } -> std::same_as<std::shared_ptr<KeyValuePair>>; // Checks if jsonize returns a shared_ptr
};

/**
 * @brief Converts a vector of key-value pairs to a JSON-formatted string.
 * @param key_value_pairs A vector of shared pointers to KeyValuePair objects.
 * @return A JSON-formatted string representation of the key-value pairs.
 */
inline std::string valuesToJson(const std::vector<std::shared_ptr<Plotter::KeyValuePair>>& key_value_pairs)
{
  std::ostringstream oss;
  oss << "{";
  bool first = true;
  for (const auto& key_value_pair : key_value_pairs) {
    if (!first) {
      oss << ",";
    }
    first = false;
    oss << "\"" << key_value_pair->getName() << "\":" << key_value_pair->getValue();
  }

  oss << "}";
  return oss.str();
}

/**
 * @brief Converts a vector of key-value pairs to a JSON-formatted string with a timestamp.
 * @param key_value_pairs A vector of shared pointers to KeyValuePair objects.
 * @param timestamp The timestamp to include in the JSON string.
 * @return A JSON-formatted string representation of the key-value pairs with the timestamp.
 */
inline std::string valuesToJson(const std::vector<std::shared_ptr<Plotter::KeyValuePair>>& key_value_pairs, const double& timestamp)
{
  std::ostringstream oss;
  oss << R"({"timestamp":)" << timestamp << ",";
  bool first = true;
  for (const auto& key_value_pair : key_value_pairs) {
    if (!first) {
      oss << ",";
    }
    first = false;
    oss << "\"" << key_value_pair->getName() << "\":" << key_value_pair->getValue();
  }

  oss << "}";
  return oss.str();
}

/**
 * @brief Converts an object with a `jsonize` method to a JSON-formatted string.
 * @tparam T A type that has a `jsonize` method returning a vector of shared pointers to KeyValuePair objects.
 * @param obj The object to convert.
 * @return A JSON-formatted string representation of the object.
 */
template <typename T>
  requires HasJsonize<T>
inline std::string structToJson(const T& obj)
{
  const auto& members = obj.jsonize();
  if constexpr (std::is_same_v<std::vector<std::shared_ptr<KeyValuePair>>, decltype(members)>) {
    return valuesToJson(obj.jsonize());
  } else {
    return valuesToJson(std::vector<std::shared_ptr<KeyValuePair>>{obj.jsonize()});
  }
}

/**
 * @brief Converts an object with a `jsonize` method to a JSON-formatted string with a timestamp.
 * @tparam T A type that has a `jsonize` method returning a vector of shared pointers to KeyValuePair objects.
 * @param obj The object to convert.
 * @param timestamp The timestamp to include in the JSON string.
 * @return A JSON-formatted string representation of the object with the timestamp.
 */
template <typename T>
  requires HasJsonize<T>
inline std::string structToJson(const T& obj, const double& timestamp)
{
  const auto& members = obj.jsonize();
  if constexpr (std::is_same_v<std::vector<std::shared_ptr<KeyValuePair>>, decltype(members)>) {
    return valuesToJson(obj.jsonize(), timestamp);
  } else {
    return valuesToJson(std::vector<std::shared_ptr<KeyValuePair>>{obj.jsonize()}, timestamp);
  }
}

} // namespace Plotter