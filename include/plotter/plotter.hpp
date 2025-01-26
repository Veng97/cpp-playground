#include "plotter/publisher.hpp"

#include <iomanip>
#include <sstream>
#include <string>

class Plotter : public Publisher {
public:
  Plotter(std::string ip, int port) : Publisher(std::move(ip), port) {}
  ~Plotter() {}

  void sendData(const std::string &timeSeriesName, double value) {
    // Create a simple JSON string manually
    std::ostringstream json_stream;
    json_stream << "{"
                << "\"" << timeSeriesName << "\": " << std::fixed
                << std::setprecision(6) << value << "}";

    const std::string json_string = json_stream.str();
    publish(json_string);
  }

private:
};
