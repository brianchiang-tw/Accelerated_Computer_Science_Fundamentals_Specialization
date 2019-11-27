
/**
 * @file UnorderedMapCommon.h
 * University of Illinois CS 400, MOOC 3, Week 1: Unordered Map
 *
 * @author Eric Huber
 *
**/

// --------------------------------------------------------------
// This file has some common includes and helper functions
// that are used by several other parts of the program.
// --------------------------------------------------------------

// We use the pragma once directive to ensure that this header won't
// be included multiple times per compilation unit by mistake.
#pragma once

#include <string> // for std::string
#include <vector> // for std::vector
#include <utility> // for std::pair
#include <unordered_map> // for std::unordered_map
#include <chrono> // for std::chrono::high_resolution_clock

// ------------------------------------------------------------------------
//  About the timer code
// ----------------------
// We use this timer code to help catch mistakes that could cause one
// of the examples to infinite loop. You don't need to study how it works.

// References for chrono library usage:
// https://en.cppreference.com/w/cpp/chrono/high_resolution_clock/now
// https://en.cppreference.com/w/cpp/chrono/duration/duration_cast
using timeUnit = std::chrono::time_point<std::chrono::high_resolution_clock>;
static inline timeUnit getTimeNow() noexcept {
  return std::chrono::high_resolution_clock::now();
}
template <typename T>
static double getMilliDuration(T start_time, T stop_time) {
  std::chrono::duration<double, std::milli> dur_ms = stop_time - start_time;
  return dur_ms.count();
}
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//  About std::pair
// -----------------
// This "pair" type is a templated way to create a new type that pairs any
// two other types together as a unit.
// 
// When you want to construct a std::pair, there is a templated helper
// function that can construct ANY variation of pair for us. For example:
//
// std::pair<std::string, int> myPair = std::make_pair("hello", 7);
//
// When you do so, each pair has the members "first" and "second" that
// correspond to the two parts. As we define below, our StringIntPair type
// combines a string and an int. The "first" member is the string, and the
// "second" member is the int.
//
// std::string str = myPair.first; // str == "hello"
// int x = myPair.second; // x == 7
// ------------------------------------------------------------------------

// Here are some convenient type aliases for word counting.
// (We are just making alternative, shorter names for these STL types.)
using StringVec = std::vector<std::string>;
using StringIntPair = std::pair<std::string, int>;
using StringIntMap = std::unordered_map<std::string, int>;
using StringIntPairVec = std::vector<StringIntPair>;

// The palindrome exercise involves pairs of integer indices, and in particular,
// we want to be able to put these in unordered_map or unordered_set as keys,
// which requires hashing support. (We're going to need to specify how to hash
// a pair of integers, because the standard library doesn't support this natively.)
#include "IntPair.h" // for IntPair definition as well as custom hashing support

// Type alias for the palindrome problem:
// LengthMemo is a "memoization" table (please read the instructions PDF
// for more explanation about that). It maps a pair of int indices to an int
// palindrome length. This is meant to represent the maximum length of any
// palindrome substring found between the given indices.
using LengthMemo = std::unordered_map<IntPair, int>;

// Load the whole book "Through the Looking-Glass" as vector of strings.
// (This is handled for you.)
// Only strings of at least min_word_length are included (default: 5)
// and they will be converted to lowercase. Hyphens (-) and apostrophes (')
// are included in words where they are found, so strings like "alice" and
// "alice's" are counted separately as unique words.
StringVec loadBookStrings(unsigned int min_word_length=5);

// This helper function can be used to sort records of word counts
// based on the count. This is used in sortWordCounts.
bool wordCountComparator(const StringIntPair& x, const StringIntPair& y);

// sortWordCounts produces a fresh vector containing sorted copies
// of the word count records from wordcount_map.
StringIntPairVec sortWordCounts(const StringIntMap& wordcount_map);

// -------------------------------------------------------------------------
// makeWordCounts takes a vector of strings and counts the number of occurences
// of each unique string.
// NOTE: You will implement this function yourself in UnorderedMapExercises.cpp
StringIntMap makeWordCounts(const StringVec& words);
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// lookupWithFallback is a wrapper function for safely performing read-only
// lookups in a std::unordered_map object.
// NOTE: You will implement this function yourself in UnorderedMapExercises.cpp
int lookupWithFallback(const StringIntMap& wordcount_map, const std::string& key, int fallbackVal);
// -------------------------------------------------------------------------

// These functions make lists (actually, std::vector) of the most common and least common
// words found in the book. As input, these take the result of sortWordCounts.
// (These don't try to combine variations of words like "alice" and "alice's"
// in any way, so it's not entirely accurate.)
StringIntPairVec getBottomWordCounts(const StringIntPairVec& sorted_wordcounts, unsigned int max_words=20);
StringIntPairVec getTopWordCounts(const StringIntPairVec& sorted_wordcounts, unsigned int max_words=20);

// longestPalindromeLength uses brute-force recursion to calculate the
// longest palindrome substring within str, based on the left and right index
// limits given. It also takes clock information to prevent running too long,
// in some cases.
int longestPalindromeLength(const std::string& str, int leftLimit, int rightLimit, timeUnit startTime, double maxDuration);

// -------------------------------------------------------------------------
// This is a "memoized" version of the longestPalindromeLength function.
// (Please read the instructions PDF for information about what "memoization"
//  is all about.)
// NOTE: You will implement part of this function yourself in UnorderedMapExercises.cpp
int memoizedLongestPalindromeLength(LengthMemo& memo, const std::string& str, int leftLimit, int rightLimit, timeUnit startTime, double maxDuration);
// -------------------------------------------------------------------------

// reconstructPalindrome returns a copy of the longest palindrome within str,
// based on the information provided by the memoization table that has been
// previously calculated by memoizedLongestPalindromeLength.
std::string reconstructPalindrome(const LengthMemo& memo, const std::string& str);

// The timer code we use to prevent your functions from running too long by mistake
// can throw this exception to show what has happened. The unit tests handle this
// situation for you.
class TooSlowException : public std::runtime_error {
public:
  // import constructor from the base class
  using std::runtime_error::runtime_error;
};
