/* Class Pair has already been declared
 * as shown in the following comments:
 */
 
 #include <iostream>
 
 using namespace std;


 class Pair {
 	
	public:
    	int *pa,*pb;
    	
    	Pair(int, int);
    	
    	Pair(const Pair &);
    	
		~Pair();
	
  };
 
 
 // Implement its member functions below.
 
Pair::Pair(int a_val_ , int b_val_ )
 {
 	// allocate pa with a_val_
 	// allocate pb with b_val_
 	pa = new int( a_val_ );
 	pb = new int( b_val_ );
 }
 //End of constructor
 
Pair::Pair(const Pair& p)
 {
	pa = new int( *(p.pa) );
	pb = new int( *(p.pb) );
 }
 //End of copy construtor with deep-copy
 
 Pair::~Pair()
 {
 	// deallocate pa and pb
 	delete pa;
 	delete pb;
 	
 	// Set released ptr points to NULL
 	pa = nullptr;
 	pb = nullptr;
 }
 //End of destructor
 
 
 /* Here is a main() function you can use
  * to check your implementation of the
  * class Pair member functions.
  */


// expected output
/*

If this message is printed, at least the program hasn't crashed yet!
But you may want to print other diagnostic messages too.

*/

int main() {
  Pair p(15,16);
  Pair q(p);
  Pair *hp = new Pair(23,42);
  delete hp;
  
  std::cout << "If this message is printed,"
    << " at least the program hasn't crashed yet!\n"
    << "But you may want to print other diagnostic messages too." << std::endl;
  return 0;
}