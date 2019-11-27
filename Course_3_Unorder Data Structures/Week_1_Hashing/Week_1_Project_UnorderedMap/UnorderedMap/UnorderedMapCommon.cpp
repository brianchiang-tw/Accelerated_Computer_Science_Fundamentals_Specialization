
/**
 * @file UnorderedMapCommon.cpp
 * University of Illinois CS 400, MOOC 3, Week 1: Unordered Map
 *
 * @author Eric Huber
 *
**/

// --------------------------------------------------------------
// This file has some common includes and helper functions
// that are used by several other parts of the program.
// --------------------------------------------------------------

// It makes sense to put certain includes in an implementation file as done
// here, rather than putting them all into the associated header file, when
// the included libraries are only referenced by the implementation itself
// and not the exposed interface (the function headers and return types,
// etc.). This avoids slowing down the compilation of other object files
// in your project, as well as polluting the namespaces of those files with
// stuff that isn't needed or used in the header file explicitly.
#include <stdexcept> // for std::runtime_error
#include <iostream> // for std::cerr, std::cout
#include <fstream> // for std::fstream
#include <string> // for std::string
#include <vector> // for std::vector
#include <cctype> // std::tolower
#include <algorithm> // for std::sorts
#include <regex> // for std::regex

#include "UnorderedMapCommon.h"

// Load the whole book "Through the Looking-Glass" as vector of strings.
// (This is handled for you.)
// Only strings of at least min_word_length are included (default: 5)
// and they will be converted to lowercase. Hyphens (-) and apostrophes (')
// are included in words where they are found, so strings like "alice" and
// "alice's" are counted separately as unique words.
StringVec loadBookStrings(unsigned int min_word_length) {

  static const std::string filename = "through_the_looking_glass.txt";
  static const std::string start_text = "CHAPTER I";
  static const std::string end_text = "End of the Project Gutenberg EBook";
  constexpr bool DEBUGGING = false;
  constexpr int DEBUGGING_MAX_WORDS = 30;

  // We'll just handle the input file as plain text. (It is actually encoded as UTF-8,
  // which is partially compatible as "plain text," but some special characters
  // or punctuation in the file cannot be represented as an ordinary "char" in C++.
  // In practice, parsing text robustly can be much more complicated than this,
  // but it's a topic for another course.)
  
  // Open the input file for reading with an ifstream, input file stream object.
  // If it can't be opened, throw an exception.
  std::ifstream instream(filename);
  if (!instream) {
    instream.close();
    throw std::runtime_error("Could not load book file: " + filename);
  }

  // Prepare a vector of strings.
  StringVec bookstrings;

  // Skipping the introduction section of the book file. This fast-forwards
  // through the file until the start_text string is found.
  {
    // Read lines of the input one at a time, as a temporary copy string "line".
    std::string line;
    while (std::getline(instream, line)) {
      // While we're still getting lines from the file... (This "while" block
      // doesn't execute at all if no lines could be retrieved, such as if
      // the file turned out to be blank, if we named the wrong file locally.)

      // Search within the line of text for the start_text string.
      // If not found at all, then this returns the special value std::string::npos.
      // So if the find() call does NOT equal std::string::npos, then we DID
      // find start_text within this line, and so we break out of the loop,
      // because we're done fast-forwarding.
      if (line.find(start_text) != std::string::npos) {
        break;
      }
    }
  }

  // We will read through the main body of the book one line at a time.
  std::string line;
  bool stillReading = true;
  while (stillReading && std::getline(instream, line)) {

    // If this line contains the ending of the book text,
    // then stop so we don't read the legal disclaimers.
    if (line.find(end_text) != std::string::npos) {
      stillReading = false;
      break;
    }

    // This uses regex ("regular expressions") to find and replace troublesome
    // string patterns. That makes it easier to parse below in a simple way.

    // Replace right single quotation mark with plain ASCII apostrophe:
    while (line.find("’") != std::string::npos) {
      line = std::regex_replace(line, std::regex("’"), "'");
    }

    // Replace double dash with a space. This preserves single hyphens.
    while (line.find("--") != std::string::npos) {
      line = std::regex_replace(line, std::regex("--"), " ");
    }

    // Parse individual words and convert them to lowercase.
    // (This part is a little tricky. Again, in practice, there are much
    // more robust libraries for parsing and validating strings according
    // to specific grammars. That is a topic for CS 421.)
    while (stillReading && line.length() > 0) {
      // Keep only letters and convert to lower case.
      std::string trimmed_word = "";
      int consumedChars = 0;
      char prevChar = ' ';
      for (char c : line) {
        consumedChars++;
        if (std::isalpha(c)) {
          char thisChar = std::tolower(c);
          if (prevChar == '\'') {
            // This happens when we have the sequence:
            // [letter][apostrophe][letter]
            // Then the first letter has already been recorded, and now
            // we'll put down the apostrophe, and then the following letter.
            trimmed_word += "'";
          }
          else if (prevChar == '-') {
            // Likewise, we'll allow [letter][dash][letter] to include
            // the dash in the word.
            trimmed_word += "-";
          }
          trimmed_word += thisChar;
          prevChar = thisChar;
        }
        else if ('\'' == c) {
          // Only note an apostrophe as a previous character if it follows
          // a letter. Don't add it to the recorded word yet.
          if (std::isalpha(prevChar)) {
            prevChar = '\'';
          }
          else {
            prevChar = ' ';
          }
        }
        else if ('-' == c) {
          // Only note a dash as a previous character if it follows
          // a letter. Don't add it to the recorded word yet.
          if (std::isalpha(prevChar)) {
            prevChar = '-';
          }
          else {
            prevChar = ' ';
          }
        }
        else break;
      }

      // Keep the part of the line that hasn't been consumed yet.
      // This is from the index equal to consumedChars through to the end.
      line = line.substr(consumedChars);

      if (trimmed_word.length() >= min_word_length) {
        bookstrings.push_back(trimmed_word);
      }

      // For debugging: Only load the first n words
      if (DEBUGGING && bookstrings.size() >= DEBUGGING_MAX_WORDS) {
        stillReading = false;
        break;
      }
    }
  }

  return bookstrings;
}

// This helper function can be used to sort records of word counts
// based on the count. This is used in sortWordCounts.
bool wordCountComparator(const StringIntPair& x, const StringIntPair& y) {
  // When defining a comparator (comparison helper function) like this one,
  // for use with std::sort, it's VERY IMPORTANT to define a "<" relationship
  // and NOT a "<=" relationship. Otherwise, std::sort may infinite loop.
  return x.second < y.second;
}

// sortWordCounts produces a fresh vector containing sorted copies
// of the word count records from wordcount_map.
StringIntPairVec sortWordCounts(const StringIntMap& wordcount_map) {
  // Copy all the wordcount entries from the map into a vector,
  // then sort the vector with our custom comparator.
  StringIntPairVec wordcount_vec;
  for (const StringIntPair& wc : wordcount_map) {
    wordcount_vec.push_back(wc);
  }
  std::sort(wordcount_vec.begin(), wordcount_vec.end(), wordCountComparator);
  // Return the the new vector (by value). This may or may not make a copy
  // depending on compiler optimizations, but in any case, we aren't worrying
  // about trying to pass references here. Notice that we're not using "new"
  // or "delete" anywhere.
  return wordcount_vec;
}

// -------------------------------------------------------------------------
// makeWordCounts: Can be found in UnorderedMapExercises.cpp
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// lookupWithFallback: Can be found in UnorderedMapExercises.cpp
// -------------------------------------------------------------------------

// Makes a list (actually, std::vector) of the least common words found in
// the book. As input, it takes the result of sortWordCounts.
StringIntPairVec getBottomWordCounts(const StringIntPairVec& sorted_wordcounts, unsigned int max_words) {
  StringIntPairVec bottom_wordcounts;
  for (const auto& wordcount_item : sorted_wordcounts) {
    if (bottom_wordcounts.size() < max_words) {
      bottom_wordcounts.push_back(wordcount_item);
    }
    else break;
  }
  return bottom_wordcounts;
}

// Makes a list (actually, std::vector) of the most common words found in
// the book. As input, it takes the result of sortWordCounts.
StringIntPairVec getTopWordCounts(const StringIntPairVec& sorted_wordcounts, unsigned int max_words) {
  StringIntPairVec top_wordcounts;
  for (auto rit = sorted_wordcounts.rbegin(); rit != sorted_wordcounts.rend(); rit++) {
    if (top_wordcounts.size() < max_words) {
      const auto& wordcount_item = *rit;
      top_wordcounts.push_back(wordcount_item);
    }
    else break;
  }
  return top_wordcounts;
}

// This uses brute-force recursion to calculate the longest palindrome substring
// within str, based on the left and right index limits given. It also takes
// clock information to prevent running too long, in some cases.
int longestPalindromeLength(const std::string& str, int leftLimit, int rightLimit, timeUnit startTime, double maxDuration) {
  // Base case: Return 0 as the longest palindrome length when the indices cross.
  // This could happen during our recursive steps defined below.
  if (leftLimit > rightLimit) {
    return 0;
  }

  // Otherwise, we know that leftLimit <= rightLimit.

  // Check validity of indices.
  if (leftLimit < 0) {
    throw std::runtime_error("leftLimit negative");
  }
  if (rightLimit < 0) {
    // This can happen in the base case, but then it would have been handled
    // above already.
    throw std::runtime_error("rightLimit negative, but it's not the base case");
  }
  
  // Apart from that, in the following code, the std::string::at() function
  // will throw an exception if an index is out of bounds.

  if (false) {
    // Debugging messages
    int range = rightLimit-leftLimit+1;
    if (range < 0) range = 0;
    std::cout << "Considering substring: " << str.substr(leftLimit, range) << std::endl;
    std::cout << " because l/r limits are: " << leftLimit << " " << rightLimit << std::endl;
  }

  // Some examples may take an absurdly long time to calculate with brute force.
  const auto currentTime = getTimeNow();
  const auto timeElapsed = getMilliDuration(startTime, currentTime);
  if (timeElapsed > maxDuration) {
    throw TooSlowException("taking too long");
  }

  // A single-character substring is a palindrome of size 1.
  // We include the character check with .at() to make sure the string isn't
  // empty and the indices are valid.
  if (leftLimit == rightLimit && str.at(leftLimit) == str.at(rightLimit)) {
    return 1;
  }

  // If the first and last character match, then...
  if (str.at(leftLimit) == str.at(rightLimit)) {
    // move left limit to the right
    int newLeft = leftLimit+1;
    // move right limit to the left
    int newRight = rightLimit-1;

    // Solve the middle subproblem.
    int middleSubproblemResult = longestPalindromeLength(str, newLeft, newRight, startTime, maxDuration);

    // (Base case note: Suppose that str had length 2, so after moving the indices,
    //  now newLeft > newRight. Because we handled the crossing case already,
    //  in that situation, middleSubproblemResult correctly gets value 0.)

    // For reference, let's calculate the longest length possible in the
    // middle substring range.
    int middleMaxLength = newRight-newLeft+1;
    // In the base case situation when the indices cross,
    // we force this value to be 0 instead of negative.
    if (middleMaxLength < 0) middleMaxLength = 0;
    
    // If the middle subproblem result equals the entire length
    // of the middle substring, then the middle substring is a palindrome.
    // So, since the first and last outer characters match each other,
    // the entire string between leftLimit and rightLimit is a palindrome.
    if (middleSubproblemResult == middleMaxLength) {
      return 2+middleSubproblemResult;
    }

    // Otherwise, don't return from the function yet!
    // We continue executing the code below.
  }

  // If we've reached this line in the function, we know the entire
  // string between leftLimit and rightLimit, inclusive, is NOT a palindrome.
  // That means we still need to try moving in each of the left limit
  // and right limit, separately, and compare the results.

  // Move the right limit to the left.
  int leftSubproblemResult = longestPalindromeLength(str, leftLimit, rightLimit-1, startTime, maxDuration);
  // Move the left limit to the right.
  int rightSubproblemResult = longestPalindromeLength(str, leftLimit+1, rightLimit, startTime, maxDuration);
  // Return whichever result was greater.
  return std::max(leftSubproblemResult,rightSubproblemResult);
}

// -------------------------------------------------------------------------
// memoizedLongestPalindromeLength: Can be found in UnorderedMapExercises.cpp
// -------------------------------------------------------------------------

// reconstructPalindrome returns a copy of the longest palindrome within str,
// based on the information provided by the memoization table that has been
// previously calculated by memoizedLongestPalindromeLength.
std::string reconstructPalindrome(const LengthMemo& memo, const std::string& str) {

  // If the input string is empty, just return an empty string.
  if (!str.length()) return "";

  // Begin with the limits on the first and last characters.
  int left = 0;
  int right = str.length()-1;

  // Make a working copy of the read-only memo.
  // We can then edit the copy destructively as we work, if needed.
  // (In practice, to avoid copying a lot of memory at the beginning, you
  // would want to do something more elaborate and careful to iterate over
  // the original memoization table in-place.)
  LengthMemo edit_memo = memo;

  // Note that making lookups with [] will insert default-initialized items
  // when the key isn't found! So, the memo will return 0 for missing items.
  // That will also edit the unordered_map in use, so we made a working copy.

  // We know that the best (longest) palindrome length is recorded in the
  // memoization table entry that represents the entire string from beginning
  // to end. So we begin with that best-ever value for comparison below.
  const int BEST_LEN = edit_memo[std::make_pair(left,right)];

  bool loop_again = true;
  while (loop_again) {
    // assume it's the final loop
    loop_again = false;

    // One at a time, we try moving the left limit to the right or the
    // right limit to the left. This narrows the substring. We only keep
    // changes that maintain the best palindrome length reported by the
    // memoization table.

    int test_left, test_right;

    // try moving left limit
    test_left = left+1;
    test_right = right;
    if (test_left <= test_right) {
      int test_len = edit_memo[std::make_pair(test_left,test_right)];
      if (test_len == BEST_LEN) {
        left = test_left;
        loop_again = true;
      }
    }

    // try moving right limit
    test_left = left;
    test_right = right-1;
    if (test_left <= test_right) {
      int test_len = edit_memo[std::make_pair(test_left,test_right)];
      if (test_len == BEST_LEN) {
        right = test_right;
        loop_again = true;
      }
    }
  }

  // After moving the limits inward as much as possible without lowering
  // the reported best palindrome length contained within, we have found
  // the actual palindrome substring. (But only if the indices are still
  // a valid substring range where left <= right.)
  if (left <= right) {
    return str.substr(left, right-left+1);
  }
  else {
    return "";
  }
}

