
/**
 * @file IntPair2.h
 * University of Illinois CS 400, MOOC 3, Week 3: Graph Search
 *
 * @author Eric Huber
 *
**/

// This file has more content than the IntPair.h that was originally released
// with the Week 1 assignment, Unordered Map. It includes operator<< support
// for IntPair, as well as a definition of IntPairPair (with hashing support).

#pragma once

#include <functional> // for std::hash
#include <tuple> // for std::pair
#include <string> // for std::string
#include <ostream> // for std:ostream

// We don't have to define an entire IntPair class ourselves since
// C++ provides a templated pair type already.
// We define IntPair as a type alias for convenience.
using IntPair = std::pair<int, int>;

// In the C++ standard library, classes like std::unordered_map and
// std::unordered_set use the default hashing object std::hash internally.
// However, std::hash doesn't natively support all possible types,
// just a few of the primitive C++ types like int and std::string.
// For more specific types, such as std::pair<int, int>, or for custom
// classes you make yourself, you need to specify a hashing function.
// Since std::hash is templated, we can add new cases for it using
// what is called "template specialization".

// In addition, custom class types made by users will also need to define
// their equivalence relation function, operator==, for this to work.
// Fortunately, std::pair<int,int> already gives us that automatically.

// ------------------------------------------------------------------

// Reference: https://en.cppreference.com/w/cpp/utility/hash

// We're adding more content to the standard namespace.
// (This isn't normally something you should do casually.)
namespace std {

  // A minor detail: std::hash is a struct, not a class.
  // In C++, a struct is very similar to a class, but it has public
  // members by default.

  // The "template <>" syntax indicates that we are specializing an existing
  // template for std::hash.

  template <>
  struct hash<IntPair> {

    // By overriding the () "operator", we can make an instance of a class
    // respond to similar syntax as if it were merely a function name.
    // The std::hash type is intended to work this way, as a "function object".
    // See also: https://en.cppreference.com/w/cpp/utility/functional

    // The () operator definition is where we will essentially define
    // our custom hashing function, and it returns the actual hash
    // as a std::size_t value (which is an integral type).
    std::size_t operator() (const IntPair& p) const {

      // We know that std::string has a well-defined hasher already,
      // so we'll turn our pair of ints into a unique string representation,
      // and then just hash that. We'll turn each integer into a string
      // and concatenate them with "##" in the middle, which should make
      // a unique string for any given pair of ints.
      std::string uniqueIntPairString = std::to_string(p.first) + "##" + std::to_string(p.second);

      // Get the default hashing function object for a std::string.
      std::hash<std::string> stringHasher;
      // Use the string hasher on our unique string.
      return stringHasher(uniqueIntPairString);
    }
  };

}

// Allow IntPair to be printed using std::cout << ...
static inline std::ostream& operator<<(std::ostream& os, const IntPair& point) {
  // For simplicity, we'll insert padding spaces according to the assumption
  // that point coordinates are only one or two digits each, and that they
  // are nonnegative. That is, they are between 0 and 99, inclusive.
  // Examples: " (2,3) " and " (4,56)".
  // THe padding space is necessary to make sure the grid lines leading
  // to and from the vertex stay aligned.
  if (point.first < 10 && point.first >= 0) os << " "; // optional padding space
  os << "(";
  os << point.first;
  os << ",";
  os << point.second;
  os << ")";
  if (point.second < 10 && point.second >= 0) os << " "; // optional padding space
  return os;
}

// Here we do the same hashing trick again for a pair of IntPair.
// In a few cases this could be useful for making a set of edges.
// However, with adjacency lists, we don't usually need to explicitly
// create a set of edges.
using IntPairPair = std::pair<IntPair, IntPair>;

namespace std {

  template <>
  struct hash<IntPairPair> {

    std::size_t operator() (const IntPairPair& twoPairs) const {
      auto p1 = twoPairs.first;
      auto p2 = twoPairs.second;
      std::string uniqueString;

      // Note that this hashing scheme does NOT reorder the pair of pairs
      // to ensure that (A,B) and (B,A) hash to the same bucket, although
      // that may be desirable for some use cases.
      // For example, we use IntPairPair to represent an undirected GridGraph
      // edge in this project, but there are certain lines of code in GridGraph.h
      // that manually ensure that ((1,2),(2,2)) and ((2,2),(1,2)) will be
      // considered equal when inserting to std::unordered_set.
      // We could take this further by making a new derived class based on
      // IntPairPair, explicitly intended for representing undirected edges only,
      // and then defining new equivalence operators and hashing support for it.
      // But this time, we will just let IntPairPair be generic, and manually
      // take care of the edge flipping in GridGraph.h.
      uniqueString =
        std::to_string(p1.first) + "##" + std::to_string(p1.second)
        + "##"
        + std::to_string(p2.first) + "##" + std::to_string(p2.second);

      // Get the default hashing function object for a std::string.
      std::hash<std::string> stringHasher;
      // Use the string hasher on our unique string.
      return stringHasher(uniqueString);
    }
  };

}

// Allow IntPairPair to be printed using std::cout << ...
static inline std::ostream& operator<<(std::ostream& os, const IntPairPair& points) {
  os << "(";
  os << points.first;
  os << ",";
  os << points.second;
  os << ")";
  return os;
}
