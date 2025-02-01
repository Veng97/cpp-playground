#pragma once

#include "plotter/jsonize.hpp"

#include <ios>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace Plotter::Types
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
    for (const auto& value : m_values) {
      if (!first) {
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
    for (const auto& value : m_values) {
      if (!first) {
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

} // namespace Plotter::Types
