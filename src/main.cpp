#include <iostream>

#include "template.hpp"

int main() {
  std::cout << "Hello, World!" << std::endl;

  Template t(3.0f);

  std::cout << "Value: " << t.GetValue() << std::endl;

  t.SetValue(5.0F);

  std::cout << "Value: " << t.GetValue() << std::endl;

  return 0;
}