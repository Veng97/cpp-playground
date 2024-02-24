#pragma once

class Template {
 private:
  float m_value;

 public:
  Template(const float& value) : m_value(value) {}
  ~Template() {}

  float GetValue() const { return m_value; }
  void SetValue(const float& value) { m_value = value; }
};
