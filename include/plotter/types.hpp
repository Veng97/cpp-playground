#pragma once

#include "plotter/jsonize.hpp"

#include <charconv>
#include <memory>
#include <string>
#include <system_error>
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
  std::string getValue() override
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
  std::string getValue() override
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
  Float(std::string name, double value) : KeyValuePair(std::move(name)), m_value(value)
  {
  }

  /**
   * @brief Gets the value of the key-value pair as a string.
   * @return The double value as a string.
   */
  std::string getValue() override
  {
    // Buffer large enough to hold any double representation
    char buffer[32];
    auto [ptr, ec] = std::to_chars(buffer, buffer + sizeof(buffer), m_value, std::chars_format::general);
    if (ec != std::errc()) {
      return "NaN";
    }
    return std::string(buffer, ptr);
  }

private:
  double m_value; ///< Float value.
};

/**
 * @brief Represents a key-value pair with nested fields.
 */
class Dict : public KeyValuePair
{
public:
  /**
   * @brief Constructs a Dict with a single key-value pair.
   * @param name The key name.
   * @param value A single key-value pair.
   */
  Dict(std::string name, std::shared_ptr<KeyValuePair> value) : KeyValuePair(std::move(name)), m_value(std::move(value)), m_is_scalar(true)
  {
  }

  /**
   * @brief Constructs a Dict with multiple key-value pairs.
   * @param name The key name.
   * @param values A vector of key-value pairs.
   */
  Dict(std::string name, std::vector<std::shared_ptr<KeyValuePair>> values) : KeyValuePair(std::move(name)), m_values(std::move(values)), m_is_scalar(false)
  {
  }

  /**
   * @brief Gets the value of the key-value pair as a JSON string.
   * @return A JSON-formatted string of the nested fields.
   */
  std::string getValue() override
  {
    return m_is_scalar ? toJson(m_value) : toJson(m_values);
  }

private:
  const std::shared_ptr<KeyValuePair> m_value;               ///< Single key-value pair (optional)
  const std::vector<std::shared_ptr<KeyValuePair>> m_values; ///< Multiple key-value pairs (optional)
  const bool m_is_scalar;                                    ///< Flag to track which variant is used
};

} // namespace Plotter::Types
