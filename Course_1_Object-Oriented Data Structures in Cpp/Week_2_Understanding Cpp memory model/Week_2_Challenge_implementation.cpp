#include <iostream>

// This class Pair has already been defined for you.
// (You may not change this definition.)
class Pair {
public:
  int first, second;
  
  void check() {
    first = 5;
    std::cout << "Congratulations! The check() method of the Pair class \n has executed. (But, this isn't enough to guarantee \n that your code is correct.)" << std::endl;
  }
  
};




// Insert your declaration and implementation of the function pairFactory()
// below, by replacing "..." with proper C++ code. Be sure to declare the
// function type to return a pointer to a Pair.

Pair* pairFactory() {
  // ...
  // (You can use as many lines as you want.)
  
  // create an instance of class Pair
  Pair* pair_ptr = new Pair();
  
  // return the memory address of pair_ptr
  return pair_ptr;
}




// Your function should be able to satisfy the tests below. You should try
// some other things to convince yourself. If you have a bug in this problem,
// the usual symptom is that after you submit, the grader will crash with a
// system error. :-)


// expected output of main()
/*

Congratulations! The check() method of the Pair class 
 has executed. (But, this isn't enough to guarantee 
 that your code is correct.)
If you can see this text, the system hasn't crashed yet!

*/


int main() {
  Pair *p;
  p = pairFactory();
  
  // This function call should work without crashing:
  p->check();
  
  // Deallocating the heap memory. (Assuming it was made on the heap!)
  delete p;

  std::cout << "If you can see this text, the system hasn't crashed yet!" << std::endl;  

  return 0;
}
