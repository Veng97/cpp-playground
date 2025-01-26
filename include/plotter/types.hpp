#pragma once

#include <concepts>
#include <memory>
#include <string>
#include <vector>

namespace Plotter {

class KeyValuePair {
public:
  explicit KeyValuePair(std::string name) : m_name(std::move(name)) {}
  virtual ~KeyValuePair() = default;

  virtual std::string getValue() const = 0;
  std::string getName() const { return m_name; }

private:
  std::string m_name;
};

// Define a concept to check for the presence of jsonize
template <typename T>
concept HasJsonize = requires(T obj) {
  { obj.jsonize() } -> std::same_as<std::vector<std::shared_ptr<KeyValuePair>>>;
};

class Integer : public KeyValuePair {
public:
  Integer(std::string name, int value)
      : KeyValuePair(std::move(name)), m_value(value) {}

  std::string getValue() const override { return std::to_string(m_value); }

private:
  int m_value;
};

class Float : public KeyValuePair {
public:
  Float(std::string name, float value)
      : KeyValuePair(std::move(name)), m_value(value) {}

  std::string getValue() const override { return std::to_string(m_value); }

private:
  float m_value;
};

class String : public KeyValuePair {
public:
  String(std::string name, std::string value)
      : KeyValuePair(std::move(name)), m_value(std::move(value)) {}

  std::string getValue() const override { return "\"" + m_value + "\""; }

private:
  std::string m_value;
};

} // namespace Plotter