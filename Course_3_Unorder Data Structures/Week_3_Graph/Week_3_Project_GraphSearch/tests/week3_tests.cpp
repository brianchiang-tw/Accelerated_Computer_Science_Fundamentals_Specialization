
// University of Illinois CS 400, MOOC 3, Week 3: Graph Search
// Author: Eric Huber, University of Illinois staff
// Autograder based on Zephyr test runner by Prof. Wade Fagen-Ulmschneider and the CS 225 Course Staff
// Based on Catch2 unit testing framework

#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <chrono>

#include "../GraphSearchCommon.h"

#include "../uiuc/catch/catch.hpp"

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
// Tests: countEdges
// ========================================================================

TEST_CASE("Testing countEdges:", "[weight=1][ex1]") {

  GridGraph graph;
  // top of the "I"
  graph.insertEdge(IntPair(0,0), IntPair(0,1));
  graph.insertEdge(IntPair(0,2), IntPair(0,1));
  // vertical bar of the "I"
  graph.insertEdge(IntPair(0,1), IntPair(1,1));
  graph.insertEdge(IntPair(2,1), IntPair(1,1));
  graph.insertEdge(IntPair(2,1), IntPair(3,1));
  // bottom of the "I"
  graph.insertEdge(IntPair(3,0), IntPair(3,1));
  graph.insertEdge(IntPair(3,2), IntPair(3,1));

  int numEdges = graph.countEdges();

  SECTION("Should not count each edge twice by mistake") {
    REQUIRE(numEdges != 14);
  }

  SECTION("Should count 7 edges") {
    REQUIRE(numEdges == 7);
  }

}

// ========================================================================
// Tests: removePoint
// ========================================================================

TEST_CASE("Testing removePoint:", "[weight=1][ex1]") {

  // Reference graph with only two points, for use later
  GridGraph edgelessGraph;
  edgelessGraph.insertPoint(IntPair(1,2));
  edgelessGraph.insertPoint(IntPair(2,1));

  // Working graph for testing
  GridGraph graph;

  // initial graph
  graph.insertEdge(IntPair(2,2), IntPair(1,2));
  graph.insertEdge(IntPair(2,2), IntPair(2,1));
  const GridGraph backupGraph = graph;

  SECTION("Checking that point (2,2) was removed") {

    // Removing point (2,2)
    graph.removePoint(IntPair(2,2));

    REQUIRE(!graph.hasPoint(IntPair(2,2)));

    // looping over key-value pairs in the adjacency map
    for (const auto& kv : graph.adjacencyMap) {
      // the key type is a point (int pair)
      IntPair k = kv.first;
      REQUIRE(k != IntPair(2,2));
      if (k == IntPair(2,2)) {
        throw std::runtime_error("Found (2,2) key in the adjacencyMap");
      }
      else {
        // the value type is a set of neighboring points that are adjacent to this point in the graph
        const GridGraph::NeighborSet& adjacencies = kv.second;
        for (const IntPair& neighbor : adjacencies) {
          REQUIRE(neighbor != IntPair(2,2));
          if (neighbor == IntPair(2,2)) {
            throw std::runtime_error("Found (2,2) listed as an adjacent point somewhere");
          }
        }
      }
    }
  }

  SECTION("Checking that no other contents were added") {
    // Removing point (2,2)
    graph.removePoint(IntPair(2,2));
    REQUIRE(graph == edgelessGraph);
  }

  SECTION("Trying to remove a non-existent point") {
    graph = backupGraph;
    graph.removePoint(IntPair(7,7));
    REQUIRE(graph == backupGraph);
  }

}

// ========================================================================
// Test: graphBFS when the goal is unreachable
// ========================================================================

TEST_CASE("Testing graphBFS when the goal is unreachable:", "[weight=1][ex2][timeout=60000]") {

  GridGraph graph;
  // We'll leave these points (and their attached edges) out of the graph.
  std::unordered_set<IntPair> exclusionSet = {{6,0}, {0,5}};

  // Going to start with a 6x5 mesh.
  // Make vertical edges
  for (int row=0; row<=5; row++) {
    for (int col=0; col<=5; col++) {
      auto p1 = IntPair(row,col);
      auto p2 = IntPair(row+1,col);
      if (exclusionSet.count(p1) || exclusionSet.count(p2)) continue;
      graph.insertEdge(p1,p2);
    }
  }

  // Make horizontal edges
  for (int row=0; row<=6; row++) {
    for (int col=0; col<=4; col++) {
      auto p1 = IntPair(row,col);
      auto p2 = IntPair(row,col+1);
      if (exclusionSet.count(p1) || exclusionSet.count(p2)) continue;
      graph.insertEdge(p1,p2);
    }
  }

  // Remove certain edges to leave a maze. (These coordinates were figured out beforehand.)
  // Note that for convenience, we can use the C++11 "initializer" syntax in curly braces {}
  // to concisely enter all these nested objects. As long as the compiler can find constructors
  // that match the types we're using, it will be okay.
  std::vector<IntPairPair> edgesToRemove = {
    {{0,1},{0,2}}, {{0,3},{1,3}}, {{1,1},{2,1}}, {{1,2},{1,3}}, {{1,3},{2,3}},
    {{1,4},{2,4}}, {{2,0},{2,1}}, {{2,1},{2,2}}, {{2,2},{2,3}}, {{3,0},{3,1}},
    {{3,2},{3,3}}, {{3,2},{4,2}}, {{3,3},{4,3}}, {{3,4},{3,5}}, {{4,0},{4,1}},
    {{4,1},{4,2}}, {{4,3},{5,3}}, {{4,4},{4,5}}, {{4,4},{5,4}}, {{4,5},{5,5}},
    {{5,1},{5,2}}, {{6,1},{6,2}}, {{6,2},{6,3}}, {{6,4},{6,5}},
    // Removing this last edge separates the graph into two connected components,
    // where the start no longer has a path to the goal:
    {{0,2},{0,3}}
  };

  for (auto e : edgesToRemove) {
    auto p1 = e.first;
    auto p2 = e.second;
    graph.removeEdge(p1,p2);
  }

  const auto maze_start = IntPair(0,0);
  const auto maze_goal = IntPair(6,5);

  std::cout << std::endl << "Now testing graphBFS when the goal is unreachable." << std::endl
    << "This may take several seconds and display a warning (which is OK)." << std::endl
    << "If you want to cancel the test, press this key combination: Ctrl c" << std::endl << std::endl;
  std::list<IntPair> path = graphBFS(maze_start, maze_goal, graph);

  SECTION("Should return empty path when goal unreachable") {
    REQUIRE(path.empty());
  }

}

// ========================================================================
// Test: graphBFS when start==goal
// ========================================================================

TEST_CASE("Testing graphBFS when start==goal:", "[weight=1][ex2]") {

  GridGraph graph;

  std::vector<std::vector<int>> edgesToInsert = {{0,0,0,1}, {0,1,0,2}};
  for (auto e : edgesToInsert) {
    auto p1 = IntPair(e[0],e[1]);
    auto p2 = IntPair(e[2],e[3]);
    graph.insertEdge(p1,p2);
  }

  const auto maze_start = IntPair(0,0);
  const auto maze_goal = maze_start;
  std::list<IntPair> path = graphBFS(maze_start, maze_goal, graph);

  SECTION("Test configuration sanity check") {
    REQUIRE(maze_start == maze_goal);
  }

  SECTION("Should not return empty path when start==goal") {
    REQUIRE(!path.empty());
  }

  SECTION("Should return path containing only the start") {
    REQUIRE(maze_start == maze_goal);
    REQUIRE(path.front() == maze_start);
    REQUIRE(path.back() == maze_goal);
  }

  // somewhat redundant
  SECTION("Should return path of size 1") {
    REQUIRE(path.size() == 1);
  }

}


// ========================================================================
// Test: graphBFS with normal maze
// ========================================================================

TEST_CASE("Testing graphBFS when a path can be found:", "[weight=1][ex2]") {

  GridGraph graph;
  // We'll leave these points (and their attached edges) out of the graph.
  std::unordered_set<IntPair> exclusionSet = {{6,0}, {0,5}};

  // Going to start with a 6x5 mesh.
  // Make vertical edges
  for (int row=0; row<=5; row++) {
    for (int col=0; col<=5; col++) {
      auto p1 = IntPair(row,col);
      auto p2 = IntPair(row+1,col);
      if (exclusionSet.count(p1) || exclusionSet.count(p2)) continue;
      graph.insertEdge(p1,p2);
    }
  }

  // Make horizontal edges
  for (int row=0; row<=6; row++) {
    for (int col=0; col<=4; col++) {
      auto p1 = IntPair(row,col);
      auto p2 = IntPair(row,col+1);
      if (exclusionSet.count(p1) || exclusionSet.count(p2)) continue;
      graph.insertEdge(p1,p2);
    }
  }

  // Remove certain edges to leave a maze. (These coordinates were figured out beforehand.)
  // Note that for convenience, we can use the C++11 "initializer" syntax in curly braces {}
  // to concisely enter all these nested objects. As long as the compiler can find constructors
  // that match the types we're using, it will be okay.
  std::vector<IntPairPair> edgesToRemove = {
    {{0,1},{0,2}}, {{0,3},{1,3}}, {{1,1},{2,1}}, {{1,2},{1,3}}, {{1,3},{2,3}},
    {{1,4},{2,4}}, {{2,0},{2,1}}, {{2,1},{2,2}}, {{2,2},{2,3}}, {{3,0},{3,1}},
    {{3,2},{3,3}}, {{3,2},{4,2}}, {{3,3},{4,3}}, {{3,4},{3,5}}, {{4,0},{4,1}},
    {{4,1},{4,2}}, {{4,3},{5,3}}, {{4,4},{4,5}}, {{4,4},{5,4}}, {{4,5},{5,5}},
    {{5,1},{5,2}}, {{6,1},{6,2}}, {{6,2},{6,3}}, {{6,4},{6,5}}
  };

  for (auto e : edgesToRemove) {
    auto p1 = e.first;
    auto p2 = e.second;
    graph.removeEdge(p1,p2);
  }

  const auto maze_start = IntPair(0,0);
  const auto maze_goal = IntPair(6,5);
  std::list<IntPair> path = graphBFS(maze_start, maze_goal, graph);

  SECTION("Path should not be empty") {
    REQUIRE(!path.empty());
  };

  SECTION("Path should have ended at the goal") {
    REQUIRE(!path.empty());
    REQUIRE(path.back() == maze_goal);
  };

  SECTION("Path should have begun at the start") {
    REQUIRE(!path.empty());
    REQUIRE(path.front() == maze_start);
  };

  SECTION("Path can only use existing edges in the graph") {
    REQUIRE(!path.empty());
    REQUIRE(path.front() == maze_start);
    REQUIRE(path.back() == maze_goal);

    // Make a working copy of the path that we can take apart
    GridGraph newGraph;
    auto working_path = path;
    // Begin from the start of the path and remove the key from the list
    auto path_start = working_path.front();
    working_path.pop_front();
    // Insert the start as an isolated point
    newGraph.insertPoint(path_start);
    // Walk through subsequent pairs and add edges
    int num_steps = 0;
    auto p1 = path_start;
    while (!working_path.empty()) {
      auto p2 = working_path.front();
      working_path.pop_front();
      newGraph.insertEdge(p1,p2);
      REQUIRE(graph.hasEdge(p1,p2));
      // step forward
      num_steps++;
      p1 = p2;
    }
  }

  SECTION("Path should be the shortest solution possible") {
    REQUIRE(!path.empty());
    REQUIRE(path.front() == maze_start);
    REQUIRE(path.back() == maze_goal);

    // Make a working copy of the path that we can take apart
    GridGraph newGraph;
    auto working_path = path;
    // Begin from the start of the path and remove the key from the list
    auto path_start = working_path.front();
    working_path.pop_front();
    // Insert the start as an isolated point
    newGraph.insertPoint(path_start);
    // Walk through subsequent pairs and add edges
    int num_steps = 0;
    auto p1 = path_start;
    while (!working_path.empty()) {
      auto p2 = working_path.front();
      working_path.pop_front();
      newGraph.insertEdge(p1,p2);
      REQUIRE(graph.hasEdge(p1,p2));
      // step forward
      num_steps++;
      p1 = p2;
    }

    REQUIRE(num_steps == 19);
  }

}

// ========================================================================
// Test: puzzleBFS when the goal is unreachable (puzzle can't be solved)
// ========================================================================

TEST_CASE("Testing puzzleBFS when the goal is unreachable (puzzle can't be solved):", "[weight=1][ex3][timeout=60000]") {

  const PuzzleState puzzle_start({1,3,2,4,5,6,7,8,9});
  const PuzzleState puzzle_goal({1,2,3,4,5,6,7,8,9});

  std::cout << std::endl << "Now testing puzzleBFS when the goal is unreachable." << std::endl
    << "This may take several seconds and display a warning (which is OK)." << std::endl
    << "If you want to cancel the test, press this key combination: Ctrl c" << std::endl << std::endl;

  std::list<PuzzleState> path = puzzleBFS(puzzle_start, puzzle_goal);

  SECTION("Path should be empty") {
    REQUIRE(path.empty());
  }

}

// ========================================================================
// Test: puzzleBFS when start==goal
// ========================================================================

TEST_CASE("Testing puzzleBFS when start==goal:", "[weight=1][ex3]") {

  const PuzzleState puzzle_start({1,2,3,4,5,6,7,8,9});
  const PuzzleState puzzle_goal = puzzle_start;

  std::list<PuzzleState> path = puzzleBFS(puzzle_start, puzzle_goal);

  SECTION("Test configuration sanity check") {
    REQUIRE(puzzle_start == puzzle_goal);
  }

  SECTION("Should not return empty path when start==goal") {
    REQUIRE(!path.empty());
  }

  SECTION("Should return path containing only the start") {
    REQUIRE(puzzle_start == puzzle_goal);
    REQUIRE(path.front() == puzzle_start);
    REQUIRE(path.back() == puzzle_goal);
  }

  // somewhat redundant
  SECTION("Should return path of size 1") {
    REQUIRE(path.size() == 1);
  }

}

// ========================================================================
// Test: puzzleBFS with a simple puzzle
// ========================================================================

TEST_CASE("Testing puzzleBFS when there is a solution:", "[weight=1][ex3]") {

  const PuzzleState puzzle_start({1,2,3,4,6,9,7,5,8});
  const PuzzleState puzzle_goal({1,2,3,4,5,6,7,8,9});
  const int CORRECT_STEPS = 3;

  std::list<PuzzleState> path = puzzleBFS(puzzle_start, puzzle_goal);

  SECTION("Path should not be empty") {
    REQUIRE(!path.empty());
  };

  SECTION("Path should have ended at the goal") {
    REQUIRE(!path.empty());
    REQUIRE(path.back() == puzzle_goal);
  };

  SECTION("Path should have begun at the start") {
    REQUIRE(!path.empty());
    REQUIRE(path.front() == puzzle_start);
  };

  SECTION("Solution can only use valid puzzle moves") {
    REQUIRE(!path.empty());
    REQUIRE(path.front() == puzzle_start);
    REQUIRE(path.back() == puzzle_goal);

    int num_steps = 0;

    // Make a working copy of the path that we can take apart
    auto working_path = path;
    auto path_start = working_path.front();
    working_path.pop_front();
    auto state1 = path_start;

    while (!working_path.empty()) {
      auto state2 = working_path.front();
      working_path.pop_front();
      REQUIRE(state1.isAdjacent(state2));
      // step forward
      num_steps++;
      state1 = state2;
    }
  }

  SECTION("Should find the shortest solution") {
    REQUIRE(!path.empty());
    REQUIRE(path.front() == puzzle_start);
    REQUIRE(path.back() == puzzle_goal);

    int num_steps = 0;

    // Make a working copy of the path that we can take apart
    auto working_path = path;
    auto path_start = working_path.front();
    working_path.pop_front();
    auto state1 = path_start;

    while (!working_path.empty()) {
      auto state2 = working_path.front();
      working_path.pop_front();
      REQUIRE(state1.isAdjacent(state2));
      // step forward
      num_steps++;
      state1 = state2;
    }

    REQUIRE(num_steps == CORRECT_STEPS);
  }

}

// ========================================================================
// Test: puzzleBFS with a harder puzzle
// ========================================================================

TEST_CASE("Testing puzzleBFS on a harder puzzle:", "[weight=1][ex3]") {

  const PuzzleState puzzle_start({9,2,6,1,3,5,4,7,8});
  const PuzzleState puzzle_goal({1,2,3,4,5,6,7,8,9});
  const int CORRECT_STEPS = 10;

  std::list<PuzzleState> path = puzzleBFS(puzzle_start, puzzle_goal);

  SECTION("Path should not be empty") {
    REQUIRE(!path.empty());
  };

  SECTION("Path should have ended at the goal") {
    REQUIRE(!path.empty());
    REQUIRE(path.back() == puzzle_goal);
  };

  SECTION("Path should have begun at the start") {
    REQUIRE(!path.empty());
    REQUIRE(path.front() == puzzle_start);
  };

  SECTION("Solution can only use valid puzzle moves") {
    REQUIRE(!path.empty());
    REQUIRE(path.front() == puzzle_start);
    REQUIRE(path.back() == puzzle_goal);

    int num_steps = 0;

    // Make a working copy of the path that we can take apart
    auto working_path = path;
    auto path_start = working_path.front();
    working_path.pop_front();
    auto state1 = path_start;

    while (!working_path.empty()) {
      auto state2 = working_path.front();
      working_path.pop_front();
      REQUIRE(state1.isAdjacent(state2));
      // step forward
      num_steps++;
      state1 = state2;
    }
  }

  SECTION("Should find the shortest solution") {
    REQUIRE(!path.empty());
    REQUIRE(path.front() == puzzle_start);
    REQUIRE(path.back() == puzzle_goal);

    int num_steps = 0;

    // Make a working copy of the path that we can take apart
    auto working_path = path;
    auto path_start = working_path.front();
    working_path.pop_front();
    auto state1 = path_start;

    while (!working_path.empty()) {
      auto state2 = working_path.front();
      working_path.pop_front();
      REQUIRE(state1.isAdjacent(state2));
      // step forward
      num_steps++;
      state1 = state2;
    }

    REQUIRE(num_steps == CORRECT_STEPS);
  }

}

// ========================================================================
// Test: puzzleBFS with a random puzzle
// ========================================================================

TEST_CASE("Testing puzzleBFS on a random puzzle:", "[weight=1][ex3]") {

  const PuzzleState puzzle_goal({1,2,3,4,5,6,7,8,9});
  const int MAX_STEPS = 10;
  const PuzzleState puzzle_start = PuzzleState::randomizePuzzle(puzzle_goal, MAX_STEPS);

  std::list<PuzzleState> path = puzzleBFS(puzzle_start, puzzle_goal);

  SECTION("Should find the shortest solution") {
    REQUIRE(!path.empty());
    REQUIRE(path.front() == puzzle_start);
    REQUIRE(path.back() == puzzle_goal);

    int num_steps = 0;

    // Make a working copy of the path that we can take apart
    auto working_path = path;
    auto path_start = working_path.front();
    working_path.pop_front();
    auto state1 = path_start;

    while (!working_path.empty()) {
      auto state2 = working_path.front();
      working_path.pop_front();
      REQUIRE(state1.isAdjacent(state2));
      // step forward
      num_steps++;
      state1 = state2;
    }

    REQUIRE(num_steps <= MAX_STEPS);
  }

}


