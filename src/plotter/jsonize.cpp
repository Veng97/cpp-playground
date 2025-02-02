#include "plotter/jsonize.hpp"

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace Plotter
{

KeyValuePair::KeyValuePair(std::string name) : m_key(std::move(name))
{
}

std::string_view KeyValuePair::getKey() const
{
  return m_key;
}

std::string toJson(const std::shared_ptr<KeyValuePair>& data, std::optional<double> timestamp, std::optional<unsigned long> reserve)
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

std::string toJson(const std::vector<std::shared_ptr<KeyValuePair>>& data, std::optional<double> timestamp, std::optional<unsigned long> reserve)
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

} // namespace Plotter
