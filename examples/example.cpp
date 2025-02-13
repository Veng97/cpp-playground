#include "plotter/jsonize.hpp"
#include "plotter/plotter.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <numbers>
#include <thread>
#include <vector>

// Example struct with nested structs
struct Example {

  bool enabled = true;

  struct Coordinate {
    int axis;
    double value;

    std::vector<std::shared_ptr<Plotter::KeyValuePair>> jsonize() const
    {
      return {
          std::make_shared<Plotter::Types::Integer>("axis", axis),
          std::make_shared<Plotter::Types::Float>("value", value),
      };
    };
  };

  Coordinate x;
  Coordinate y;
  Coordinate z;

  std::vector<std::shared_ptr<Plotter::KeyValuePair>> jsonize() const
  {
    return {
        std::make_shared<Plotter::Types::Boolean>("enabled", enabled),
        std::make_shared<Plotter::Types::Dict>("x", x.jsonize()),
        std::make_shared<Plotter::Types::Dict>("y", y.jsonize()),
        std::make_shared<Plotter::Types::Dict>("z", z.jsonize()),
    };
  };
};

int main()
{
  Plotter::Publisher pub("0.0.0.0", 9870);
  std::cout << "Publisher started on " << pub.address() << "\n";

  Example data = {true, {0, 1.123456789123456789}, {1, 2.0}, {2, 3.0}};
  std::cout << Plotter::toJson(data, 0.0) << "\n";

  // Simulate sending data periodically
  double timestep = 0.001;
  for (int i = 0; i < 100000; ++i) {

    data.enabled = !data.enabled;

    data.x.value = std::sin(i * 0.1);
    data.y.value = std::sin(i * 0.1 + std::numbers::pi / 4);
    data.z.value = std::sin(i * 0.1 + std::numbers::pi / 2);

    pub.publish(data, i * timestep);

    std::this_thread::sleep_for(std::chrono::duration<double>(timestep));
  }
}
