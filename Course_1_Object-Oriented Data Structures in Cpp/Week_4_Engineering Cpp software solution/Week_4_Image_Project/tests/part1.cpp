
// Original version by CS 225 Course Staff
// CS 400 revision with member type testing by Eric Huber

#include <type_traits>

#include "../uiuc/catch/catch.hpp"

#include "../ImageTransform.h"
#include "../uiuc/PNG.h"
#include "../uiuc/HSLAPixel.h"

namespace uiuc_testing {
  // Based on the floating point type detector template here:
  // https://en.cppreference.com/w/cpp/types/is_floating_point
  template< class T >
  struct is_double : std::integral_constant<bool, std::is_same<double, T>::value> {};
}

TEST_CASE("HSLAPixel should have member h as double", "[weight=1]") {
  REQUIRE(uiuc_testing::is_double<decltype(HSLAPixel::h)>::value);
}

TEST_CASE("HSLAPixel should have member s as double", "[weight=1]") {
  REQUIRE(uiuc_testing::is_double<decltype(HSLAPixel::s)>::value);
}

TEST_CASE("HSLAPixel should have member l as double", "[weight=1]") {
  REQUIRE(uiuc_testing::is_double<decltype(HSLAPixel::l)>::value);
}

TEST_CASE("HSLAPixel should have member a as double", "[weight=1]") {
  REQUIRE(uiuc_testing::is_double<decltype(HSLAPixel::a)>::value);
}

// The following tests are disabled.
// They evaluate constructors, but the Week 1-2 content doesn't cover constructors currently.
#undef DO_PART1_CTOR_TESTS
#ifdef DO_PART1_CTOR_TESTS

// HSLAPixel default constructor
TEST_CASE("HSLAPixel's default constructor should create a white pixel", "[weight=1]") {
  HSLAPixel pixel;
  REQUIRE( pixel.l == 1 );
}

TEST_CASE("HSLAPixel's default constructor should create an opaque pixel", "[weight=1]") {
  HSLAPixel pixel;
  REQUIRE( pixel.a == 1 );
}


// HSLAPixel 3-param constructor
TEST_CASE("HSLAPixel's HSL constructor should create a pixel with provided HSL values", "[weight=1]") {
  HSLAPixel pixel(260, 0.8, 0.7);
  REQUIRE( pixel.h == 260 );
  REQUIRE( pixel.s == 0.8 );
  REQUIRE( pixel.l == 0.7 );
}

TEST_CASE("HSLAPixel's HSL constructor should create an opaque pixel", "[weight=1]") {
  HSLAPixel pixel(260, 0.8, 0.7);
  REQUIRE( pixel.a == 1 );
}


// HSLAPixel 4-param constructor
TEST_CASE("HSLAPixel's HSLA constructor should create a pixel with provided HSLA values", "[weight=1]") {
  HSLAPixel pixel(260, 0.8, 0.7, 0.4);
  REQUIRE( pixel.h == 260 );
  REQUIRE( pixel.s == 0.8 );
  REQUIRE( pixel.l == 0.7 );
  REQUIRE( pixel.a == 0.4 );
}

#endif

