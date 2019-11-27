
/**
 * @file PuzzleState.cpp
 * University of Illinois CS 400, MOOC 3, Week 3: Graph Search
 *
 * @author Eric Huber
 *
**/

#include "PuzzleState.h"

// There are some notes in GridGraph.cpp about what it means to instantiate
// a static class member variable.

// Instantiating this static class member variable at global scope.
// This works like a global variable throughout the program (in any source
// code file), but technically it's part of the PuzzleState class.
// See notes in PuzzleState.h.
bool PuzzleState::allowPlotting = true;

