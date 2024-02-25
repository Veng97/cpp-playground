#include <array>
#include <iostream>

#include "template.hpp"

// function with memory leak
void func_to_show_mem_leak() { int* ptr = new int(5); }

int main() {
  std::cout << "Hello, World!" << std::endl;

  Template t(3.0f);

  std::cout << "Value: " << t.GetValue() << std::endl;

  func_to_show_mem_leak();

  t.SetValue(5.0F);

  std::cout << "Value: " << t.GetValue() << std::endl;

  return 0;
}