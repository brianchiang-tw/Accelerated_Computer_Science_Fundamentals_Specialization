/**
 * @file HSLAPixel.cpp
 * Implementation of the HSLAPixel class for use in with the PNG library.
 *
 * @author University of Illinois CS 225 Course Staff
 * @version 2018r1-lab1 - Updated for CS 400
 */

#include <cmath>
#include <iostream>
#include "HSLAPixel.h"
using namespace std;

namespace uiuc {
    
    // default constructor
    HSLAPixel::HSLAPixel():HSLAPixel(0.0, 0.0, 0.0, 0.0)
    {
        
    }
    
    // constructor with parameters hue, saturation, luminance, alpha
    HSLAPixel::HSLAPixel(double h_val, double s_val, double l_val, double a_val)
    {
        h = h_val;
        s = s_val;
        l = l_val;
        a = a_val;
    }
        

    HSLAPixel::~HSLAPixel()
    {
        
    }

}
//End of namespae uiuc