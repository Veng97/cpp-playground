#pragma once

class Template {
 private:
  float m_value;

 public:
  Template(const float& value) : m_value(value) {
    // Memory leak example
    double* ptr = new double(50);
  }

  ~Template() {}

  float GetValue() const { return m_value; }
  void SetValue(const float& value) { m_value = value; }
};
