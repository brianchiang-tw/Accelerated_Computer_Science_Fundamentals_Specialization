#include <iostream>
using namespace std;

/* Class Pair has already been
* declared and defined with the
* following constructor:
*
*   Pair(int,int)
*
* that stores its two arguments in
* two private member variables of Pair.
*
* Class sumPair has also already been
* defined as follows:
*/

class Pair{
  public:
    Pair(int num_1, int num_2): a(num_1), b(num_2) {}
    int a;
    int b;

};

class sumPair : public Pair {
  public:
    int sum;
    sumPair(int,int);
};



 /* 
 * Implement the constructor
 * sumPair(int,int) such that it
 * loads the two member variables of
 * the base Pair class with its
 * arguments, and initializes the
 * member variable sum with their sum.
 */
 
 // constructor of sumPair
sumPair::sumPair(int a, int b) : Pair( a, b)
{
	sum = a + b;
}



/* Below is a main() function
* you can use to test your
* implementation of the
* sumPair constructor.
*/
 
// expected output of main:
/* 
 sp(15,16).sum =31
*/

int main() {
  sumPair sp(15,16);
  std::cout << "sp(15,16).sum =" << sp.sum << std::endl;
  return 0;
}