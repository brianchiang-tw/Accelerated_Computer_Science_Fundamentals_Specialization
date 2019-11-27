
/**
 * @file IntPair.h
 * University of Illinois CS 400, MOOC 3, Week 1: Unordered Map
 *
 * @author Eric Huber
 *
**/

#pragma once

#include <functional> // for std::hash
#include <utility> // for std::pair
#include <string> // for std::string

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

