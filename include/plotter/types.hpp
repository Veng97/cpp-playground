#pragma once

#include <concepts>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
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
  for (const auto& key_value_pair : key_value_pairs)
  {
    if (!first)
    {
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
  for (const auto& key_value_pair : key_value_pairs)
  {
    if (!first)
    {
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
  if constexpr (std::is_same_v<std::vector<std::shared_ptr<KeyValuePair>>, decltype(members)>)
  {
    return valuesToJson(obj.jsonize());
  }
  else
  {
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
  if constexpr (std::is_same_v<std::vector<std::shared_ptr<KeyValuePair>>, decltype(members)>)
  {
    return valuesToJson(obj.jsonize(), timestamp);
  }
  else
  {
    return valuesToJson(std::vector<std::shared_ptr<KeyValuePair>>{obj.jsonize()}, timestamp);
  }
}

namespace Types
{
/**
 * @brief Represents a key-value pair with a boolean value.
 */
class Boolean : public KeyValuePair
{
public:
  /**
   * @brief Constructs a Bool key-value pair.
   * @param name The name of the key.
   * @param value The boolean value.
   */
  Boolean(std::string name, bool value) : KeyValuePair(std::move(name)), m_value(value)
  {
  }

  /**
   * @brief Gets the value of the key-value pair as a string.
   * @return "true" or "false" based on the boolean value.
   */
  std::string getValue() const override
  {
    return m_value ? "true" : "false";
  }

private:
  bool m_value; ///< Boolean value.
};

/**
 * @brief Represents a key-value pair with an integer value.
 */
class Integer : public KeyValuePair
{
public:
  /**
   * @brief Constructs an Integer key-value pair.
   * @param name The name of the key.
   * @param value The integer value.
   */
  Integer(std::string name, int value) : KeyValuePair(std::move(name)), m_value(value)
  {
  }

  /**
   * @brief Gets the value of the key-value pair as a string.
   * @return The integer value as a string.
   */
  std::string getValue() const override
  {
    return std::to_string(m_value);
  }

private:
  int m_value; ///< Integer value.
};

/**
 * @brief Represents a key-value pair with multiple integer values.
 */
class Integers : public KeyValuePair
{
public:
  /**
   * @brief Constructs an Integers key-value pair.
   * @param name The name of the key.
   * @param values A vector of integer values.
   */
  Integers(std::string name, std::vector<int> values) : KeyValuePair(std::move(name)), m_values(std::move(values))
  {
  }

  /**
   * @brief Gets the value of the key-value pair as a JSON string.
   * @return A JSON-formatted string of the integer values.
   */
  std::string getValue() const override
  {
    std::ostringstream oss;
    oss << "[";
    bool first = true;
    for (const auto& value : m_values)
    {
      if (!first)
      {
        oss << ",";
      }
      first = false;
      oss << value;
    }

    oss << "]";
    return oss.str();
  }

private:
  std::vector<int> m_values; ///< Integer values.
};

/**
 * @brief Represents a key-value pair with a float value.
 */
class Float : public KeyValuePair
{
public:
  /**
   * @brief Constructs a Float key-value pair.
   * @param name The name of the key.
   * @param value The float value.
   */
  Float(std::string name, double value) : KeyValuePair(std::move(name)), m_value(value)
  {
  }

  /**
   * @brief Gets the value of the key-value pair as a string.
   * @return The double value as a string.
   */
  std::string getValue() const override
  {
    std::ostringstream oss;
    oss.precision(std::numeric_limits<double>::max_digits10);
    oss << std::fixed << m_value;
    return oss.str();
  }

private:
  double m_value; ///< Float value.
};

/**
 * @brief Represents a key-value pair with multiple float values.
 */
class Floats : public KeyValuePair
{
public:
  /**
   * @brief Constructs a Floats key-value pair.
   * @param name The name of the key.
   * @param values A vector of float values.
   */
  Floats(std::string name, std::vector<double> values) : KeyValuePair(std::move(name)), m_values(std::move(values))
  {
  }

  /**
   * @brief Gets the value of the key-value pair as a string.
   * @return A JSON-formatted string of the float values.
   */
  std::string getValue() const override
  {
    std::ostringstream oss;
    oss.precision(std::numeric_limits<double>::max_digits10);
    oss << "[";
    bool first = true;
    for (const auto& value : m_values)
    {
      if (!first)
      {
        oss << ",";
      }
      first = false;
      oss << std::fixed << value;
    }

    oss << "]";
    return oss.str();
  }

private:
  std::vector<double> m_values; ///< Float values.
};

/**
 * @brief Represents a key-value pair with nested fields.
 */
class Dict : public KeyValuePair
{
public:
  /**
   * @brief Constructs a Dict key-value pair.
   * @param name The name of the key.
   * @param values A vector of nested key-value pairs.
   */
  Dict(std::string name, std::vector<std::shared_ptr<KeyValuePair>> values) : KeyValuePair(std::move(name)), m_values(std::move(values))
  {
  }

  /**
   * @brief Gets the value of the key-value pair as a JSON string.
   * @return A JSON-formatted string of the nested fields.
   */
  std::string getValue() const override
  {
    return valuesToJson(m_values);
  }

private:
  std::vector<std::shared_ptr<KeyValuePair>> m_values; ///< Nested key-value pairs.
};

} // namespace Types
} // namespace Plotter
