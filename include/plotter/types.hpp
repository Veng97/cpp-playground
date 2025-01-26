#pragma once

#include <concepts>
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
  std::string m_name; ///< Name of the key.
};

/**
 * @brief Concept to check for the presence of a `jsonize` method in a type.
 */
template <typename T>
concept HasJsonize = requires(T obj) {
  { obj.jsonize() } -> std::same_as<std::vector<std::shared_ptr<KeyValuePair>>>;
};

/**
 * @brief Converts a vector of key-value pairs to a JSON-formatted string.
 * @param values A vector of shared pointers to KeyValuePair objects.
 * @return A JSON-formatted string representation of the key-value pairs.
 */
std::string valuesToJson(const std::vector<std::shared_ptr<Plotter::KeyValuePair>>& values)
{
  std::ostringstream oss;
  oss << "{";
  bool first = true;
  for (const auto& value : values)
  {
    if (!first)
    {
      oss << ",";
    }
    first = false;
    oss << "\"" << value->getName() << "\":" << value->getValue();
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
std::string structToJson(const T& obj)
{
  const auto& members = obj.jsonize();
  return valuesToJson(obj.jsonize());
}

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
  Float(std::string name, float value) : KeyValuePair(std::move(name)), m_value(value)
  {
  }

  /**
   * @brief Gets the value of the key-value pair as a string.
   * @return The float value as a string.
   */
  std::string getValue() const override
  {
    return std::to_string(m_value);
  }

private:
  float m_value; ///< Float value.
};

/**
 * @brief Represents a key-value pair with a string value.
 */
class String : public KeyValuePair
{
public:
  /**
   * @brief Constructs a String key-value pair.
   * @param name The name of the key.
   * @param value The string value.
   */
  String(std::string name, std::string value) : KeyValuePair(std::move(name)), m_value(std::move(value))
  {
  }

  /**
   * @brief Gets the value of the key-value pair as a string.
   * @return The string value enclosed in quotes.
   */
  std::string getValue() const override
  {
    return "\"" + m_value + "\"";
  }

private:
  std::string m_value; ///< String value.
};

/**
 * @brief Represents a key-value pair with a boolean value.
 */
class Bool : public KeyValuePair
{
public:
  /**
   * @brief Constructs a Bool key-value pair.
   * @param name The name of the key.
   * @param value The boolean value.
   */
  Bool(std::string name, bool value) : KeyValuePair(std::move(name)), m_value(value)
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
 * @brief Represents a key-value pair with nested fields.
 */
class NestedFields : public KeyValuePair
{
public:
  /**
   * @brief Constructs a NestedFields key-value pair.
   * @param name The name of the key.
   * @param values A vector of nested key-value pairs.
   */
  NestedFields(std::string name, std::vector<std::shared_ptr<KeyValuePair>> values) : KeyValuePair(std::move(name)), m_values(std::move(values))
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

} // namespace Plotter
