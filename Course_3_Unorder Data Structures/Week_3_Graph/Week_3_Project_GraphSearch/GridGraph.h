
/**
 * @file GridGraph.h
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
#include <vector>
#include <string>
#include <sstream> // for std::stringstream

#include "IntPair2.h" // for IntPair and point printing support

// This implements a very simple version of an adjacency list graph,
// where each vertex must be a point with 2D integer coordinates.
// For simplicity, we'll assume points are in (row, column) format,
// and they have integer coordinates between 0 and 99, inclusive.
class GridGraph {
public:
  // We use the concept of adjacency lists for our graph implementation.
  // However, each vertex's list of adjacent vertices will actually be a set.
  // For convenience, we make a type alias for a set of coordinate pairs.
  using NeighborSet = std::unordered_set<IntPair>;

  // adjacencyMap records both the vertices and (implicitly) the edges.
  // The keys are of type IntPair, which we mean to be grid point coordinates.
  // Each key entry in the map represents a vertex that exists in the graph.
  // The values are of type NeighborSet. Thus, each key is associated with a set of
  // coordinates that are the adjacent points (those connected by an edge).
  std::unordered_map<IntPair, GridGraph::NeighborSet> adjacencyMap;

  // For better accessibility with screen readers, you can set this to false
  // here to prevent GridGraph plotting diagrams from being displayed
  // verbosely in the terminal. This class member variable is "static"; the
  // static keyword in C++ can mean several different things, but in this
  // case, that means it's shared among all instances of the class, and
  // referred to globally as GridGraph::allowPlotting. We initialize this
  // setting as true in GridGraph.cpp. You can override this in your main
  // program like this:
  //   GridGraph::allowPlotting = false;
  // We've already mentioned a place in informalTests(), in main.cpp, where it
  // would be possible to do that.
  static bool allowPlotting;

  // See the discussion of allowPlotting above. When plotting is not allowed,
  // verbose text output will be used instead. This can be set to false in your
  // main program setup to also prevent that output, in case there are very
  // many edges and points to list.
  static bool allowVerboseTextDescription;

  // This sanity check ensures that we are defining a valid graph, since
  // we only want edges to join points that are exactly 1 unit apart.
  bool checkUnitDistance(const IntPair& p1, const IntPair& p2) const {
    int dist_x = p1.first - p2.first;
    int dist_y = p1.second - p2.second;
    // Compute the squared Euclidean distance.
    int dist2 = dist_x*dist_x + dist_y*dist_y;
    // We require the squared distance to be 1.
    // (No need to take the square root, because 1*1 == 1. It's very good
    //  that we can do this entirely with integer arithmetic, too.)
    return (dist2 == 1);
  }

  // Insert an isolated point to the graph (if it doesn't already exist).
  // If the point already exists, this basically does nothing.
  void insertPoint(const IntPair& p) {
    // Just by referring to given key with [], it is inserted to the
    // unordered_map if not already present.
    adjacencyMap[p];
  }

  // Implicitly insert an edge by creating a pair of associated vertices.
  // This inserts points p1 and p2 (if they don't already exist), and it
  // marks them as adjacent to each other.
  void insertEdge(const IntPair& p1, const IntPair& p2) {

    if (!checkUnitDistance(p1, p2)) {
      std::cerr << "Error: Can't add edge from " << p1 << " to " << p2 << std::endl;
      std::cerr << "Points must be 1 unit apart." << std::endl;
      throw std::runtime_error("Requested an invalid edge insertion");
    }

    // Ensure that p1 exists in the adjacency map,
    // and mark p2 as adjacent in its own adjacency set.
    adjacencyMap[p1].insert(p2);

    // To clarify about the syntax:
    // adjacencyMap[p1] creates an entry with key p1 in the map if necessary,
    // or otherwise it looks up the existing one, and then it returns a
    // reference to p1's associated NeighborSet. (If it was just created now by
    // this usage of [], then that set will be empty.)
    // Then, .insert(p2) calls the NeighborSet's insert() function to insert p2.

    // Now, do the same for p2 with respect to p1.
    adjacencyMap[p2].insert(p1);
  }

  // If the edge exists, remove it by having both points mark each other
  // as not adjacent.
  void removeEdge(const IntPair& p1, const IntPair& p2) {
    if (hasEdge(p1,p2)) {
      adjacencyMap[p1].erase(p2);
      adjacencyMap[p2].erase(p1);
    }
  }

  // Remove the point from the graph, if it exists, after removing all
  // the edges that connected to it.
  // (To be defined in GraphSearchExercises.cpp)
  void removePoint(const IntPair& p1);

  // Report whether the graph contains this point.
  bool hasPoint(const IntPair& p) const {
    return adjacencyMap.count(p);
  }

  // Can be used for convenience to see if two points are adjacent.
  bool hasEdge(const IntPair& p1, const IntPair& p2) const {

    // We'll check if the points are adjacent (i.e., there is an edge)
    // but we'll also do a sanity check in the process, to make sure
    // the graph setup doesn't have an inconsistency.
    int directions = 0;

    // "Short circuiting" in C++ ensures that if the first boolean
    // is false here, then the second one after "&&" will not be
    // evaluated. That's important because otherwise, the .at() call
    // could throw an exception we don't want.
    if (adjacencyMap.count(p1) && adjacencyMap.at(p1).count(p2)) {
      // If p1 is a point in the graph, and if it's marked adjacent to p2,
      // then we've seen one "direction" for this edge. An undirected
      // graph should not have unidirectional edges, so both endpoints
      // should be marked adjacent to each other if everything has been
      // set up correctly.
      directions++;
    }

    if (adjacencyMap.count(p2) && adjacencyMap.at(p2).count(p1)) {
      directions++;
    }

    if (directions == 0) {
      return false;
    }
    else if (directions == 2) {
      return true;
    }
    else {
      // This means one endpoint refers to the other, but not the other way
      // around. Both ends should be recorded as adjacent to each other.
      // Most likely this means there is either a bug in insertEdge,
      // or the programmer tried to insert an edge manually and made a mistake,
      // or an endpoint was deleted but maintenance was not performed on its
      // neighboring points' adjacency lists.
      throw std::runtime_error("hasEdge: edge found, but only in one direction");
    }
  }

  // Returns the number of vertices in the GridGraph currently.
  int countVertices() const {
    // Each key in the adjacencyMap represents one point in the graph,
    // so we just return the size of the std::unordered_map, which is
    // the number of keys it has.
    return adjacencyMap.size();
  }

  // Returns the number of edges in the GridGraph.
  // You'll implement this yourself in an exercise.
  int countEdges() const;

  // Check for equality between two GridGraph objects by comparing their
  // adjacency maps directly
  bool operator==(const GridGraph& other) const {
    return (adjacencyMap == other.adjacencyMap);
  }

  // Check for inequality of two GridGraph objects by delegating to the
  // implementation of operator==. Remember that "this" is a pointer to the
  // current instance of the class, so "*this" is the current GridGraph itself.
  bool operator!=(const GridGraph& other) const {
    return !(*this == other);
  }

  // Plot a GridGraph diagram in the terminal, graphically.
  // For more accessible printouts, use printDetails instead.
  // (The function body is defined in GridGraph.cpp)
  std::ostream& plot(std::ostream& os) const;

  // Print a list of the internal contents of a GridGraph structure in an accessible way.
  // (The function body is defined in GridGraph.cpp)
  std::ostream& printDetails(std::ostream& os) const;

  // Let the constructors and destructor just be the defaults!
  // Note that since all of the class's data is contained in STL
  // data structure types (unordered_map, unordered_set, pair),
  // these will use the appropriate memory management and destructors
  // that have already been defined for them by the standard library.

};

// Some of the member function bodies are defined in GridGraph.cpp, so please look there too.

// Adds << support for plotting a GridGraph.
static inline std::ostream& operator<<(std::ostream& os, const GridGraph& graph) {
  if (GridGraph::allowPlotting) {
    return graph.plot(os);
  }
  else {
    return graph.printDetails(os);
  }
}
