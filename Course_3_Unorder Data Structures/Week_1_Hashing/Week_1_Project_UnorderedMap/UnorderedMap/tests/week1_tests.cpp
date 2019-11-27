
// University of Illinois CS 400, MOOC 3, Week 1: Unordered Map
// Author: Eric Huber, University of Illinois staff
// Autograder based on Zephyr test runner by Prof. Wade Fagen-Ulmschneider and the CS 225 Course Staff
// Based on Catch2 unit testing framework

#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <chrono>

#include "../uiuc/catch/catch.hpp"

#include "../UnorderedMapCommon.h"

// May be useful in writing some tests
template <typename T>
void assertPtr(T* ptr) {
  if (!ptr) {
    throw std::runtime_error("Would have dereferenced a null pointer");
  }
}

template <typename T>
T& deref(T* ptr) {
  if (!ptr) {
    throw std::runtime_error("Would have dereferenced a null pointer");
  }
  else {
    return *ptr;
  }
}

// ========================================================================
// Tests: makeWordCounts
// ========================================================================

TEST_CASE("Testing makeWordCounts:", "[weight=1]") {

  constexpr int MIN_WORD_LENGTH = 5;
  StringVec bookstrings = loadBookStrings(MIN_WORD_LENGTH);
  StringIntMap wordcount_map = makeWordCounts(bookstrings);

  SECTION("Checking that map contains the right number of keys") {
    const int number_of_keys = wordcount_map.size();
    const int number_of_keys_expected = 2181;
    REQUIRE(number_of_keys == number_of_keys_expected);
  }

  SECTION("Checking that \"bandersnatch\" was counted 3 times") {
    bool found_bandersnatch = wordcount_map.count("bandersnatch");
    if (found_bandersnatch) {
      const int bandersnatch_count = wordcount_map["bandersnatch"];
      const int bandersnatch_count_expected = 3;
      REQUIRE(bandersnatch_count == bandersnatch_count_expected);
    }
    else {
      REQUIRE(found_bandersnatch);
    }
  }

  SECTION("Checking that \"alice\" was recorded 434 times") {
    bool found_alice = wordcount_map.count("alice");
    if (found_alice) {
      const int alice_count = wordcount_map["alice"];
      const int alice_count_expected = 434;
      REQUIRE(alice_count == alice_count_expected);
    }
    else {
      REQUIRE(found_alice);
    }
  }

  SECTION("Checking that \"frabjous\" was recorded 1 time") {
    bool found_frabjous = wordcount_map.count("frabjous");
    if (found_frabjous) {
      const int frabjous_count = wordcount_map["frabjous"];
      const int frabjous_count_expected = 1;
      REQUIRE(frabjous_count == frabjous_count_expected);
    }
    else {
      REQUIRE(found_frabjous);
    }
  }

}

// ========================================================================
// Tests: lookupWithFallback (with no dependency on makeWordCounts)
// ========================================================================

TEST_CASE("Testing lookupWithFallback: When the key exists", "[weight=1]") {

  StringIntMap wordcount_map;
  wordcount_map["bandersnatch"] = 3;

  SECTION("Should return the found value") {
    const int bandersnatch_count = lookupWithFallback(wordcount_map, "bandersnatch", 0);
    const int bandersnatch_count_expected = 3;
    REQUIRE(bandersnatch_count == bandersnatch_count_expected);
  }

  SECTION("Map should not be changed by lookup") {
    auto wordcount_map_backup = wordcount_map;
    const int bandersnatch_count = lookupWithFallback(wordcount_map, "bandersnatch", 0);
    REQUIRE(wordcount_map == wordcount_map_backup);
  }

}

TEST_CASE("Testing lookupWithFallback: When the key doesn't exist", "[weight=1]") {

  StringIntMap wordcount_map;

  SECTION("Should return the fallback value when key not found") {
    const int not_found_result = lookupWithFallback(wordcount_map, "cheshire", -7);
    const int not_found_result_expected = -7;
    REQUIRE(not_found_result == not_found_result_expected);
  }

  SECTION("Map should not be changed by lookup") {
    auto wordcount_map_backup = wordcount_map;
    const int cheshire_count = lookupWithFallback(wordcount_map, "cheshire", -7);
    REQUIRE(wordcount_map == wordcount_map_backup);
  }

}

// ========================================================================
// Tests: memoizedLongestPalindromeLength
// ========================================================================

TEST_CASE("Testing memoizedLongestPalindromeLength: Should return correct length", "[weight=1]") {

  SECTION("Should return correct length") {
    LengthMemo memo;
    const std::string str_small = "abbbcdeeeefgABCBAz";
    const double max_duration = 10000.0; // 10 seconds
    const auto start_time = getTimeNow();
    const auto str = str_small;
    const int pal_result = memoizedLongestPalindromeLength(memo, str, 0, str.length()-1, start_time, max_duration);
    const int pal_result_expected = 5;
    const bool returned_correct_length = (pal_result == 5);
    REQUIRE(pal_result == pal_result_expected);
  }

}

TEST_CASE("Testing memoizedLongestPalindromeLength: Memoization table should be complete", "[weight=1]") {

  SECTION("Should be able to reconstruct correct palindrome from memoization table") {
    LengthMemo memo;
    const std::string str_small = "abbbcdeeeefgABCBAz";
    const double max_duration = 10000.0; // 10 seconds
    const auto start_time = getTimeNow();
    const auto str = str_small;
    const int pal_result = memoizedLongestPalindromeLength(memo, str, 0, str.length()-1, start_time, max_duration);
    const std::string reconstructed_palindrome = reconstructPalindrome(memo, str);
    const std::string reconstructed_palindrome_expected = "ABCBA";
    REQUIRE(reconstructed_palindrome == reconstructed_palindrome_expected);
  }

}
