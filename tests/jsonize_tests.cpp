
#include "plotter/types.hpp"

#include <gtest/gtest.h>
#include <memory>
#include <vector>

/**
 * @brief Tests JSON serialization of different types of key-value pairs.
 */
TEST(JsonizeTests, ValuesToJson)
{
  std::vector<std::shared_ptr<Plotter::KeyValuePair>> values = {
      std::make_shared<Plotter::Types::Boolean>("boolean", true),
      std::make_shared<Plotter::Types::Integer>("integer", 42),
      std::make_shared<Plotter::Types::Float>("float", 0.123456789),
  };

  std::string json = Plotter::valuesToJson(values);
  std::string expected_json = R"({"boolean":true,"integer":42,"float":0.123456789)";

  // Expect that the JSON string starts with the following substring.
  // For the float value, we expect these digits to be exact, but skip the rest as they are prone to floating point errors.
  EXPECT_TRUE(json.find(expected_json) == 0) << "  Actual: " << json << "\nExpected: " << expected_json;
}

/**
 * @brief Tests JSON serialization of a struct that jsonizes to a vector of KeyValuePair objects.
 */
TEST(JsonizeTests, JsonizeVectorFromStruct)
{
  struct Vec3
  {
    int x;
    int y;
    int z;

    std::vector<std::shared_ptr<Plotter::KeyValuePair>> jsonize() const
    {
      return std::vector<std::shared_ptr<Plotter::KeyValuePair>>{
          std::make_shared<Plotter::Types::Integer>("x", x),
          std::make_shared<Plotter::Types::Integer>("y", y),
          std::make_shared<Plotter::Types::Integer>("z", z),
      };
    }
  };

  Vec3 vec3{1, 2, 3};

  EXPECT_EQ(Plotter::structToJson(vec3), R"({"x":1,"y":2,"z":3})");
}

/**
 * @brief Tests JSON serialization of a struct that jsonizes to a single KeyValuePair object.
 */
TEST(JsonizeTests, JsonizeScalarFromStruct)
{
  struct Vec3
  {
    int x;
    int y;
    int z;

    std::shared_ptr<Plotter::KeyValuePair> jsonize() const
    {
      return std::make_shared<Plotter::Types::Dict>("vec3", std::vector<std::shared_ptr<Plotter::KeyValuePair>>{
                                                                std::make_shared<Plotter::Types::Integer>("x", x),
                                                                std::make_shared<Plotter::Types::Integer>("y", y),
                                                                std::make_shared<Plotter::Types::Integer>("z", z),
                                                            });
    }
  };

  Vec3 vec3{1, 2, 3};

  EXPECT_EQ(Plotter::structToJson(vec3), R"({"vec3":{"x":1,"y":2,"z":3}})");
}

/**
 * @brief Tests JSON serialization of a struct that jsonizes with timestamp.
 */
TEST(JsonizeTests, JsonizeWithTimestamp)
{
  struct Vec3
  {
    int x;
    int y;
    int z;

    std::shared_ptr<Plotter::KeyValuePair> jsonize() const
    {
      return std::make_shared<Plotter::Types::Dict>("vec3", std::vector<std::shared_ptr<Plotter::KeyValuePair>>{
                                                                std::make_shared<Plotter::Types::Integer>("x", x),
                                                                std::make_shared<Plotter::Types::Integer>("y", y),
                                                                std::make_shared<Plotter::Types::Integer>("z", z),
                                                            });
    }
  };

  Vec3 vec3{1, 2, 3};

  double timestamp = 0.1234;
  EXPECT_EQ(Plotter::structToJson(vec3, timestamp), R"({"timestamp":0.1234,"vec3":{"x":1,"y":2,"z":3}})");
}

/**
 * @brief Tests JSON serialization of a float vector.
 */
TEST(JsonizeTests, FloatVectorToJson)
{
  EXPECT_EQ(Plotter::valuesToJson({std::make_shared<Plotter::Types::Integers>("integers", std::vector<int>{1, 2})}), "{\"integers\":[1,2]}");
}