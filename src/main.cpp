#include "plotter/plotter.hpp"

#include <chrono>
#include <cmath>
#include <thread>

int main() {
  Plotter plotter("0.0.0.0", 9870);

  // Simulate sending data periodically
  for (int i = 0; i < 1000; ++i) {
    double value = std::sin(i * 0.1); // Example data
    plotter.sendData("sin_wave", value);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
