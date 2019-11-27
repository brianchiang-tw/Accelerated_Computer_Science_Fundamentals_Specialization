#include <iostream>
#include <vector>

//vector<int> traversal_path;


class DisjointSets {
public:
	int s[256];

	DisjointSets() { for (int i = 0; i < 256; i++) s[i] = -1; }

	int find(int i);
};

/* Modify the find() method below
 * to implement path compression
 * so that element i and all of
 * its ancestors in the up-tree
 * point to directly to the root
 * after find() completes.
 */

/*
int DisjointSets::find(int i) {
  if ( s[i] < 0 ) {
    return i;
  } else {
    return find(s[i]);
  }
}
*/

int DisjointSets::find( int i)
{
 
 static std::vector<int> traversal_path;
 
 if( s[i] < 0 )
 {
   for( auto iter = traversal_path.begin() ; iter != traversal_path.end() ; iter++)
   {
     // path compression from i to root node
     int node = *iter;
     s[ node ] = i;
     
   }
   
   // return the name(i.e., array index) of Disjoint set
   return i;
 }
 else
 {
   //add currnet node into traversal path
   traversal_path.push_back( i );
   return find( s[i] );
 }
  
}
//end of function int DisjointSet::find( int i)


// expected output of main:

/*

d.find(3) = 7
d.s(1) = 3
d.s(3) = 7
d.s(5) = 7
d.s(7) = -1

*/


int main() {
  DisjointSets d;

  d.s[1] = 3;
  d.s[3] = 5;
  d.s[5] = 7;
  d.s[7] = -1;

  std::cout << "d.find(3) = " << d.find(3) << std::endl;
  std::cout << "d.s(1) = " << d.s[1] << std::endl;
  std::cout << "d.s(3) = " << d.s[3] << std::endl;
  std::cout << "d.s(5) = " << d.s[5] << std::endl;
  std::cout << "d.s(7) = " << d.s[7] << std::endl;

  return 0;
}