
/**
 * @file main.cpp
 * University of Illinois CS 400, MOOC 3, Week 3: Graph Search
 *
 * @author Eric Huber
**/

#include <iostream>
#include <string>

#include "GraphSearchCommon.h"

// -----------------------------------------------------------------------
// Helper function prototypes
// -----------------------------------------------------------------------
// When global functions are defined later in the compilation unit than they
// are called, then at least these "function prototype" header lines must be
// written before any calls are made. The function body definitions appear
// later in this file.

// Helper function that can either print a warning message or throw an exception,
//  depending on how you modify the function.
void errorReaction(std::string msg);
// Header formatting for neat output in the terminal
std::string makeHeader1(std::string msg);
std::string makeHeader2(std::string msg);

// -----------------------------------------------------------------------
// Informal testing function prototypes
// -----------------------------------------------------------------------
// These are some tests you can run in the main program. See also the unit
// testing code described in the "tests" subfolder.

// Main launcher for informal unit tests. This is defined below.
void informalTests();
// Function prototypes for some informal testing suites. Definitions are below.
void demoRunner();
void removeEdgeTester();
void removePointTester();
void countEdgesTester();
void graphBFS_Tester();
void graphBFS_Tester_NoPath();
void graphBFS_Tester_StartAtGoal();
void puzzleBFS_Tester();
void puzzleBFS_Tester_NoPath();
void puzzleBFS_Tester_StartAtGoal();
void puzzleBFS_Tester_Random();

// -----------------------------------------------------------------------
// Main program definition
// -----------------------------------------------------------------------

// main program: You can do whatever you want to test your code here.
// See also the test suite that you can build with "make test".
int main() {

  // Initialize ("seed") the randomness for subsequent rand() calls.
  // (See also the comment about randomization libraries in GraphSearchCommon.h)
  srand(time(0));

  // Run the informal tests
  informalTests();

  return 0;
}

// -----------------------------------------------------------------------
// Informal test launchers
// -----------------------------------------------------------------------

// These tests aren't very exhaustive of the student exercise code.
// They are more of a sanity check on the LinkedList class itself.
void informalTests() {

  // Test runner options -------------------------------------------------

  // You can toggle these from "true" to "false" here if you would like to
  // disable some tests. This can decrease the amount of terminal output
  // you see when you run ./main, so you can focus on a specific part.

  // The first-run welcome screen is meant to alert people that these
  // options exist.
  constexpr bool showWelcome = true;
  // You can view some demonstrations of the GridGraph class and read some
  // explanations. (See also the actual code in this file and the instructions PDF.)
  constexpr bool showDemonstrations = false;
  // You should set these to true if you want to run all the informal tests:
  // (The output will be very verbose.)
  constexpr bool doEx1Tests = false;
  constexpr bool doEx2Tests = false;
  constexpr bool doEx3Tests = true;
  // Some of the tests may take a few seconds to process. If that is annoying,
  // you can temporarily disable them by setting this to false:
  constexpr bool doEx3SlowTests = true; // requires doEx3Tests == true

  // Verbosity options --------------------------------------------------

  // If the verbose diagram plots in the terminal cause problems with screen
  // readers, then these can be changed to false to restrict output to text
  // format only. (Note that these are static class members, so they're like
  // global variables that belong to the class type.)
  GridGraph::allowPlotting = true;
  PuzzleState::allowPlotting = true;
  // When GridGraph plotting is not allowed, it instead try to describe the
  // contents of the large graphs verbosely. If that is also not desired,
  // then this can be set to false:
  GridGraph::allowVerboseTextDescription = true;

  // ---------------------------------------------------------------------

  // Welcome Message (first-run introduction) ----------------------------
  if (showWelcome) {
    std::cout << makeHeader1("Welcome") << std::endl << std::endl;
    std::cout
      << "Make sure to inspect informalTests() in main.cpp to turn on the available" << std::endl
      << "informal tests. The terminal output for each test is lengthy, so you may" << std::endl
      << "want to only activate one test at a time." << std::endl
      << std::endl
      << "If the diagram plots in the terminal cause problems with screen readers," << std::endl
      << "then please adjust the verbosity settings in the informalTests function." << std::endl
      << std::endl
      << "Remember that the informal tests are to help you debug with detailed text" << std::endl
      << "output. They are not the same tests used to grade your code. You can also" << std::endl
      << "run unit tests on your code that are more similar to our autograder." << std::endl
      << "Those are defined in the file(s) in the tests/ subdirectory. Since those" << std::endl
      << "other unit tests are separate from the main program, you must compile and" << std::endl
      << "run them separately by using this command in the terminal:" << std::endl
      << std::endl
      << "  make clean && make test && ./test" << std::endl
      << std::endl;
  }
  else {
    std::cout << makeHeader2("Skipping welcome message.") << std::endl << std::endl;
  }

  // Demonstrations -------------------------------------------------------
  if (showDemonstrations) {
    std::cout << makeHeader1("Demonstrations") << std::endl << std::endl;
    demoRunner();
  }
  else {
    std::cout << makeHeader2("Skipping demonstrations.") << std::endl << std::endl;
    std::cout << "  (Please do try the demonstrations and look at the instructions PDF.)" << std::endl << std::endl;
  }

  // Exercise 1 informal tests --------------------------------------------
  if (doEx1Tests) {
    std::cout << makeHeader1("Exercise 1 informal tests") << std::endl << std::endl;
    countEdgesTester();
    removeEdgeTester();
    removePointTester();
  }
  else {
    std::cout << makeHeader2("Skipping informal tests for Exercise 1.") << std::endl << std::endl;
  }

  // Exercise 2 informal tests --------------------------------------------
  if (doEx2Tests) {
    std::cout << makeHeader1("Exercise 2 informal tests") << std::endl << std::endl;

      // ("edge cases" refers to "special cases", not graph edges.
      //  This has a similar meaning to the "base cases" of recursion.)

      // graphBFS edge cases: --------------------
      graphBFS_Tester_NoPath();
      graphBFS_Tester_StartAtGoal();
      // graphBFS general cases: --------------------
      graphBFS_Tester();
  }
  else {
    std::cout << makeHeader2("Skipping informal tests for Exercise 2.") << std::endl << std::endl;
  }

  // Exercise 3 informal tests --------------------------------------------
  if (doEx3Tests) {
    std::cout << makeHeader1("Exercise 3 informal tests") << std::endl << std::endl;

    std::cout << "Now running puzzleBFS tests. Some tests may be disabled for speed; see" << std::endl
      << "informalTests() in main.cpp for details. If any tests take a very long time," << std::endl
      << "you probably have a bug. You can cancel the running process in the terminal" << std::endl
      << "by pressing: Ctrl c" << std::endl << std::endl;

    // The puzzleBFS test cases might take a long time to run, especially if
    // you have a bug in your solution. This is because of the potentially
    // huge number of states to explore in a graph modeling problem.
    // When your solution is correct, the ones we've provided run pretty fast.

    // puzzleBFS edge cases: --------------------
    if (!doEx3SlowTests) std::cout << "(informalTests: doEx3SlowTests==false, so we'll skip some slow tests.)" << std::endl << std::endl;

    if (doEx3SlowTests) puzzleBFS_Tester_NoPath(); // This one is slow even when your code is correct!
    puzzleBFS_Tester_StartAtGoal();

    // puzzleBFS general cases: --------------------
    puzzleBFS_Tester();
    puzzleBFS_Tester_Random();
  }
  else {
    std::cout << makeHeader2("Skipping informal tests for Exercise 3.") << std::endl << std::endl;
  }

  std::cout << "\nInformal tests finished without crashing, but the output may have been incorrect."
    << "\nPlease scroll to the top of the output to read the diagnostics."
    << "\nIf any tests were disabled, you can enable them in the informalTests"
    << "\nfunction in main.cpp." << std::endl;
}

void demoRunner() {

  {
    std::cout << makeHeader2("Using std::unordered_set") << std::endl << std::endl;

    std::cout
      << "In this project, we use std::unordered_set in several places to represent" << std::endl
      << "collections of items having some shared property. A set contains value copies" << std::endl
      << "of objects, and it contains no duplicates; inserting the same item twice has" << std::endl
      << "no effect. In C++, the unordered set type uses hashing internally, so the" << std::endl
      << "key type must support equality comparison and std::hash. Unlike the unordered" << std::endl
      << "map type, the keys for an unordered set do not have associated values. We only" << std::endl
      << "care whether a given key has been inserted or not." << std::endl
      << std::endl
      << "This is an alternative to \"labeling\" vertices with some status, for example." << std::endl
      << "If an algorithm tells you to label vertices as \"visited\", you could instead" << std::endl
      << "make a set for all the visited vertices, and insert copies of vertices to the" << std::endl
      << "visited set when you want to record that they have been visited." << std::endl
      << std::endl
      << "For example, we could make a set to contain unique integers like this:" << std::endl
      << std::endl
      << "  std::unordered_set<int> favoriteNumbers;" << std::endl
      << std::endl;

    std::unordered_set<int> favoriteNumbers;

    std::cout << "Then we can insert some items with insert():" << std::endl << std::endl;

    std::cout << "Inserting 7." << std::endl;
    favoriteNumbers.insert(7);
    std::cout << "size() is now: " << favoriteNumbers.size() << std::endl;
    std::cout << "Inserting 42." << std::endl;
    favoriteNumbers.insert(42);
    std::cout << "size() is now: " << favoriteNumbers.size() << std::endl;
    std::cout << "Inserting 42 again. (This doesn't change anything.)" << std::endl;
    favoriteNumbers.insert(42);
    std::cout << "size() is now: " << favoriteNumbers.size() << std::endl;

    std::cout
      << "At this point, the set contains 7 and 42. (It only has a single copy of 42.)" << std::endl
      << std::endl;

    std::cout << "Iterating over std::unordered_set contents. (These may not be in any order.)" << std::endl;
    for (int x : favoriteNumbers) {
      std::cout << " Set contains: " << x << std::endl;
    }
    std::cout << std::endl;

    std::cout
      << "And then we can use count() to query if an item is in the set or not, and we" << std::endl
      << "can remove items from the set with erase(). Remeber that since a set contains" << std::endl
      << "unique items, we only ever expect a count of 1 or 0." << std::endl
      << std::endl;

    // Side note: Don't erase or insert items directly within a std::cout statement,
    // even if you want to display the returned value from the operation. The order
    // of evaluation for the stream syntax may not be what you expect.
    std::cout << "Counting the key 7 (expecting 1): " << favoriteNumbers.count(7) << std::endl;
    std::cout << "Erasing 7." << std::endl;
    favoriteNumbers.erase(7);
    std::cout << "Counting the key 7 (expecting 0): " << favoriteNumbers.count(7) << std::endl
      << std::endl;
  }

  {
    std::cout << makeHeader2("Undirected edge representation with IntPairPair") << std::endl << std::endl;

    std::cout << "Our GridGraph class doesn't explicitly store edges since it uses an adjacency" << std::endl
      << "list scheme (actually, an adjacency map and set scheme in our implementation)." << std::endl
      << "However, sometimes we may want to examine the undirected edges of the graph" << std::endl
      << "as explcit items. For example, GridGraph::printDetails makes a set of unique" << std::endl
      << "edges in order to display terminal output." << std::endl << std::endl;

    std::cout << "This is where we have to be careful, because IntPairPair is just a pair of" << std::endl
      << "pairs, so it doesn't guarantee the (A,B)==(B,A) equality that we would expect" << std::endl
      << "for an undrected edge representation." << std::endl << std::endl;

    std::cout << "Creating some points using IntPair:" << std::endl;
    IntPair pointA = {2,3};
    std::cout << "pointA: " << pointA << std::endl;
    IntPair pointB = {1,3};
    std::cout << "pointB: " << pointB << std::endl;

    std::cout << std::endl;

    std::cout << "Creating IntPairPair AB from {pointA, pointB} and BA from {pointB, pointA}." << std::endl;
    IntPairPair AB = {pointA, pointB};
    IntPairPair BA = {pointB, pointA};
    std::cout << "Now is (AB == BA)? " << ((AB == BA) ? "true" : "false") << std::endl;

    std::hash<IntPairPair> ippHasher;
    std::cout << "Hashing AB: " << ippHasher(AB) << std::endl;
    std::cout << "Hashing BA: " << ippHasher(BA) << std::endl;

    std::cout << std::endl;

    std::cout << "If you wanted to extend the code further, you could make a derived class" << std::endl
      << "based on IntPairPair that would ensure that edges (A,B) and (B,A) would" << std::endl
      << "automatically be tested equal and identically hashed. For this" << std::endl
      << "assignment, the provided code does that manually in a few places," << std::endl
      << "such as in GridGraph::printDetails." << std::endl << std::endl;
  }

  {
    std::cout << makeHeader2("Various ways to initialize objects") << std::endl << std::endl;

    std::cout << "As you examine the provided code, you may see objects being initialized in" << std::endl
      << "various ways. Remember that in C++11 and newer, there are compiler features" << std::endl
      << "that can infer (detect) the correct STL types automatically, which can help" << std::endl
      << "you write neater code." << std::endl
      << std::endl
      << "Our IntPair type is based on std::pair<int,int>. The lines below all construct" << std::endl
      << "the same IntPair. The last one uses an \"initializer list\" in braces {}, which" << std::endl
      << "can be used wherever the type can be understood correctly by the compiler." << std::endl
      << std::endl
      << "  IntPair point1 = IntPair(1,2);" << std::endl
      << "  IntPair point2 = std::make_pair(1,2);" << std::endl
      << "  auto point3 = IntPair(1,2);" << std::endl
      << "  IntPair point4 = {1,2};" << std::endl
      << std::endl;

    IntPair point1 = IntPair(1,2);
    IntPair point2 = std::make_pair(1,2);
    auto point3 = IntPair(1,2);
    IntPair point4 = {1,2};

    std::cout << "Displaying print output for all four objects:" << std::endl
      << point1 << point2 << point3 << point4 << std::endl
      << std::endl;

    std::cout << "Verifying equality:" << std::endl
      << std::boolalpha
      << "  point1 == point2 : " << (point1 == point2) << std::endl
      << "  point2 == point3 : " << (point2 == point3) << std::endl
      << "  point3 == point4 : " << (point3 == point4) << std::endl
      << std::noboolalpha
      << std::endl;

    std::cout
      << "However, if you're not explicit enough, the compilation may fail." << std::endl
      << "The compiler needs to be able to tell if you mean IntPair, std::vector," << std::endl
      << "or something else, based on the structure of the variable and how you use it." << std::endl
      << "The following may or may not work, depending on the context of your code:" << std::endl
      << std::endl
      << "  auto point5 = {1,2}; // Probably too vague" << std::endl
      << std::endl;
  }

}

void removeEdgeTester() {

  {
    std::cout << makeHeader2("Testing GridGraph::removeEdge") << std::endl << std::endl;

    std::cout << "(Note: This function was probably implemented for you already.)" << std::endl << std::endl;

    std::cout << "        Here's a graph:" << std::endl << std::endl;;
    GridGraph graph1;
    graph1.insertEdge(IntPair(2,2), IntPair(1,2));
    graph1.insertEdge(IntPair(2,2), IntPair(2,1));
    graph1.plot(std::cout);
    std::cout << std::endl;
    graph1.printDetails(std::cout);
    std::cout << std::endl << std::endl;

    std::cout << "        Removing an edge:" << std::endl << std::endl;;
    GridGraph graph2 = graph1;
    graph2.removeEdge(IntPair(2,2), IntPair(2,1));
    graph2.plot(std::cout);
    std::cout << std::endl;
    graph2.printDetails(std::cout);
    std::cout << std::endl << std::endl;

    GridGraph expectedGraph;
    expectedGraph.insertEdge(IntPair(2,2), IntPair(1,2));
    expectedGraph.insertPoint(IntPair(2,1));
    if (graph2 == expectedGraph) {
      std::cout << "Good, the edge was removed correctly." << std::endl << std::endl;
    }
    else {
      throw std::runtime_error("The starter code did not remove the edge correctly!");
    }
  }

}

void removePointTester() {

  std::cout << makeHeader2("Testing your removePoint function") << std::endl << std::endl;

  // Reference graph with only two points, for use later
  GridGraph edgelessGraph;
  edgelessGraph.insertPoint(IntPair(1,2));
  edgelessGraph.insertPoint(IntPair(2,1));

  // Working graph for testing
  GridGraph graph;

  std::cout << "    Here's the first graph again:" << std::endl << std::endl;;
  graph.insertEdge(IntPair(2,2), IntPair(1,2));
  graph.insertEdge(IntPair(2,2), IntPair(2,1));
  graph.plot(std::cout);
  std::cout << std::endl;
  graph.printDetails(std::cout);
  std::cout << std::endl << std::endl;

  std::cout << "        Removing point (2,2):" << std::endl << std::endl;;
  graph.removePoint(IntPair(2,2));
  graph.plot(std::cout);
  std::cout << std::endl;
  graph.printDetails(std::cout);
  std::cout << std::endl << std::endl;

  if (graph.hasPoint(IntPair(2,2))) {
    errorReaction("The point (2,2) is still present.");
  }
  else {
    std::cout << "Good, the point (2,2) was removed." << std::endl << std::endl;
  }

  // looping over key-value pairs in the adjacency map
  for (const auto& kv : graph.adjacencyMap) {
    // the key type is a point (int pair)
    IntPair k = kv.first;
    if (k == IntPair(2,2)) {
      errorReaction("Found (2,2) key in the adjacencyMap");
    }
    else {
      // the value type is a set of neighboring points that are adjacent to this point in the graph
      const GridGraph::NeighborSet& adjacencies = kv.second;
      for (const IntPair& neighbor : adjacencies) {
        if (neighbor == IntPair(2,2)) {
          errorReaction("Found (2,2) listed as an adjacent point somewhere");
        }
      }
    }
  }

  if (graph == edgelessGraph) {
    std::cout << "Good, it looks like the point removal worked correctly." << std::endl << std::endl;
  }
  else {
    errorReaction("The point removal should only remove the point\n and all the adjacency records that refer to it.");
  }

  GridGraph backupGraph = graph;

  std::cout << "Now trying to remove a non-existent point (7,7)..." << std::endl
    << "The graph should be unchanged and the program shouldn't crash." << std::endl << std::endl;
  graph.removePoint(IntPair(7,7));
  graph.plot(std::cout);
  std::cout << std::endl;
  graph.printDetails(std::cout);
  std::cout << std::endl << std::endl;

  if (graph == backupGraph) {
    std::cout << "Good, the graph was unchanged by that operation." << std::endl << std::endl;
  }

}

void countEdgesTester() {

  std::cout << makeHeader2("Testing your countEdges function") << std::endl << std::endl;

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

  std::cout << "Here's an \"I\"-shaped graph:" << std::endl << std::endl;
  graph.plot(std::cout);
  std::cout << std::endl;
  graph.printDetails(std::cout);
  std::cout << std::endl << std::endl;

  int numEdges = graph.countEdges();

  std::cout << "You counted: " << numEdges << " edges" << std::endl << std::endl;

  if (numEdges == 7) {
    std::cout << "Good! You counted the right number of edges." << std::endl << std::endl;
  }
  else {
    errorReaction("This isn't the correct number of edges.");
  }

}

void graphBFS_Tester() {

  std::cout << makeHeader2("Testing graphBFS (when a path does exist)") << std::endl << std::endl;

  std::cout << "A path from start to goal does exist in this graph, and the edges all have\n"
    << "the same length, so your graphBFS function should be able to find and return\n"
    << "the shortest path:" << std::endl << std::endl;

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

  std::cout << graph << std::endl;

  std::cout << "Finding the shortest path from (0,0) to (6,5):" << std::endl << std::endl;
  const auto maze_start = IntPair(0,0);
  const auto maze_goal = IntPair(6,5);
  std::list<IntPair> path = graphBFS(maze_start, maze_goal, graph);
  if (path.empty()) {
    errorReaction("Could not reach goal from start. BFS function may be incomplete or buggy.");
  }
  else {

    bool allGood = true;

    if (path.front() != maze_start) {
      allGood = false;
      errorReaction("First point in reconstructed path was not start.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }
    if (path.back() != maze_goal) {
      allGood = false;
      errorReaction("Last point in reconstructed path was not goal.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }

    // If you have GridGraph::allowVerboseTextDescription set to false,
    // but you'd like to have the path described anyway, set this to true.
    const bool forceDescribePath = false;

    if (!GridGraph::allowVerboseTextDescription && !forceDescribePath) {
      std::cout << "Omitting verbose text description of path. You can change this in main.cpp" << std::endl
        << " in the graphBFS_Tester function definition." << std::endl << std::endl;
    }

    // Display the path found as its own graph
    GridGraph newGraph;
    // Make a working copy of the path that we can take apart
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
      if (!graph.hasEdge(p1,p2)) {
        allGood = false;
        std::cout << "Reported edge: " << p1 << " to " << p2 << std::endl;
        errorReaction("The reported path edge doesn't exist in the original graph");
      }
      else {
        // When plotting is not allowed, we take special care to print the verbose
        // details of the path in path order.
        if (forceDescribePath || (!GridGraph::allowPlotting && GridGraph::allowVerboseTextDescription)) {
          std::cout << "[(" << p1.first << "," << p1.second << ") to (" << p2.first << "," << p2.second << ")] ";
        }
      }
      // step forward
      num_steps++;
      p1 = p2;
    }

    // The default behavior of << when plotting is not allowed would be to print
    // details of the path edges all jumbled out of order.
    if (GridGraph::allowPlotting) std::cout << newGraph << std::endl;
    std::cout << "Steps taken: " << num_steps << std::endl;

    if (num_steps != 19) {
      allGood = false;
      errorReaction("The path returned is not the shortest possible path");
    }

    std::cout << std::endl;

    if (allGood) std::cout << "Good! You found the shortest path to the goal." << std::endl;
    else std::cout << "Some warnings or errors occurred in this test." << std::endl;
  }

  std::cout << std::endl;
}

void graphBFS_Tester_NoPath() {

  std::cout << makeHeader2("Testing graphBFS (when there is no path)") << std::endl << std::endl;

  std::cout << "In this maze there's no way to reach the goal from the start,\n"
    << "so your graphBFS should return an empty list:" << std::endl << std::endl;

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

  std::cout << graph << std::endl;

  std::cout << "Finding the shortest path from (0,0) to (6,5):" << std::endl << std::endl;
  const auto maze_start = IntPair(0,0);
  const auto maze_goal = IntPair(6,5);
  std::list<IntPair> path = graphBFS(maze_start, maze_goal, graph);
  if (path.empty()) {
    std::cout << "Good, an empty path was returned in this case." << std::endl;
  }
  else {
    std::cout << "Error: Your graphBFS returned some path when none should exist.\n"
      << "Here is the path that your graphBFS returned:" << std::endl << std::endl;

    if (path.front() != maze_start) {
      errorReaction("First point in reconstructed path was not start.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }
    if (path.back() != maze_goal) {
      errorReaction("Last point in reconstructed path was not goal.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }

    // Display the path found as its own graph
    GridGraph newGraph;
    // Make a working copy of the path that we can take apart
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
      if (!graph.hasEdge(p1,p2)) {
        std::cout << "Reported edge: " << p1 << " to " << p2 << std::endl;
        errorReaction("The reported path edge doesn't exist in the original graph");
      }
      // step forward
      num_steps++;
      p1 = p2;
    }
    std::cout << newGraph << std::endl;
    std::cout << "Steps taken: " << num_steps << std::endl;

    errorReaction("graphBFS returned some path when no path should be found");
  }

  std::cout << std::endl;
}

void graphBFS_Tester_StartAtGoal() {

  std::cout << makeHeader2("Testing graphBFS (when start==goal)") << std::endl << std::endl;

  std::cout << "Testing the edge case where we start at the goal.\nHere's a simple graph:" << std::endl << std::endl;

  GridGraph graph;

  std::vector<std::vector<int>> edgesToInsert = {{0,0,0,1}, {0,1,0,2}};
  for (auto e : edgesToInsert) {
    auto p1 = IntPair(e[0],e[1]);
    auto p2 = IntPair(e[2],e[3]);
    graph.insertEdge(p1,p2);
  }

  std::cout << graph << std::endl;

  std::cout << "Finding the shortest path from (0,0) to (0,0):" << std::endl << std::endl;
  const auto maze_start = IntPair(0,0);
  const auto maze_goal = maze_start;
  std::list<IntPair> path = graphBFS(maze_start, maze_goal, graph);
  if (path.empty()) {
    errorReaction("When starting at the goal, graphBFS should return a path\ncontaining only that one point, but not an empty path.");
  }
  else {
    std::cout << "Here is the path that your graphBFS returned:" << std::endl
      << "(It should be a single point (0,0), in this case.)" << std::endl << std::endl;

    bool allGood = true;

    if (path.front() != maze_start) {
      allGood = false;
      errorReaction("First point in reconstructed path was not start.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }
    if (path.back() != maze_goal) {
      allGood = false;
      errorReaction("Last point in reconstructed path was not goal.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }

    // Display the path found as its own graph
    GridGraph newGraph;
    // Make a working copy of the path that we can take apart
    auto working_path = path;
    // Begin from the start of the path and remove the key from the list
    auto path_start = working_path.front();
    working_path.pop_front();
    // Insert path_start as an isolated point
    newGraph.insertPoint(path_start);
    // Walk through subsequent pairs and add edges
    int num_steps = 0;
    auto p1 = path_start;
    while (!working_path.empty()) {
      auto p2 = working_path.front();
      working_path.pop_front();
      newGraph.insertEdge(p1,p2);
      if (!graph.hasEdge(p1,p2)) {
        allGood = false;
        std::cout << "Reported edge: " << p1 << " to " << p2 << std::endl;
        errorReaction("The reported path edge doesn't exist in the original graph");
      }
      // step forward
      num_steps++;
      p1 = p2;
    }
    std::cout << newGraph << std::endl;
    std::cout << "Steps taken: " << num_steps << std::endl;

    if (num_steps != 0) {
      allGood = false;
      errorReaction("Should have taken 0 steps");
    }

    std::cout << std::endl;

    if (allGood) std::cout << "Good! When start==goal, the shortest path is to stay at the start." << std::endl;
    else std::cout << "Some warnings or errors occurred in this test." << std::endl;
  }

  std::cout << std::endl;
}

void puzzleBFS_Tester() {

  std::cout << makeHeader2("Testing puzzleBFS (simple puzzle instance)") << std::endl << std::endl;

  std:: cout << "This puzzle can be solved from the given starting position, so puzzleBFS" << std::endl
    << "should be able to solve it and find the shortest solution too." << std::endl << std::endl;

  const PuzzleState puzzle_start({1,2,3,4,6,9,7,5,8});
  const PuzzleState puzzle_goal({1,2,3,4,5,6,7,8,9});

  // Print a compact string version of the state as well as the plot diagram (if allowed):
  std::cout << puzzle_start << std::endl;

  std::cout << "Finding the shortest solution sequence:" << std::endl << std::endl;
  std::list<PuzzleState> path = puzzleBFS(puzzle_start, puzzle_goal);
  if (path.empty()) {
    errorReaction("Could not reach goal from start. BFS function may be incomplete or buggy.");
  }
  else {

    std::cout << "Returned some path (list of sequential states). Validating..." << std::endl << std::endl;

    // We'll check that the solution sequence is valid and display each step

    bool allGood = true;

    if (path.front() != puzzle_start) {
      allGood = false;
      errorReaction("First state in reconstructed sequence was not start.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }
    if (path.back() != puzzle_goal) {
      allGood = false;
      errorReaction("Last state in reconstructed sequence was not goal.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }

    int num_steps = 0;

    // Make a working copy of the path that we can take apart
    auto working_path = path;
    auto path_start = working_path.front();
    working_path.pop_front();
    auto state1 = path_start;

    std::cout << state1 << std::endl;

    while (!working_path.empty()) {
      auto state2 = working_path.front();
      working_path.pop_front();
      if (!state1.isAdjacent(state2)) {
        std::cout << "Illegal move in sequence from:" << std::endl;
        std::cout << state1 << std::endl;
        std::cout << " to:" << std::endl;
        std::cout << state2 << std::endl;
        allGood = false;
        errorReaction("The reported sequence contains an illegal move");
      }
      // step forward
      num_steps++;
      state1 = state2;

      std::cout << state1 << std::endl;
    }

    std::cout << "Steps taken: " << num_steps << std::endl;

    if (num_steps > 3) {
      allGood = false;
      errorReaction("Took more steps than should have been necessary.");
    }

    std::cout << std::endl;

    if (allGood) std::cout << "Good! You found the shortest solution sequence." << std::endl;
    else std::cout << "Some warnings or errors occurred in this test." << std::endl;
  }

  std::cout << std::endl;
}

void puzzleBFS_Tester_NoPath() {

  std::cout << makeHeader2("Testing puzzleBFS (when there is no solution)") << std::endl << std::endl;

  std::cout << "(Please allow at least 10 seconds for this test to complete. If this is" << std::endl
    << " inconvenient, you can disable this test in the informalTests function" << std::endl
    << " in main.cpp.)" << std::endl << std::endl;

  std::cout << "This puzzle can't be solved from the given starting state," << std::endl
    << "so your puzzleBFS should return an empty list:" << std::endl << std::endl;

  const PuzzleState puzzle_start({1,3,2,4,5,6,7,8,9});
  const PuzzleState puzzle_goal({1,2,3,4,5,6,7,8,9});

  std::cout << puzzle_start << std::endl;

  std::cout << "Finding the shortest solution sequence:" << std::endl << std::endl;
  std::list<PuzzleState> path = puzzleBFS(puzzle_start, puzzle_goal);
  if (path.empty()) {
    std::cout << "Good, an empty path was returned in this case." << std::endl;
  }
  else {
    std::cout << "Error: Your puzzleBFS returned some path when none should exist.\n"
      << "Here is the path that your puzzleBFS returned:" << std::endl << std::endl;

    // We'll check that the solution sequence is valid and display each step

    if (path.front() != puzzle_start) {
      errorReaction("First state in reconstructed sequence was not start.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }
    if (path.back() != puzzle_goal) {
      errorReaction("Last state in reconstructed sequence was not goal.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }

    int num_steps = 0;

    // Make a working copy of the path that we can take apart
    auto working_path = path;
    auto path_start = working_path.front();
    working_path.pop_front();
    auto state1 = path_start;

    std::cout << state1 << std::endl;

    while (!working_path.empty()) {
      auto state2 = working_path.front();
      working_path.pop_front();
      if (!state1.isAdjacent(state2)) {
        std::cout << "Illegal move in sequence from:" << std::endl;
        std::cout << state1 << std::endl;
        std::cout << " to:" << std::endl;
        std::cout << state2 << std::endl;
        errorReaction("The reported sequence contains an illegal move");
      }
      // step forward
      num_steps++;
      state1 = state2;

      std::cout << state1 << std::endl;
    }

    std::cout << "Steps taken: " << num_steps << std::endl;

    std::cout << std::endl;

    errorReaction("puzzleBFS returned some path when no path should be found");
  }

  std::cout << std::endl;
}

void puzzleBFS_Tester_StartAtGoal() {

  std::cout << makeHeader2("Testing puzzleBFS (when start==goal)") << std::endl << std::endl;

  std::cout << "Testing the edge case where we start at the goal." << std::endl << std::endl;

  const PuzzleState puzzle_start({1,2,3,4,5,6,7,8,9});
  const PuzzleState puzzle_goal = puzzle_start;

  std::cout << puzzle_start << std::endl;

  std::cout << "Finding the shortest solution sequence:" << std::endl << std::endl;
  std::list<PuzzleState> path = puzzleBFS(puzzle_start, puzzle_goal);
  if (path.empty()) {
    errorReaction("Returned an empty path, but should have returned a path containing only start.");
  }
  else {
    std::cout << "Returned some path (list of sequential states). Validating..." << std::endl << std::endl;

    // We'll check that the solution sequence is valid and display each step

    bool allGood = true;

    if (path.front() != puzzle_start) {
      allGood = false;
      errorReaction("First state in reconstructed sequence was not start.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }
    if (path.back() != puzzle_goal) {
      allGood = false;
      errorReaction("Last state in reconstructed sequence was not goal.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }

    int num_steps = 0;

    // Make a working copy of the path that we can take apart
    auto working_path = path;
    auto path_start = working_path.front();
    working_path.pop_front();
    auto state1 = path_start;

    std::cout << state1 << std::endl;

    while (!working_path.empty()) {
      auto state2 = working_path.front();
      working_path.pop_front();
      if (!state1.isAdjacent(state2)) {
        std::cout << "Illegal move in sequence from:" << std::endl;
        std::cout << state1 << std::endl;
        std::cout << " to:" << std::endl;
        std::cout << state2 << std::endl;
        allGood = false;
        errorReaction("The reported sequence contains an illegal move");
      }
      // step forward
      num_steps++;
      state1 = state2;

      std::cout << state1 << std::endl;
    }

    std::cout << "Steps taken: " << num_steps << std::endl;

    if (num_steps > 0) {
      allGood = false;
      errorReaction("Took >0 steps when 0 steps should have been taken.");
    }

    std::cout << std::endl;

    if (allGood) std::cout << "Good! When start==goal, the shortest path is to stay at the start." << std::endl;
    else std::cout << "Some warnings or errors occurred in this test." << std::endl;
  }

  std::cout << std::endl;
}

void puzzleBFS_Tester_Random() {

  std::cout << makeHeader2("Testing puzzleBFS with a random, solvable puzzle") << std::endl << std::endl;

  std:: cout << "This puzzle can be solved from the given starting position, so puzzleBFS" << std::endl
    << "should be able to solve it and find the shortest solution too." << std::endl << std::endl;

  const PuzzleState puzzle_goal({1,2,3,4,5,6,7,8,9});
  const int max_steps = 10;
  const PuzzleState puzzle_start = PuzzleState::randomizePuzzle(puzzle_goal, max_steps);

  std::cout << puzzle_start << std::endl;

  std::cout << "Finding the shortest solution sequence:" << std::endl << std::endl;
  std::list<PuzzleState> path = puzzleBFS(puzzle_start, puzzle_goal);
  if (path.empty()) {
    errorReaction("Could not reach goal from start. BFS function may be incomplete or buggy.");
  }
  else {

    std::cout << "Returned some path (list of sequential states). Validating..." << std::endl << std::endl;

    // We'll check that the solution sequence is valid and display each step

    bool allGood = true;

    if (path.front() != puzzle_start) {
      allGood = false;
      errorReaction("First state in reconstructed sequence was not start.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }
    if (path.back() != puzzle_goal) {
      allGood = false;
      errorReaction("Last state in reconstructed sequence was not goal.\n Make sure you've set predecessors correctly with pred[].\n (And make sure you don't have it backwards.)");
    }

    int num_steps = 0;

    // Make a working copy of the path that we can take apart
    auto working_path = path;
    auto path_start = working_path.front();
    working_path.pop_front();
    auto state1 = path_start;

    std::cout << state1 << std::endl;

    while (!working_path.empty()) {
      auto state2 = working_path.front();
      working_path.pop_front();
      if (!state1.isAdjacent(state2)) {
        std::cout << "Illegal move in sequence from:" << std::endl;
        std::cout << state1 << std::endl;
        std::cout << " to:" << std::endl;
        std::cout << state2 << std::endl;
        allGood = false;
        errorReaction("The reported sequence contains an illegal move");
      }
      // step forward
      num_steps++;
      state1 = state2;

      std::cout << state1 << std::endl;
    }

    std::cout << "Steps taken: " << num_steps << std::endl;

    if (num_steps > max_steps) {
      allGood = false;
      errorReaction("Took more steps than should have been necessary.");
    }

    std::cout << std::endl;

    if (allGood) std::cout << "Good! You found the shortest solution sequence." << std::endl;
    else std::cout << "Some warnings or errors occurred in this test." << std::endl;
  }

  std::cout << std::endl;
}

// ------------------------------------------------------------------------
// Helper function definitions
// ------------------------------------------------------------------------

// Choose what to do if an informal test has a big problem.
void errorReaction(std::string msg) {
  // If you want to display a warning and keep going:
  std::cout << std::endl << "WARNING: " << msg << std::endl << std::endl;;

  // If you want to throw an exception and stop the program immediately:
  // throw std::runtime_error(msg);
}

// Section headers are printed with fancy formatting so they stand out.
// These are meant to make reading the output logs easier when scrolling.
// Header level 1 is more prominent than header level 2.

// makeHeader1 generates output that looks like this:
// =========================================================================
//                               Header 1
// =========================================================================
std::string makeHeader1(std::string msg) {
  // Should be less than the actual width (80 expected)
  int terminal_width = 75;
  // The horizontal rule is an entire terminal row filled with the same symbol
  std::string rule;
  for (int i=0; i<terminal_width; i++) {
    rule += "=";
  }
  int space_available = terminal_width - msg.size();
  if (space_available < 0) space_available = 0;
  // Ensure an even number:
  if (space_available % 2) space_available++;
  int padding_size = (space_available/2)-1;
  std::string padding_string;
  for (int i=0; i<padding_size; i++) {
    // Center text with spaces on either side
    padding_string += " ";
  }
  std::string output;
  output += rule + "\n";
  output += padding_string + " " + msg + " " + padding_string + "\n";
  output += rule;
  return output;
}

// makeHeader2 generates output that looks like this:
// ============================== Header 2 =================================
std::string makeHeader2(std::string msg) {
  // Should be less than the actual width (80 expected)
  int terminal_width = 75;
  int space_available = terminal_width - msg.size();
  if (space_available < 0) space_available = 0;
  // Ensure an even number:
  if (space_available % 2) space_available++;
  int padding_size = (space_available/2)-1;
  std::string padding_string;
  for (int i=0; i<padding_size; i++) {
    // Center text with "=" symbol on either side
    padding_string += "=";
  }
  std::string output;
  output += padding_string + " " + msg + " " + padding_string;
  return output;
}


