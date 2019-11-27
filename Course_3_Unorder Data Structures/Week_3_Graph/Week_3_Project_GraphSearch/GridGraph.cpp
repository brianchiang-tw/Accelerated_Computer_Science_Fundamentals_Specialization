
/**
 * @file GridGraph.cpp
 * University of Illinois CS 400, MOOC 3, Week 3: Graph Search
 *
 * @author Eric Huber
 *
**/

#include "GridGraph.h"

// Additional definitions for GridGraph

// ========================================================================

// Instantiating static class member variables

// In the class header file, we declared these static member variables, but
// declaring that they belong to the class didn't create them. These are like
// global variables; a single instance of these variables exists for the
// entire running program. We can't instantiate them in the class header file
// because then we'd get multiple object instances among the various cpp files
// that include our class header, which is a linker error. We also can't use
// namespace tricks (or anything else) to restrict the instance of the variable
// to a single .o file, because we do want all of the generated .o files to
// link with this single, global instance of the variable. We have to
// "instantiate" and initialize these static class members at global scope in
// a cpp file so they can be shared globally.

// You can also have static const members of a class, and those are easier
// to deal with, since they can just be defined and initialized in the class
// definition itself. For best results with that, use "static constexpr" and
// make sure the initializer is something that can be calculated at compile
// time.

// -------------------------------------------------------------------------

// Instantiating this static class member variable at global scope.
// This works like a global variable throughout the program (in any source
// code file), but technically it's part of the GridGraph class.
// See notes in GridGraph.h.
bool GridGraph::allowPlotting = true;

// See notes on GridGraph::allowPlotting in GridGraph.h
bool GridGraph::allowVerboseTextDescription = true;

// ========================================================================

// Additional GridGraph member function definitions

// These were declared in GridGraph.h but we're defining them here separately
// because of their size and complexity.

// -------------------------------------------------------------------------

// Allow a GridGraph to be plotted graphically in the terminal.
// For more accessible printouts, use printDetails instead.
std::ostream& GridGraph::plot(std::ostream& os) const {

  // If plotting is deactivated, do nothing.
  if (!GridGraph::allowPlotting) return os;

  if (adjacencyMap.empty()) {
    return os << "[Empty graph plot]" << std::endl;
  }

  // For printing, we'll assume (0,0) is the upper left of the plot,
  // the first coordinate increases as we go lower,
  // and the second coordinate increases as we go to the right.

  // Identify the min and max row and col in use by any point (or 0 for an empty graph).
  int minRow = 0;
  int minCol = 0;
  int maxRow = 0;
  int maxCol = 0;
  if (adjacencyMap.size() > 0) {
    // begin() returns an iterator. We use "->" as if it were a pointer.
    auto firstPointKey = adjacencyMap.begin()->first;
    maxRow = minRow = firstPointKey.first;
    maxCol = minCol = firstPointKey.second;
    // Loop over all the key-value pairs in the adjacency map.
    for (const auto& kv : adjacencyMap) {
      const auto& pointKey = kv.first;
      auto pointRow = pointKey.first;
      auto pointCol = pointKey.second;
      minRow = std::min(minRow, pointRow);
      minCol = std::min(minCol, pointCol);
      maxRow = std::max(maxRow, pointRow);
      maxCol = std::max(maxCol, pointCol);
    }
  }

  // We will plot the GridGraph starting from minRow, where minCol will be
  // on the left side of the terminal output. (This part is a bit complicated.)
  for (int row = minRow; row <= maxRow; row++) {
    // A string stream lets us do stream operations with "<<" to prepare a string.
    // We'll do this to prepare the decorations in the line below the current line
    // we are printing.
    std::stringstream understream;
    for (int col = minCol; col <= maxCol; col++) {
      // pos is the position at {row,col} in the grid, which may not actually
      // be a point in the graph
      IntPair pos = {row, col};
      // If this point is in the graph, put a point marker.
      if (hasPoint(pos)) {
        // Output the point (expected to be padded to 7 characters): " (1,2) ", "(12,34)"
        os << pos;
      }
      // If the point isn't in the graph, put padding spaces.
      else {
        os << "       ";
      }
      // posRight is the position to the right
      IntPair posRight = {row, col+1};
      // If there's an edge to the right, print a horizontal line
      if (hasEdge(pos,posRight)) {
        os << "----";
      }
      // If there's no edge to the right, and this isn't the rightmost column,
      // then put padding spaces
      else if (col < maxCol) {
        os << "    ";
      }
      // We'll prepare the understream if this isn't the bottom row.
      if (row < maxRow) {
        // pos_down is the position below the current position
        IntPair posDown = {row+1, col};
        // If there's an edge beneath this point, then draw a vertical line
        if (hasEdge(pos,posDown)) {
          understream << "   |   ";
        }
        // If there's no edge below, just draw spaces.
        else {
          understream << "       ";
        }
        // If it's not the rightmost column, then we need to put padding
        // spaces to the right for the understream also
        if (col < maxCol) {
          understream << "    ";
        }
      }
    }

    // We're done processing this row, so cap this line of os with an endline
    os << std::endl;

    // If it's the last row, we're leaving understream blank.
    // Otherwise, cap understream with an endline too.
    if (row < maxRow) understream << std::endl;

    // Now dump the contents of the understream to os, twice.
    os << understream.str() << understream.str();
  }

  return os;
}

// Print a list of the internal contents of a GridGraph structure in an accessible way.
std::ostream& GridGraph::printDetails(std::ostream& os) const {

  if (adjacencyMap.empty()) {
    return os << "The graph is empty." << std::endl;
  }

  if (!allowVerboseTextDescription) {
    return os << "Omitting verbose text description of graph." << std::endl;
  }

  // Please see the definition of plotGraph for more information on how
  // this function works, as they are similar.

  std::unordered_set<IntPairPair> edgeSet;
  std::unordered_set<IntPair> isolatedPointSet;

  // Loop over key-value pairs
  for (const auto& kv : adjacencyMap) {
    // key: point
    const auto& p1 = kv.first;
    // value: neighbor point set
    const auto& p1_neighbors = kv.second;

    // Points that have no adjacencies are isolated points, with no incident edges.
    if (p1_neighbors.empty()) {
      isolatedPointSet.insert(p1);
    }
    // Otherwise, insert all the edges found into the set of edges.
    // std::unordered_set only stores one element per key inserted,
    // so inserting the same key multiple times does nothing. You could
    // count the number of unique edges this way. Since we're interpreting
    // IntPairPair (a pair of IntPair) as an undirected edge here, we need
    // to be careful to ensure unique representation: For any points A and B,
    // the edge (A,B) is the same as the edge (B,A). To keep an edge key
    // consistent regardless of which direction it is viewed from, an ordering
    // is imposed on the endpoints using the "<" operator. Note that we can
    // compare two IntPair with "<" because STL gives us an implementation
    // of "<" for std::pair<int,int> by default.
    else {
      for (const auto& p2 : p1_neighbors) {
        IntPairPair edge;
        if (p1 < p2) {
          edge = std::make_pair(p1,p2);
        }
        else {
          // If the edge key looks "backwards," then flip it for uniformity.
          // That is, the same undirected edge (A,B) and (B,A) will always be
          // recorded as (A,B).
          edge = std::make_pair(p2,p1);
        }
        edgeSet.insert(edge);
      }
    }
  }

  // You can initialize a vector with an unordered set's contents
  // if you use this constructor that takes begin and end iterators.
  std::vector<IntPairPair> edgeVec(edgeSet.begin(), edgeSet.end());
  std::vector<IntPair> isolatedPointVec(isolatedPointSet.begin(), isolatedPointSet.end());

  // We'll sort these just for appearance.
  // Temporarily make a sorted array of the point coordinates using
  // std::vector and std::sort. This works because IntPair is defined
  // as a std::pair and it already has ordering relationships defined
  // by how the operator "<" is defined for the std::pair template:
  // The first coordinate is compared, and if equal, then the second
  // is compared.
  // e.g., (2,7) < (7,2) and (2,7) < (2,9)

  std::sort(edgeVec.begin(), edgeVec.end());
  std::sort(isolatedPointVec.begin(), isolatedPointVec.end());

  os << "Edges: ";
  if (edgeVec.empty()) os << "None." << std::endl;
  else {
    for (IntPairPair edge : edgeVec) {
      auto p1 = edge.first;
      auto p2 = edge.second;
      os << "[(" << p1.first << "," << p1.second << ") to (" << p2.first << "," << p2.second << ")] ";
    }
    os << std::endl;
  }

  os << "Isolated points: ";
  if (isolatedPointVec.empty()) os << "None." << std::endl;
  else {
    for (IntPair p : isolatedPointVec) {
      os << p << " ";
    }
    os << std::endl;
  }

  return os;
}


