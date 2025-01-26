#include "plotter/publisher.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// Example struct with nested structs
struct Inner
{
  int x;
  double y;

  std::vector<std::shared_ptr<Plotter::KeyValuePair>> jsonize() const
  {
    return {std::make_shared<Plotter::Integer>("x", x), std::make_shared<Plotter::Float>("y", y)};
  };
};

struct Outer
{
  std::string name;
  Inner inner;

  std::vector<std::shared_ptr<Plotter::KeyValuePair>> jsonize() const
  {
    return {std::make_shared<Plotter::String>("name", name), std::make_shared<Plotter::NestedFields>("inner", inner.jsonize())};
  };
};

int main()
{
  Plotter::Publisher pub("0.0.0.0", 9870);

  Outer outer = {"outer", {10, 3.123456789123456789}};

  std::cout << Plotter::structToJson(outer) << std::endl;

  // Simulate sending data periodically
  for (int i = 0; i < 1000; ++i)
  {
    // double value = std::sin(i * 0.1); // Example data
    // plotter.sendData("sin_wave", value);
    pub.publish(outer);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
