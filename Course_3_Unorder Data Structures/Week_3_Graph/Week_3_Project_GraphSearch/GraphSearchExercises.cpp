
/**
 * @file GraphSearchExercises.cpp
 * University of Illinois CS 400, MOOC 3, Week 3: Graph Search
 * Spring 2019
 *                        STUDENT STARTER FILE
 *
 * @author Eric Huber - University of Illinois staff
 *
**/

// Before beginning these exercises, you should read the instructions PDF,
// and look through the other code files in this directory for examples and
// hints. You only need to edit code in this file. There are comments here
// hinting at what you need to do, and "TODO" is written in various places
// where you need to edit some code in particular.

#include "GraphSearchCommon.h"

// =========================================================================
// EXERCISE 1: Adjacency List Utilities
//
// This exercise is based on the Week 3 lecture material.
//
// Our GridGraph class implements an "adjacency list" graph data structure,
// although it actually makes use of std::unordered_map and std::unordered_set
// to do this. You can read about GridGraph in the instructions PDF and by
// examining GridGraph.h. There are also various examples of GridGraph usage
// in the informal tests defined in main.cpp.
//
// Most of the GridGraph class is already implemented for you. For this
// exercise, you need to finish implementing two functions below:
//
// GridGraph::countEdges
// GridGraph::removePoint
//
// =========================================================================

// GridGraph::countEdges:
// This function should return the number of unique edges in the GridGraph.
// Remember that GridGraph doesn't explicitly store edge objects, but
// instead it stores point adjacency information. Since the graph edges are
// undirected, for any two vertices A and B, the edges (A,B) and (B,A) are the
// same edge. This means if we add up the sizes of the GridGraph::NeighborSet
// sets mapped in adjacencyMap, we'll be double-counting the number of edges.
// We can still use that to get the answer, or instead, the implementation of
// GridGraph::printDetails in GridGraph.h shows another method, by constructing
// a set of the unique edges only.
int GridGraph::countEdges() const {
  
  
  // =======================================================================
  // TODO: Your code here!
  // =======================================================================
  
  
  int numEdges = 0;

  int count_of_undirected_edge = 0;
  
  // visit each 2D Point
  for( auto adjMap_iter = adjacencyMap.begin() ; adjMap_iter != adjacencyMap.end() ; adjMap_iter++ )
  {
    auto edge_pair_container = adjMap_iter->second;
    
    // visis each undireced edge of one 2D Point
    for( auto edge_iter = edge_pair_container.begin()  ; edge_iter != edge_pair_container.end() ; edge_iter++ )
    {
      // accumulate undirected edge counter
      count_of_undirected_edge++;
      
    }
    //end of edge_iter loop
  }
  //end of for adjMap_iter loop

  // for e = (u, v)
  // each undirected edge e will be counted twice, one on u, the other on v.
  numEdges = count_of_undirected_edge/2;


  return numEdges;
}
//end of function int GridGraph::countEdges() const

// GridGraph::removePoint:
// This function takes a point (an IntPair) as input, and it should remove
// all references to the point from the data structure. That means it should
// remove the point's key from the adjacency map, and it should also remove
// other points' references to it in the map, if those other points had been
// connected to it by implicit edges. It shouldn't change anything else about
// the graph.
void GridGraph::removePoint(const IntPair& p1) {

  // If the point p1 is not in the GridGraph, then do nothing and return.
  // (We use GridGraph::hasPoint for convenience. GridGraph already has
  //  various functions that may be useful. It's okay for class member
  //  functions to delegate tasks to one another.)
  if (!hasPoint(p1)) return;

  // All of p1's original neighboring points (that p1 was connected to with
  // edges, implicitly) have their own NeighborSet entries in adjacencyMap,
  // under their own point keys. You need to make sure that none of those
  // original neighbors refer to p1 any more.
  //
  // To help you, we've made a value copy of the original set of adjacencies
  // associated with this point. Note that the function GridGraph::removeEdge
  // alters the adjacencyMap entries for both of its input arguments. In
  // general, it can be dangerous business to add or remove items on an STL
  // structure while iterating over it, because this is prone to programming
  // mistakes. (When you tackle more advanced C++ projects in the future, you
  // can think about how to optimize your code so no unnecessary copies are
  // ever made, but the small speed benefits of that might not be worth the
  // risk of making mistakes! Until then, it's okay to make working copies
  // of small pieces of data like this.)

  const GridGraph::NeighborSet originalNeighbors = adjacencyMap.at(p1);
  


  // =======================================================================
  // TODO: Your code here!
  // =======================================================================

  // visit each neighbor 2D point's NeighborSet, remove edge e1 = ( neightbor, p1 ) and e2 = ( p1, neighbor )
  for( auto neighbor_iter = originalNeighbors.begin() ; neighbor_iter != originalNeighbors.end() ; neighbor_iter++ )
  {
    //std::cout << "Remove edge between " << *neighbor_iter << " and " << p1 << std::endl;
    // remove edge 
    removeEdge( *neighbor_iter, p1);
  }
  //end of for loop


  // Finally, for the one point we are removing, erase the point key itself
  // from adjacencyMap directly. (There is no other GridGraph helper function
  // for this, because that's what we're implementing right now! We need to
  // use adjacencyMap's own erase function directly to erase the key.)

  // =======================================================================
  // TODO: Your code here!
  // =======================================================================
  
  // remove given point in GridGraph::adjacencyMap
  adjacencyMap.erase( p1 );
  
  
  return;
}
//end of function void GridGraph::removePoint(const IntPair& p1)

// =========================================================================
// EXERCISE 2: graphBFS
//
// This exercise is based on the Week 4 lecture material.
//
// The graphBFS function below largely implements the "breadth-first search"
// algorithm for the GridGraph class. You'll be asked to edit a few parts of
// the function to complete it. Those parts are marked with "TODO".
// (Please read the instructions PDF in case these hints are expanded upon
//  there.)
//
// Differences from the version of BFS discussed in lecture:
//
// - This implementation of BFS is geared toward finding the shortest path
//   from a start point to a goal point. So, it only explores within a single
//   connected component, and it may report that the goal is unreachable.
//   As soon as the goal point is found, our algorithm stops searching.
//
// - Our implementation uses sets of points to implicitly "label" points with
//   some status such as "visited", instead of assigning a label property to
//   a point itself. This lets us associate more than one kind of status with
//   any given point at the same time, by putting the point in several sets.
//   It's convenient to do this with STL containers like unordered set, since
//   we don't have to add some kind of status member variable to our own
//   classes like IntPair this way. It also means we don't have to initialize
//   a status for every vertex. In some graph search problems, such as later
//   in Exercise 3, the number of vertices is extremely large, so we'd rather
//   not have to initialize them all!
//
// - We use a map, "pred", to record the predecessor vertex of any newly-
//   discovered vertex during the search. This implicitly records what the
//   discovery edges are, as described in lecture. We can use that to rebuild
//   the path from start to goal at the end.
//
// - We do not assign status directly to edges. However, the vertex
//   predecessor information is basically recording the "discovery" edges.
//
// - We use a map, "dist", to record information about the shortest-path
//   distance to any given node that has been discovered so far. This is
//   similar to what Dijkstra's algorithm does, although with BFS graph search
//   when the edge lengths are all equal to 1, we know that as soon as we
//   discover a node, we have found the shortest path to it. We can still use
//   this to detect if we've taken more steps than expected and quit.
//
// - Redundantly, we have created a "dequeued set" that we use to help you
//   check for mistakes that could cause an infinite loop. This isn't normally
//   part of the BFS algorithm itself. In a way, this is mirroring what the
//   "visited set" is already supposed to accomplish.
//
// =========================================================================

// graphBFS:
// We use our GridGraph class where vertices are points (IntPair objects).
// The input consists of a start point, a goal point, and a GridGraph containing
// these points and the implicit edge information.
// We use BFS (breadth-first search) to find the shortest path from start to
// goal, if there exists any path at all. Then we return the path as a list of
// points. If there is no path, or if we take too many steps without success,
// then we return an empty list.
std::list<IntPair> graphBFS(const IntPair& start, const IntPair& goal, const GridGraph& graph) {

  // Intialization details -------------------------------------------------

  // maxDist failsafe:
  //
  // We'll hard-code a maximum distance for any shortest path through the graph
  // that we will consider before giving up. This isn't strictly necessary for
  // BFS to be correct; when a path can be found from start to goal, we'll find
  // the shortest such path and we won't always need to visit all of the vertices
  // before that happens anyway. But suppose the goal is unreachable from start;
  // then if the graph has a very large number of vertices, an unrestricted BFS
  // algorithm would have to explore all the reachable vertices before giving up
  // anyway, which could require a lot of time and memory if the graph is huge.
  // To avoid this issue, we can hard-code the maximum number of steps we will
  // allow before giving up. Of course, we must ensure that it's a reasonable
  // number. We might be able to estimate the maximum number of steps based on
  // the number of edges in the graph, for example. In the case of puzzleBFS
  // later in this assignment, there is a proven upper bound on the number of
  // steps needed.
  //
  // This is not very important for the small GridGraph examples we have,
  // but with a graph modeling problem like the puzzleBFS problem in this
  // assignment, the number of states implied in the graph is extremely large,
  // an exhaustive search could take a long time just to fail.
  constexpr int maxDist = 100;

  // Queue of graph vertices to explore next. This is an ordinary FIFO queue
  // (first in, first out), not a priority queue. When we say "explore" a vertex
  // in this assignment, we mean to dequeue it and loop over its adjacent vertices.
  std::queue<IntPair> exploreQ;

  // Map that associates a given point with its predecessor (the neighbor that discovered it first)
  // This will allow us to reconstruct the path once the goal is found.
  std::unordered_map<IntPair, IntPair> pred;
  // Example usage: "pred[B] = A" records A as the predecessor of B, meaning that A discovered B.

  // Map that associates a given point with its shortest-path distance from the starting point.
  // This lets us see if we've taken too many steps (see the above comment about maxDist).
  // The simplest implementations of BFS don't need a distance record like this,
  // but it can be useful information, and other graph search algorithms like Dijkstra's
  // algorithm do need to use such a record.
  // Remember: with std::unordered_map, the first time we look up a key with [], it will be
  //  inserted if it doesn't exist yet. Also, its initial int value will be 0 by default.
  std::unordered_map<IntPair, int> dist;

  // Set of graph vertices that have already been visited. (In this case, by
  // "visited", we mean the same thing as a vertex that has been "discovered"
  // or "reached" -- that is, it has been added to the exploration queue,
  // but not necessarily removed from the queue yet.) This lets us avoid
  // re-enqueueing graph vertices that we've already enqueued for exploration.
  // (For further optimization, we could just use pred to query whether a vertex
  //  has been discovered previously, but using this separate set is more explicit
  //  for the sake of explaining the concepts here.)
  std::unordered_set<IntPair> visitedSet;

  // This "dequeued" set isn't strictly part of the traditional BFS algorithm,
  // but we'll use it for error checking, to make sure that no vertex is ever
  // removed from the queue more than once. This is technically redundant,
  // because we also have the visitedSet to use.
  std::unordered_set<IntPair> dequeuedSet;

  // Check for an erroneous problem specification, in case we made a simple mistake.
  // (Note that for puzzleBFS, the PuzzleState constructors already check for obviously
  //  invalid configurations, so you don't need to do this.)
  if (!graph.hasPoint(start)) throw std::runtime_error("Starting point doesn't exist in graph");
  if (!graph.hasPoint(goal)) throw std::runtime_error("Goal point doesn't exist in graph");

  // Initialize the predecessor of the start vertex as itself. You might
  // wonder why we don't just leave pred[start] non-existent, because that's
  // another way we could check the beginning of the path as we retrace the
  // path steps later. It's because initializing pred[start] sanely here
  // lets us make sure that the start vertex can be treated like any other
  // vertex, which can help prevent subtle bugs in our implementation.
  pred[start] = start;

  // Initialize that the shortest-path distance from start to itself is 0.
  // (This isn't strictly necessary because of how std::unordered_map works.
  //  It would default-initialize dist[start] to 0 later when we referred to it.
  //  However, this is more clear.)
  dist[start] = 0;

  // We begin at the start point, so mark it discovered by putting a copy of it
  // in the visited set.
  visitedSet.insert(start);

  // We will begin searching from the start point. Push a copy of it into the exploration queue.
  exploreQ.push(start);

  // Below, a "flag" just means a variable for controlling a loop condition.

  // foundGoal is a flag for controlling the loop below. It's initialized as false,
  // unless we start at the goal, in which case it's initialized as true.
  bool foundGoal = (start == goal);

  // tooManySteps is a flag that will help us break out of the loop if we detect
  // that we've already taken too many steps and probably need to give up searching.
  bool tooManySteps = false;

  // The main search loop --------------------------------------------------

  // While the exploration queue isn't empty yet, there are still discovered points to explore.
  // Also, we would want to stop looping if foundGoal or tooManySteps became true.
  while (!exploreQ.empty() && !foundGoal && !tooManySteps) {

    // Get a copy of the next point to explore from the front of the queue.
    // This becomes the "current" point.
    auto curPoint = exploreQ.front();
    // Pop to remove the point from the queue.
    exploreQ.pop();

    // Error prevention: Check whether this vertex has ever been dequeued in the past.
    // (Checking for vertices that are dequeued more than once isn't strictly considered
    //  part of the BFS algorithm, because in the loop below, you are meant to check for
    //  previously-visited vertices while considering the adjacent vertices. We do this
    //  here for additional sanity checking, in case you have made any mistakes that would
    //  cause an infinite loop.)
    bool curPointWasPreviouslyDequeued = dequeuedSet.count(curPoint);
    if (curPointWasPreviouslyDequeued) {
      std::cout << "graphBFS ERROR: Dequeued a vertex that had already been dequeued before." << std::endl
        << " If you're using visitedSet correctly, then no vertex should ever be added" << std::endl
        << " to the explore qeueue more than once. [Returning an empty path now.]" << std::endl << std::endl;
      return std::list<IntPair>();
    }
    else {
      // We'll record that this vertex has been dequeued by adding a copy of it to the set.
      dequeuedSet.insert(curPoint);
    }

    // =====================================================================
    // TODO: Your code here!
    // We'll need to loop over the neighbors that are the points adjacent to curPoint.
    // Get a copy of the set of neighbors we're going to loop over.
    GridGraph::NeighborSet neighbors; // Change this...
    
    // Get the neighbor 2D points of curPoint
    neighbors = graph.adjacencyMap.at( curPoint );
    
    
    // =====================================================================

    for (auto neighbor : neighbors) {

      // ==================================================================
      // TODO: Your code here!
      // Check whether the neighbor has already been visited.
      bool neighborWasAlreadyVisited = false; // Change this...
      
      if( 1 == visitedSet.count( neighbor ) )
      {
        neighborWasAlreadyVisited = true;
      }
      
      // ==================================================================

      // If this adjacent vertex has NOT been visited before, we will visit it now.
      // If it HAS been visited before, we do nothing and continue to loop.
      // This way, we avoid enqueueing the same vertex more than once.
      if (!neighborWasAlreadyVisited) {

        // ================================================================
        // TODO: Your code here!

        // Record that the curPoint is the predecessor of the neighbor point,
        // since curPoint has just led to the discovery of this neighbor for
        // the first time.
        // ...
        pred[ neighbor ] = curPoint;

        // Add neighbor to the visited set.
        // ...
        visitedSet.insert( neighbor );

        // Push neighbor into the exploration queue.
        // ...
        exploreQ.push( neighbor );

        // ================================================================

        // Check if we've taken too many steps so far.
        // The shortest-path distance to this neighbor is the shortest-path distance
        // to curPoint, plus one. (We know that dist[curPoint] has already been
        // initialized because we previously reached curPoint from another point,
        // and it was assigned a value at that time, or else curPoint is the
        // start point that we explicitly initialized at the beginning.)
        dist[neighbor] = dist[curPoint]+1;
        if (dist[neighbor] > maxDist) {
          // If the shortest path to this neighbor is a further distance than we
          // are allowed to explore, then flag that we have taken too many steps,
          // and break out of the nearest loop (the for loop). After that, the
          // tooManySteps flag will cause the while loop to stop also.

          // You may ask: How do we know to give up here? What if there are other
          // paths we can still explore through the exploration queue, that are shorter?
          // We know there are not, because in BFS, we identify all the reachable nodes
          // of distance 1, and only after that, all the reachable nodes with distance 2, etc.
          // So right now, if we need "too many" steps to reach this discovered neighbor,
          // we'll also need "too many" steps to reach any other node later.
          // This is the same reasoning that allows us to conclude we have found the
          // shortest path, when we finally do reach the goal.
          tooManySteps = true;
          break;
        }

        // If we haven't taken too many steps yet, and if the neighbor is the goal,
        // then flag that we have found the goal, and break out of the "for" loop.
        // The foundGoal flag will then cause the "while" loop to stop, also.
        if (neighbor == goal) {
          foundGoal = true;
          break;
        }

      } // end of handling the just-discovered neighbor

    } // end of for loop
  } // end of while loop

  // Now that the looping is over, we can evaluate what the results were.

  // If we took too many steps, we issue an error message and return an empty list.
  if (tooManySteps) {
    std::cout << "graphBFS warning: Could not reach goal within the maximum allowed steps.\n (This may be expected if no path exists.)" << std::endl << std::endl;
    return std::list<IntPair>();
  }

  // If we never found the goal even after exploring the entire reachable graph,
  // then issue an error message and return an empty list.
  if (!foundGoal) {
    std::cout << "graphBFS warning: Could not reach goal. (This may be expected\n if no path exists.)" << std::endl << std::endl;
    return std::list<IntPair>();
  }

  // Otherwise, we must have found a path from start to goal, and it is as short as possible.
  // (If there exist multiple shortest paths from start to goal that all have the same length,
  //  we have found ONE of these shortest paths.)

  // Make a new, empty list of IntPairs in the stack memory at function scope.
  // This will represent the path from start to goal. We'll return it by value at the end.
  std::list<IntPair> path;
  // We will walk back from the goal to the start using the predecessor records.
  auto cur = goal;
  // Push the currently-walked vertex onto the FRONT of the list, so that as we walk back
  // from goal to start, the created list will be in order from start to goal.
  path.push_front(cur);
  // Make sure that there is a predecessor recorded for cur, and then while the
  // predecessor of cur is not recorded as itself, keep looping. (The start vertex
  // recorded itself as its predecessor, so then we know to stop.)
  while (pred.count(cur) && pred[cur] != cur) {

    // It's good to note here that the "&&" operator has "short circuiting" behavior,
    // which means that in "A && B", if A is false, then B is never evaluated at all.
    // The whole expression is immediately determined to be false. This way, the
    // "A" part can be used as a safeguard on the "B" part, if "B" could cause problems.

    // Push a copy of the predecessor onto the front of the list, as we reconstruct
    // the path back to the start.
    path.push_front(pred[cur]);
    // Step backwards to the predecessor so we can continue looping.
    cur = pred[cur];
  }

  // Return the reconstructed path from start to goal.
  return path;
}

// =========================================================================
// EXERCISE 3: puzzleBFS
//
// This time, we will use BFS to solve a graph modeling problem. This is
// where we model a realistic problem in terms of an imaginary graph, and
// then we can use graph search concepts to solve the modeled problem.
//
// (Please see the instructions PDF for a detailed introduction to this
//  problem, with illustrations.)
//
// The PuzzleState class represents one current state of the "8 puzzle",
// which is a puzzle played on a 3x3 grid containing 8 tiles, where a tile
// can slide into the blank space to shuffle the puzzle. Each state of the
// puzzle can be modeled as a vertex in an imaginary graph, where two states
// are virtually connected by an edge (adjacent) if a single tile move can
// transform the first state into the second state. We do not need a map
// structure for adjacencies since we can use the helper functions from the
// PuzzleState class to determine which states are adjacent at any given
// time. Therefore we also don't need an explicit graph class at all. It's
// important that we can use such an implicit graph representation, because
// the total number of vertices (states) and edges (moves) in the graph model
// for "8 puzzle" would be extremely large, and that would greatly impact the
// running time and memory usage. We don't need to examine every vertex or
// every edge in the graph model; we can just search from the start and quit
// after either finding the goal or giving up (in relatively few steps).
//
// This function is VERY similar to graphBFS, but now we are using the
// PuzzleState class to represent imaginary vertices instead of using IntPair
// to represent literal 2D points, and we do not use GridGraph. You should
// finish graphBFS first before trying this problem. The starter code shown
// below for puzzleBFS is so similar to graphBFS that the comments have mostly
// been removed.
//
// =========================================================================

// puzzleBFS:
// Given start and goal sates as PuzzleState objects, we perform BFS in the
// imaginary graph model implied by the start state, where the rest of the
// reachable vertices (states) and the edges leading to them (puzzle moves)
// can be figure out based on the tile sliding rules of the puzzle.
// If there exists any path from start to goal, then the puzzle can be solved,
// and we return the shortest path (which represents the solution steps).
// If there is no path to the goal, or if we take too many steps without
// success, then the puzzle cannot be solved, and we return an empty list.
std::list<PuzzleState> puzzleBFS(const PuzzleState& start, const PuzzleState& goal) {

  // Intialization details -------------------------------------------------

  // maxDist failsafe:
  //
  // For the 8-tile puzzle, it actually never takes as many as 35 steps to
  // solve any given puzzle that has a solution. This has been proven in
  // the past by brute force. So we can implement a failsafe measure:
  // if we've explored all possible 35-move sequences and there's still no
  // solution found, then the puzzle cannot be solved. Then we can give up
  // and return early. (There do exist unreachable states for the 8 puzzle
  // because the tiles can only be rearranged by following the sliding rule.
  // So it's possible to specify some "goal" states that cannot be reached by
  // sliding the tiles.)
  constexpr int maxDist = 35;

  std::queue<PuzzleState> exploreQ;
  std::unordered_map<PuzzleState, PuzzleState> pred;
  std::unordered_map<PuzzleState, int> dist;
  std::unordered_set<PuzzleState> visitedSet;
  std::unordered_set<PuzzleState> dequeuedSet;

  pred[start] = start;
  dist[start] = 0;
  visitedSet.insert(start);
  exploreQ.push(start);

  bool foundGoal = (start == goal);
  bool tooManySteps = false;

  // The main search loop --------------------------------------------------

  while (!exploreQ.empty() && !foundGoal && !tooManySteps) {

    auto curState = exploreQ.front();
    exploreQ.pop();

    bool curPointWasPreviouslyDequeued = dequeuedSet.count(curState);
    if (curPointWasPreviouslyDequeued) {
      std::cout << "puzzleBFS ERROR: Dequeued a vertex that had already been dequeued before." << std::endl
        << " If you're using visitedSet correctly, then no vertex should ever be added" << std::endl
        << " to the explore qeueue more than once. [Returning an empty path now.]" << std::endl << std::endl;
      return std::list<PuzzleState>();
    }
    else {
      dequeuedSet.insert(curState);
    }

    // =====================================================================
    // TODO: Your code here!
    // We'll need to loop over the neighbors that are the points adjacent to curState.
    // We need a collection of neighbors we're going to loop over.
    
    //auto neighbors = {start}; // Change this! This line is totally wrong.
    
    auto neighbors = curState.getAdjacentStates();
    

    // Hint: Look at PuzzleState.h
    // =====================================================================

    for (auto neighbor : neighbors) {

      // ==================================================================
      // TODO: Your code here!
      // Check whether the neighbor has already been visited.
      bool neighborWasAlreadyVisited = false; // Change this...
      
      if( 1 == visitedSet.count( neighbor ) )
      {
        neighborWasAlreadyVisited = true;
      }
      
      // ==================================================================

      if (!neighborWasAlreadyVisited) {

        // ================================================================
        // TODO: Your code here!

        // Record that the curState is the predecessor of the neighbor point,
        // since curState has just led to the discovery of this neighbor for
        // the first time.
        // ...
        pred[neighbor] = curState;


        // Add neighbor to the visited set.
        // ...
        visitedSet.insert(neighbor);


        // Push neighbor into the exploration queue.
        // ...
        exploreQ.push(neighbor);

        // ================================================================

        dist[neighbor] = dist[curState]+1;
        if (dist[neighbor] > maxDist) {
          tooManySteps = true;
          break;
        }

        if (neighbor == goal) {
          foundGoal = true;
          break;
        }

      } // end of handling the just-discovered neighbor

    } // end of for loop
  } // end of while loop

  if (tooManySteps) {
    std::cout << "puzzleBFS warning: Could not reach goal within the maximum allowed steps.\n (This may be expected if no path exists.)" << std::endl << std::endl;
    return std::list<PuzzleState>();
  }

  if (!foundGoal) {
    std::cout << "puzzleBFS warning: Could not reach goal. (This may be expected\n if no path exists.)" << std::endl << std::endl;
    return std::list<PuzzleState>();
  }

  std::list<PuzzleState> path;
  auto cur = goal;
  path.push_front(cur);
  while (pred.count(cur) && pred[cur] != cur) {
    path.push_front(pred[cur]);
    cur = pred[cur];
  }

  return path;
}


