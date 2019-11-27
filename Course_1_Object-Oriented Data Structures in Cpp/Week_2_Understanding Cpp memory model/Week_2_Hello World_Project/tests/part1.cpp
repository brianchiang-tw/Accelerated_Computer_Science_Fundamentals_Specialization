
// University of Illinois
// CS 400 MP 0: Hello World
// Tests prepared by Eric Huber
// Based on Zephyr test runner by Prof. Wade Fagen-Ulmschneider and the CS 225 Course Staff
// Based on Catch2 unit testing framework

#include <cstdlib>

#include "../uiuc/catch/catch.hpp"

TEST_CASE("Should find your compiled main executable", "[weight=1]") {
  int result = std::system("test -x ./main");
  REQUIRE(0 == result);
}

// TEST_CASE("Should have ownership permissions to run your main executable", "[weight=1]") {
//   int result = std::system("test `stat -c \"%U\" ./test` = `whoami`");
//   REQUIRE(0 == result);
// }

TEST_CASE("Should find \"Hello, world!\" in standard output", "[weight=1]") {
  int result = std::system("./main | tr \'\n\' \' \' | tr A-Z a-z | sed \'s/[^a-z]//g\' | grep helloworld > /dev/null");
  REQUIRE(0 == result);
}

TEST_CASE("Should find \"Greetings from Illinois!\" in standard output", "[weight=1]") {
  int result = std::system("./main | tr \'\n\' \' \' | tr A-Z a-z | sed \'s/[^a-z]//g\' | grep greetingsfromillinois > /dev/null");
  REQUIRE(0 == result);
}

