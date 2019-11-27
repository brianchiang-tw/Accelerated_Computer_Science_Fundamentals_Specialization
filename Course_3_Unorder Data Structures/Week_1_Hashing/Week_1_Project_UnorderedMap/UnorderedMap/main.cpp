
/**
 * @file main.cpp
 * University of Illinois CS 400, MOOC 3, Week 1: Unordered Map
 *
 * @author Eric Huber
**/

#include <iostream>
#include <string>

#include "UnorderedMapCommon.h"

// -----------------------------------------------------------------------
// Function prototypes: When global functions are defined later in the
// compilation unit than they are called, then at least these function
// prototype header lines must be written before the call.

// Some informal unit tests. Defined at the bottom of main.cpp
void informalTests();
// Helper function used by informalTests to either show a warning
// or throw an exception.
void errorReaction(std::string msg);

// Some informal testing suites. Definitions are below.
void wordcountTests();
void lookupWithFallbackTests();
void hashingDemo();
void palindromeTests();

// -----------------------------------------------------------------------

// main program: You can do whatever you want to test your code here.
// See also the test suite that you can build with "make test".
int main() {
  informalTests();
  return 0;
}

// ------------------------------------------------------------------------

// Choose what to do if an informal test has a big problem.
void errorReaction(std::string msg) {
  // If you want to display a warning and keep going:
  std::cout << std::endl << "WARNING: " << msg << std::endl << std::endl;;

  // If you want to throw an exception and stop the program immediately:
  // throw std::runtime_error(msg);  
}

// You should play around with these examples and check out the terminal output,
// in addition to trying the test program's unit tests.
void informalTests() {

  std::cout << "Running a few informal tests. Feel free to study these or edit them." << std::endl;
  std::cout << "The unit tests in the tests/ subdirectory are much more exhaustive!" << std::endl;
  std::cout << "You can run those by doing: make clean && make test && ./test" << std::endl << std::endl;

  // If you want to focus on a specific part, you could comment out some of these.
  wordcountTests();
  lookupWithFallbackTests();
  
  hashingDemo();
  palindromeTests();
  // yourOwnTests();

  std::cout << "\n\nInformal tests finished without crashing, but the output may have been incorrect."
    << "\nPlease scroll to the top of the output to read the diagnostics." << std::endl;

}

void wordcountTests() {

  std::cout << "======= Testing makeWordCounts =======" << std::endl << std::endl;

  std::cout << "Loading strings from \"Through the Looking-Glass\" by Lewis Carroll" << std::endl << std::endl;

  constexpr int MIN_WORD_LENGTH = 5;
  StringVec bookstrings = loadBookStrings(MIN_WORD_LENGTH);
  constexpr int MAX_STRINGS = 50;

  std::cout << "Preview of first " << MAX_STRINGS << " strings in the book "
    << "(of length " << MIN_WORD_LENGTH << " or more):" << std::endl;

  int word_count = 0;
  for (std::string s : bookstrings) {
    if (word_count >= MAX_STRINGS) break;
    word_count++;
    std::cout << s << " ";
  }
  std::cout << std::endl << std::endl;

  std::cout << "Total strings loaded: " << bookstrings.size() << std::endl;

  StringIntMap wordcount_map = makeWordCounts(bookstrings);
  std::cout << "Unique string occurrences: " << wordcount_map.size() << std::endl;
  StringIntPairVec sorted_wordcounts = sortWordCounts(wordcount_map);

  std::cout << "Sorted size: " << sorted_wordcounts.size() << std::endl << std::endl;

  StringIntPairVec top_wordcounts = getTopWordCounts(sorted_wordcounts, 20);
  std::cout << "20 most frequent words: " << std::endl;;
  {
    int i = 1;
    for (const StringIntPair& wordcount_item : top_wordcounts) {
      // wordcount_item.first
      auto word = wordcount_item.first;
      auto count = wordcount_item.second;
      std::cout << i << ": " << word << " - " << count << " times" << std::endl;
      i++;
    }
  }
  std::cout << std::endl;

  StringIntPairVec bottom_wordcounts = getBottomWordCounts(sorted_wordcounts, 20);
  std::cout << "20 least frequent words: " << std::endl;;
  {
    int i = 1;
    for (const StringIntPair& wordcount_item : bottom_wordcounts) {
      // wordcount_item.first
      auto word = wordcount_item.first;
      auto count = wordcount_item.second;
      std::cout << i << ": " << word << " - " << count << " times" << std::endl;
      i++;
    }
  }
  std::cout << std::endl;

  if (wordcount_map.bucket_count() < 1) {
    errorReaction("No buckets in wordcount_map");
  }
  else {
    std::cout << "Reported load factor: " << wordcount_map.load_factor() << std::endl;
    double load_factor = (double)wordcount_map.size() / wordcount_map.bucket_count();
    std::cout << "Calculated load factor: " << load_factor << std::endl << std::endl;
  }

  if (!wordcount_map.count("bandersnatch") || wordcount_map.at("bandersnatch") != 3) {
    std::cout << "Looks like makeWordCounts isn't implemented yet.\nSkipping lookupWithFallback tests based upon it." << std::endl << std::endl;
    return;
  }
  
  auto wordcount_map_backup = wordcount_map;

  std::cout << "Assuming makeWordCounts is working, here are some tests of lookupWithFallback:" << std::endl;
  int bandersnatch_count = lookupWithFallback(wordcount_map, "bandersnatch", 0);
  std::cout << "Looking up \"bandersnatch\" with fallback of 0. Result (expecting 3): " << bandersnatch_count << std::endl;
  if (bandersnatch_count != 3) errorReaction("Should have found \"bandersnatch\" with a count of 3.");
  else std::cout << "Good!" << std::endl;
  std::cout << "Checking that wordcount_map was not modified by the lookup:" << std::endl;
  if (wordcount_map_backup == wordcount_map) std::cout << "Good, wordcount_map wasn't changed by the lookup." << std::endl;
  else errorReaction("The lookup operation changed wordcount_map somehow.");
  std::cout << std::endl;

  wordcount_map_backup = wordcount_map;

  std::cout << "(Now, note that the Cheshire Cat does not appear in this book.)" << std::endl;
  std::cout << "Verifying that the original wordcount_map doesn't contain \"cheshire\":" << std::endl;
  if (wordcount_map.count("cheshire")) errorReaction("Found \"cheshire\" in the map. That shouldn't have happened.");
  else std::cout << "Good, \"cheshire\" was not found, as expected." << std::endl;
  int cheshire_count = lookupWithFallback(wordcount_map, "cheshire", -1);
  std::cout << "Looking up \"cheshire\" with fallback of -1. Result (expecting -1): " << cheshire_count << std::endl;
  if (cheshire_count != -1) errorReaction("Should not have found \"cheshire\" in map, and should have reported -1 as fallback.");
  else std::cout << "Good!" << std::endl;
  std::cout << "Checking that wordcount_map was not modified by the lookup:" << std::endl;
  if (wordcount_map_backup == wordcount_map) std::cout << "Good, wordcount_map wasn't changed by the lookup." << std::endl;
  else errorReaction("The lookup operation changed wordcount_map somehow.");
  std::cout << std::endl;

  std::cout << std::endl;
}

void lookupWithFallbackTests() {

  std::cout << "======= Checking lookupWithFallback without dependencies =======" << std::endl << std::endl;

  {
    StringIntMap wordcount_map;
    wordcount_map["bandersnatch"] = 3;

    auto wordcount_map_backup = wordcount_map;

    std::cout << "Testing lookupWithFallback with a specially constructed map:" << std::endl;
    int bandersnatch_count = lookupWithFallback(wordcount_map, "bandersnatch", 0);
    std::cout << "Looking up \"bandersnatch\" with fallback of 0. Result (expecting 3): " << bandersnatch_count << std::endl;
    if (bandersnatch_count != 3) errorReaction("Should have found \"bandersnatch\" with a count of 3.");
    else std::cout << "Good!" << std::endl;
    std::cout << "Checking that wordcount_map was not modified by the lookup:" << std::endl;
    if (wordcount_map_backup == wordcount_map) std::cout << "Good, wordcount_map wasn't changed by the lookup." << std::endl;
    else errorReaction("The lookup operation changed wordcount_map somehow.");
    std::cout << std::endl;    
  }

  {
    StringIntMap wordcount_map;

    auto wordcount_map_backup = wordcount_map;

    int cheshire_count = lookupWithFallback(wordcount_map, "cheshire", -1);
    std::cout << "Looking up \"cheshire\" with fallback of -1. Result (expecting -1): " << cheshire_count << std::endl;
    if (cheshire_count != -1) errorReaction("Should hould have reported -1 as fallback\n after not finding \"cheshire\" in the map.");
    else std::cout << "Good!" << std::endl;
    std::cout << "Checking that wordcount_map was not modified by the lookup:" << std::endl;
    if (wordcount_map_backup == wordcount_map) std::cout << "Good, wordcount_map wasn't changed by the lookup." << std::endl;
    else errorReaction("The lookup operation changed wordcount_map somehow.");
    std::cout << std::endl;
  }

}

void hashingDemo() {

  {
    std::cout << "======= Demonstrating the default string hasher =======" << std::endl << std::endl;

    std::hash<std::string> stringHasher;

    std::cout << "Notice that changing the string by a single character will significantly\n change its hash:" << std::endl << std::endl;

    {
      std::string str = "Jabberwock";
      std::cout << "String: \"" << str << "\"" << std::endl;
      auto hash_val = stringHasher(str);
      std::cout << "Hash value: " << hash_val << std::endl;
      std::cout << std::endl;
    }

    {
      std::string str = "Jabberwocky";
      std::cout << "String: \"" << str << "\"" << std::endl;
      auto hash_val = stringHasher(str);
      std::cout << "Hash value: " << hash_val << std::endl;
      std::cout << std::endl;
    }

    {
      std::string str = "jabberwocky";
      std::cout << "String: \"" << str << "\"" << std::endl;
      // Here is an example of more advanced C++ syntax you might see in the future.
      // In the follow line, the first apperance of "()" syntax only constructs
      // a temporary, anonymous instance of std::hash<std::string>.
      // Then, the object's operator() member is immediately called on str.
      auto hash_val = std::hash<std::string>()(str);
      std::cout << "Hash value: " << hash_val << std::endl;
      std::cout << std::endl;
    }
  }

  {
    std::cout << "======= Demonstrating our IntPair hasher =======" << std::endl << std::endl;

    std::cout << "Note:\n You don't need to manually invoke our custom hasher to do this assignment." << std::endl
      << " Instances of std::unordered_map<IntPair, int> will automatically use it." << std::endl << std::endl;

    std::hash<IntPair> hasher;

    {
      std::cout << "Pair: (0,0)" << std::endl;
      IntPair pair = std::make_pair(0,0);
      std::cout << "Hash value: " << hasher(pair) << std::endl;
      std::cout << std::endl;
    }

    {
      std::cout << "Pair: (0,1)" << std::endl;
      IntPair pair = std::make_pair(0,1);
      std::cout << "Hash value: " << hasher(pair) << std::endl;
      std::cout << std::endl;
    }

    {
      std::cout << "Pair: (1,0)" << std::endl;
      IntPair pair = std::make_pair(1,0);
      std::cout << "Hash value: " << hasher(pair) << std::endl;
      std::cout << std::endl;
    }
  }
}

void palindromeTests() {

  // overwritten by the brute force run below
  double slow_time = 9000.0;
  int intended_pal_result = 0;
  bool have_intended_result = false;

  // Small example: Brute force is pretty fast, but memoization gives a noticeable speedup.
  const std::string str_small = "abbbcdeeeefgABCBAz";
  // Medium example: Brute force takes several seconds. Memoization is very fast.
  const std::string str_medium = "abbbcdeeeefgABCBAabcdefgh";
  // Large example: Brute force will probably time out. Memoization is still very fast.
  const std::string str_large = "abbbcdeeeefgABCBAabcdefghijkl";
  // This should be the longest palindrome hidden in each example.
  const std::string correct_palindrome_substring = "ABCBA";

  // Which string to use for testing
  const std::string str = str_small;

  // Larger examples may take an extremely long time for brute force.
  // A correct memoized algorithm can handle them all very quickly, though.
  // For large problem sizes, the memoized version can be thousands of times faster.

  {
    std::cout << "======= Finding the longest palindrome substring (brute force) =======" << std::endl << std::endl;
    std::cout << "Note: This may take a few seconds. The duration scales up very quickly\n as the problem size increases." << std::endl << std::endl;
    std::cout << "String: \"" << str << "\"" << std::endl;
    auto start_time = getTimeNow();
    auto stop_time = start_time;
    double max_duration = 10000.0; // 10 seconds
    int pal_result = 99999;
    try {
      pal_result = longestPalindromeLength(str, 0, str.length()-1, start_time, max_duration);
      stop_time = getTimeNow();
      intended_pal_result = pal_result;
      have_intended_result = true;
      std::cout << "Length of longest palindrome substring: " << pal_result << std::endl;
    }
    catch (TooSlowException& e) {
      stop_time = getTimeNow();
      std::cout << "(Timeout!) Brute force is taking too long on this example. Giving up." << std::endl;
    }
    slow_time = getMilliDuration(start_time, stop_time);
    std::cout << "Calculation time (ms): " << slow_time << std::endl;
    std::cout << std::endl;
  }

  // overwritten by the memoized test run
  double memoized_time = 9000.0;

  {
    std::cout << "======= Finding the longest palindrome substring (memoized) =======" << std::endl << std::endl;
    std::cout << "Note: If you haven't finished correctly implementing this function yet," << std::endl
      << " the provided starter code may take even longer than brute force and time out." << std::endl
      << " That could also happen if you have a bug or infinite recursion." << std::endl << std::endl;
    std::cout << "String: \"" << str << "\"" << std::endl;
    
    // Creating the memoization std::unordered_map
    LengthMemo memo;

    int pal_result = 99999;
    auto start_time = getTimeNow();
    bool timed_out = false;
    double max_duration = slow_time;
    try {
      pal_result = memoizedLongestPalindromeLength(memo, str, 0, str.length()-1, start_time, max_duration);
    }
    catch (TooSlowException& e) {
      timed_out = true;
    }
    auto stop_time = getTimeNow();
    memoized_time = getMilliDuration(start_time, stop_time);
    if (timed_out) {
      std::cout << "(Timeout!) Elapsed time (ms): " << memoized_time << std::endl;
      errorReaction("Terminated memoized algorithm due to timeout.");
    }
    else {
      std::cout << "Length of longest palindrome substring: " << pal_result << std::endl;
      if (have_intended_result) {
        if (pal_result != intended_pal_result) errorReaction("Your memoized palindrome function returned the wrong result.");
        else std::cout << " (Correct!)" << std::endl;
      }
      std::cout << "Calculation time (ms): " << memoized_time << std::endl;
      double speedup = slow_time/memoized_time;
      std::cout << "Speedup over brute force: " << speedup << " times faster" << std::endl;
      if (speedup < 2.0) errorReaction("Should have been at least 2 times faster.\n (A correct memoized solution will probably even be MUCH faster than that.)");
      else std::cout << " (Good!)" << std::endl;
      
      std::string found_palindrome = reconstructPalindrome(memo, str);
      std::cout << "Palindrome found (based on memoization): \"" << found_palindrome << "\"" << std::endl;
      if (found_palindrome != correct_palindrome_substring) errorReaction("The substring found is incorrect, so the memoization table\n is inaccurate or incomplete.");
      else std::cout << " (Correct!)" << std::endl;
    }

    std::cout << std::endl;
  }

}

