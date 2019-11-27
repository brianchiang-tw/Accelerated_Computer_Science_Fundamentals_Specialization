/**
 * @file HSLAPixel.h
 *
 * @author University of Illinois CS 225 Course Staff
 * @version 2018r1-lab1 - Updated for CS 400
 */

#pragma once

#include <iostream>
#include <sstream>

namespace uiuc {

  // Put your HSLAPixel class definition here.
  // (Remember to end it with a semicolon!)
  class HSLAPixel {
  
    public:
    
      HSLAPixel();
      HSLAPixel(double hue,double saturation, double luminance, double alpha);
      ~HSLAPixel();
    
      // hue
      double h;
      
      // saturation
      double s;
      
      // luminance
      double l;
      
      // alpha
      double a;
  };
  //End of class definition of HSLAPixel
  
}
//End of namespae uiuc