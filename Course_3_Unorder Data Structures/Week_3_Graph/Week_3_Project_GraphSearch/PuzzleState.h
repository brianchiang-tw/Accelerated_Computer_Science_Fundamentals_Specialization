
/**
 * @file PuzzleState.h
 * University of Illinois CS 400, MOOC 3, Week 3: Graph Search
 *
 * @author Eric Huber
 *
**/

#pragma once

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <algorithm> // for std::sort
#include <array>
#include <vector>
#include <string>
#include <sstream> // for std::stringstream
#include <functional> // for std::hash

// PuzzleState implements one state of the "8 puzzle", the sliding tile
// puzzle played with 8 tiles on a 3x3 grid, where a tile is allowed to slide
// into the blank space when adjacent. Internally, it stores the state with a
// std::array containing 9 ints. We are using std::array instead of a plain
// C-style array (such as "int data[9];") because this gives us full STL
// features that we expect. You can think of std::array as a std::vector that
// has a fixed size.
class PuzzleState {
private:

  // data:
  // An STL array containing 9 integers, representing the current tiles
  // in order, reading the rows from left to right. For example, if the
  // 9x9 grid is currently in the solved state, it looks like this:
  //  Top row:    1, 2, 3
  //  Middle row: 4, 5, 6
  //  Bottom row: 7, 8, (blank)
  // then we represent this as the array {1,2,3,4,5,6,7,8,9}, where 9 stands
  // for the blank space.
  std::array<int, 9> data;

public:

  // For better accessibility with screen readers, you can set this to false
  // here to prevent PuzzleState plotting diagrams from being displayed
  // verbosely in the terminal. This class member variable is "static"; the
  // static keyword in C++ can mean several different things, but in this
  // case, that means it's shared among all instances of the class, and
  // referred to globally as PuzzleState::allowPlotting. We initialize this
  // setting as true in PuzzleState.cpp. You can override this in your main
  // program like this:
  //   PuzzleState::allowPlotting = false;
  // We've already mentioned a place in informalTests(), in main.cpp, where it
  // would be possible to do that.
  static bool allowPlotting;

  // Return a copy of the internal state data.
  std::array<int, 9> getData() const {
    return data;
  }

  // Attempt to set the data to match the input parameter; if the input
  // array is clearly an invalid puzzle state, then we throw an exception.
  void setData(const std::array<int, 9>& newData) {
    if (!validateArray(newData)) {
      throw std::runtime_error("PuzzleState setData: invalid newData");
    }
    data = newData;
  }

  // Default constructor: Initialize the puzzle in the solved state.
  PuzzleState() : data({1,2,3,4,5,6,7,8,9}) {}

  // Argument constructor: Construct the PuzzleState to match the newData
  // array passed in. However, if the state is invalid, we throw an exception.
  PuzzleState(const std::array<int, 9>& newData) : data(newData) {
    if (!isValid()) {
      throw std::runtime_error("PuzzleState argument constructor: invalid newData");
    }
  }

  // Copy constructor: Initializes this PuzzleState based on another PuzzleState.
  // We throw an exception if the state is invalid.
  PuzzleState(const PuzzleState& other) : data(other.data) {
    if (!isValid()) {
      throw std::runtime_error("PuzzleState copy constructor: invalid other");
    }
  }

  // Copy assignment operator: Similar to the copy constructor.
  PuzzleState& operator=(const PuzzleState& other) {
    if (!other.isValid()) {
      throw std::runtime_error("PuzzleState copy assignment: invalid other");
    }
    data = other.data;
    return *this;
  }

  // validateArray:
  // Takes an int array as an argument. Checks that the array has 9 elements,
  // which must be some permutation (re-arrangement) of the ints 1 through 9.
  // in order to be a valid puzzle state.
  // Note: This is a static function of the class, so it belongs to
  // the entire class, not just one instance. Outside of the class declaration,
  // it could be called by users with "PuzzleState::validArray(...)".
  static bool validateArray(const std::array<int, 9>& data) {
    auto dataCopy = data;
    std::sort(dataCopy.begin(), dataCopy.end());
    if ( dataCopy != std::array<int,9>({1,2,3,4,5,6,7,8,9}) ) {
      std::cout << "validateArray warning: data must contain a permutation of numbers 1 through 9" << std::endl;
      return false;
    }
    return true;
  }

  // This is a member function that just calls validateArray on the current
  // class instance's own data array.
  bool isValid() const {
    return validateArray(data);
  }

  // Implement comparison with the equality operator for two PuzzleState objects.
  bool operator==(const PuzzleState& other) const {
    // This relies on the == operator that std::array already implements.
    return (data == other.data);
  }

  // Implement comparison with the non-equal operator.
  bool operator!=(const PuzzleState& other) const {
    return (data != other.data);
  }

  // Plot the puzzle diagram graphically in the terminal.
  // Example:
  //  -------
  // | 1 2 3 |
  // | 4 5 6 |
  // | 7 8   |
  //  -------
  std::ostream& plot(std::ostream& os) const {
    if (!PuzzleState::allowPlotting) return os;
    os << " -------" << std::endl;
    // Track which column it is
    int col = 0;
    // Iterate over the array of puzzle state numbers
    auto it = data.begin();
    while (it != data.end()) {
      int num = *it;
      // Put the left edge or left padding
      os << ((col == 0) ? "| " : " ");
      // Put blank for 9
      os << ((num == 9) ? " " : std::to_string(num));
      // Put the right edge or nothing
      os << ((col == 2) ? " |\n" : "");
      col++;
      col = col % 3;
      it++;
    }
    os << " -------" << std::endl;
    return os;
  }

  // This makes a string representation of the data array, e.g. [1,2,3,4,5,6,7,8,9]
  // Can be used along with normal std::cout operations for compact printing.
  std::string stringify() const {
    std::string s = "[";
    // (Recall that the 9 numbers are indexed 0 through 8.)
    // Iterate over the array, but not the last number
    for (int i=0; i<8; i++) {
      s += std::to_string(data.at(i));
      s += ",";
    }
    // Last number
    s += std::to_string(data.at(8));
    s += "]";
    return s;
  }

  // We conceptualize tile moves as moving the "blank" space through the puzzle,
  // although realistically, we would actually move the tile next to the blank
  // space into that space (in the opposite direction). So for the sake of our
  // code, we will think of moving the blank space (the 9 digit) around.

  // Find the index of the digit with a 9 (representing blank)
  int getBlankIndex() const {
    // find where 9 (the "blank") is and return that index
    for (int i=0; i<9; i++) {
      if (9==data[i]) return i;
    }
    // If not found, the puzzle state was already invalid somehow.
    throw std::runtime_error("getBlankIndex: not found; invalid puzzle state");
  }

  // Tries to get the puzzle state from moving the blank "up"
  //  (which slides the above tile "down" to fill the blank).
  // If this is possible, the outputState parameter is changed and true is returned.
  // Otherwise, false is returned.
  bool getUpState(PuzzleState& outputState) const {
    int blankIdx = getBlankIndex();
    // If blankIdx<3, the blank is in the top row, so there is no "up" state available.
    if (blankIdx<3) return false;
    // Replace the passed PuzzleState with a fresh working copy of the current state.
    outputState = *this;
    // We'll swap the blank with the tile above it.
    int upIdx = blankIdx-3;
    // Remember, the [] operator for std::array returns a reference,
    // so this swap does work directly on the array contents.
    std::swap(outputState.data[upIdx],outputState.data[blankIdx]);
    // The outputState has been updated in place, so we don't have to pass it back.
    // Return true since the state was found.
    return true;
  }

  // Tries to get the puzzle state from moving the blank "down"
  // See the comments on getUpState for more information.
  bool getDownState(PuzzleState& outputState) const {
    int blankIdx = getBlankIndex();
    if (blankIdx>5) return false;
    outputState = *this;
    int downIdx = blankIdx+3;
    std::swap(outputState.data[downIdx],outputState.data[blankIdx]);
    return true;
  }

  // Tries to get the puzzle state from moving the blank "left".
  // See the comments on getUpState for more information.
  bool getLeftState(PuzzleState& outputState) const {
    int blankIdx = getBlankIndex();
    if (blankIdx % 3 == 0) return false;
    outputState = *this;
    int leftIdx = blankIdx-1;
    std::swap(outputState.data[leftIdx],outputState.data[blankIdx]);
    return true;
  }

  // Tries to get the puzzle state from moving the blank "right".
  // See the comments on getUpState for more information.
  bool getRightState(PuzzleState& outputState) const {
    int blankIdx = getBlankIndex();
    if (blankIdx % 3 == 2) return false;
    outputState = *this;
    int RightIdx = blankIdx+1;
    std::swap(outputState.data[RightIdx],outputState.data[blankIdx]);
    return true;
  }

  // Returns a vector of PuzzleStates that can be reached from the current
  // state in one move. It is much more memory-efficient for us to recalculate
  // what the adjacent states are from any given state when needed, than to
  // pre-calculate and store every possible adjacent state for every possible
  // current state.
  std::vector<PuzzleState> getAdjacentStates() const {
    // Prepare a vector of states to record the adjacent states.
    std::vector<PuzzleState> adjacentStates;
    // We make a temporary PuzzleState to pass by reference to the "get"
    // functions below, since those functions return a bool to show success
    // or failure, and pass the result back through the reference variable
    // argument instead.
    PuzzleState tempState;
    // Try "moving" the blank in all four directions. If the move is possible,
    // then the tempState argument will be updated with the new state, which
    // we then store in the adjacentStates vector.
    if (getUpState(tempState)) {
      adjacentStates.push_back(tempState);
    }
    if (getDownState(tempState)) {
      adjacentStates.push_back(tempState);
    }
    if (getLeftState(tempState)) {
      adjacentStates.push_back(tempState);
    }
    if (getRightState(tempState)) {
      adjacentStates.push_back(tempState);
    }
    return adjacentStates;
  }

  // Check if the "other" state is one of the states adjacent to the current state
  bool isAdjacent(const PuzzleState& other) const {
    auto adjacentStates = getAdjacentStates();
    for (auto& neighbor : adjacentStates) {
      if (other == neighbor) return true;
    }
    return false;
  }

  // getRandomMove: a helper function for PuzzleState that randomly performs
  // a single valid tile slide move in the puzzle and returns the result.
  // That is, it returns a random adjacent state to the current state. We avoid
  // duplicating the previous state. This can be used to take random steps and
  // generate a puzzle for testing. If there is no previous state, or if you
  // don't care about possibly undoing the most recent step, then just pass the
  // current state as both the current and previous state.
  // ("static" here means the function does not belong to any one instance of
  //  the class. It belongs to the entire class type namespace, there is no
  //  "this" pointer. Instead we pass in what we need to do work on.)
  static PuzzleState getRandomMove(const PuzzleState& curState, const PuzzleState& prevState) {
    std::vector<PuzzleState> adjacentStates = curState.getAdjacentStates();
    std::vector<PuzzleState> validDestinations;
    for (auto& neighbor : adjacentStates) {
      if (neighbor != prevState) validDestinations.push_back(neighbor);
    }
    int randIdx = rand() % validDestinations.size();
    return validDestinations.at(randIdx);
  }

  // randomizePuzzle: a helper function for PuzzleState that generates puzzles
  //  with a known solution that is not too hard.
  // Given a the current state, curState, we do as many valid, random moves
  // as the maxSteps variable. We call it "max" steps because the resulting
  // puzzle state can be solved in AT MOST that many steps. In fact, we might
  // randomly produce a puzzle that has a shorter solution than that, but we
  // know it can't be any harder than that.
  // (See the comment on getRandomMove about static member functions.)
  static PuzzleState randomizePuzzle(PuzzleState curState, int maxSteps) {
    PuzzleState prevState = curState;
    for (int i=0; i<maxSteps; i++) {
      PuzzleState nextState = getRandomMove(curState, prevState);
      prevState = curState;
      curState = nextState;
    }
    return curState;
  }

};

// Implements operator<< support for plotting a PuzzleState diagram to a std::ostream.
static inline std::ostream& operator<<(std::ostream& os, const PuzzleState& puzzle) {
  if (PuzzleState::allowPlotting) {
    os << puzzle.stringify() << std::endl;
    return puzzle.plot(os);
  }
  else {
    os << puzzle.stringify() << std::endl;
    return os;
  }
}

// Adding std::hash support for PuzzleState objects so that a PuzzleState
// can be used as a key in std::unordered_map or std::unordered_set
// Reference: https://en.cppreference.com/w/cpp/utility/hash
namespace std {
  // There are more comments about how this works in the provided file IntPair2.h.
  // See also the instructions PDF for the Unordered Map project.
  template <>
  struct hash<PuzzleState> {
    std::size_t operator() (const PuzzleState& puzzle) const {
      // Create the unique string that compactly represents this puzzle state.
      std::string dataString = puzzle.stringify();
      // Get the default hashing function object for a std::string.
      std::hash<std::string> stringHasher;
      // Use the string hasher on our unique string.
      return stringHasher(dataString);
    }
  };
}

