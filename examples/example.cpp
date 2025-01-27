#include "plotter/publisher.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// Example struct with nested structs
struct Example
{

  bool enabled = true;

  struct Coordinate
  {
    int axis;
    double value;

    std::vector<std::shared_ptr<Plotter::KeyValuePair>> jsonize() const
    {
      return {std::make_shared<Plotter::Integer>("axis", axis), std::make_shared<Plotter::Float>("value", value)};
    };
  };

  Coordinate x;
  Coordinate y;
  Coordinate z;

  std::vector<std::shared_ptr<Plotter::KeyValuePair>> jsonize() const
  {
    return {
        std::make_shared<Plotter::Boolean>("enabled", enabled),
        std::make_shared<Plotter::Dict>("x", x.jsonize()),
        std::make_shared<Plotter::Dict>("y", y.jsonize()),
        std::make_shared<Plotter::Dict>("z", z.jsonize()),
    };
  };
};

int main()
{
  Plotter::Publisher pub("0.0.0.0", 9870);

  Example data = {true, {0, 1.123456789123456789}, {1, 2.0}, {2, 3.0}};

  std::cout << Plotter::valuesToJson(data.jsonize(), 0.0) << std::endl;

  // Simulate sending data periodically
  for (int i = 0; i < 10000; ++i)
  {
    data.enabled = !data.enabled;
    data.x.value = std::sin(i * 0.1);
    data.y.value = std::sin(i * 0.1 + M_PI_4);
    data.z.value = std::sin(i * 0.1 + M_PI_2);

    pub.publishWithTimestamp(data.jsonize(), i * 0.1);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}
