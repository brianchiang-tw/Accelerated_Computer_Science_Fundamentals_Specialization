#include <iostream>
#include <cmath>
#include <cstdlib>

#include "uiuc/PNG.h"
#include "uiuc/HSLAPixel.h"
#include "ImageTransform.h"

/* ******************
(Begin multi-line comment...)

Write your name and email address in the comment space here:

Name:
Email:

(...end multi-line comment.)
******************** */

using uiuc::PNG;
using uiuc::HSLAPixel;

/**
 * Returns an image that has been transformed to grayscale.
 *
 * The saturation of every pixel is set to 0, removing any color.
 *
 * @return The grayscale image.
 */
PNG grayscale(PNG image) {
  /// This function is already written for you so you can see how to
  /// interact with our PNG class.
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);

      // `pixel` is a reference to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly. No need to `set`
      // the pixel since you're directly changing the memory of the image.
      pixel.s = 0;
    }
  }

  return image;
}



/**
 * Returns an image with a spotlight centered at (`centerX`, `centerY`).
 *
 * A spotlight adjusts the luminance of a pixel based on the distance the pixel
 * is away from the center by decreasing the luminance by 0.5% per 1 pixel euclidean
 * distance away from the center.
 *
 * For example, a pixel 3 pixels above and 4 pixels to the right of the center
 * is a total of `sqrt((3 * 3) + (4 * 4)) = sqrt(25) = 5` pixels away and
 * its luminance is decreased by 2.5% (0.975x its original value).  At a
 * distance over 160 pixels away, the luminance will always decreased by 80%.
 * 
 * The modified PNG is then returned.
 *
 * @param image A PNG object which holds the image data to be modified.
 * @param centerX The center x coordinate of the crosshair which is to be drawn.
 * @param centerY The center y coordinate of the crosshair which is to be drawn.
 *
 * @return The image with a spotlight.
 */
PNG createSpotlight(PNG image, int centerX, int centerY) {

  // luminance degration = 0.5 % per pixel
  // = 0.005 per pixel
  double luminance_degrade = 0.005;

  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      
      // get pixel reference from image
      HSLAPixel & pixel = image.getPixel(x, y);
      
      // original pixel luminance
      double original_luminance = pixel.l;

      // calculate radius
      //double radius = floor( sqrt( ( x - centerX )^2 + ( y - centerY )^2 ) );
      double radius = ( sqrt( pow( x - centerX, 2 ) + pow( y - centerY, 2 ) ) );
      
      // maximal degration stop at r = 160
      if ( radius >= 160.0 )
      {
        radius = 160.0;
      }
      
      double  luminance_adjustment = radius * luminance_degrade;
      
      // update new pixel luminance
      pixel.l = original_luminance * ( 1.0 - luminance_adjustment );

      
    }
    //end of loop y
    
  }
  //end of loop x

  return image;
  
}
//end of function PNG createSpotlight
 

/**
 * Returns a image transformed to Illini colors.
 *
 * The hue of every pixel is set to the a hue value of either orange or
 * blue, based on if the pixel's hue value is closer to orange than blue.
 *
 * @param image A PNG object which holds the image data to be modified.
 *
 * @return The illinify'd image.
**/
PNG illinify(PNG image) {


  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      
      HSLAPixel & pixel = image.getPixel(x, y);

      // `pixel` is a reference to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly. No need to `set`
      // the pixel since you're directly changing the memory of the image.
      
      double current_hue = pixel.h;
      double illini_orange = 11.0;
      double illini_blue = 216.0;
      
      //double dist_to_orange = current_hue - illini_orange;
      //double dist_to_blue = current_hue - illini_blue;
      
      double dist_to_orange = min( abs( current_hue - illini_orange ), abs( 360.0+illini_orange-current_hue ) );
      double dist_to_blue = min( abs( current_hue - illini_blue ), abs( 360.0+illini_blue-current_hue ) );
      
      
      if( dist_to_orange < dist_to_blue )
      {
        pixel.h = illini_orange;
      }
      else
      {
        pixel.h = illini_blue;
      }
      //end of if...else...
      
    }
    //end of loop y
    
  }
  //end of loop x


  return image;
}
//end of function PNG illinify(PNG image)
 

/**
* Returns an immge that has been watermarked by another image.
*
* The luminance of every pixel of the second image is checked, if that
* pixel's luminance is 1 (100%), then the pixel at the same location on
* the first image has its luminance increased by 0.2.
*
* @param firstImage  The first of the two PNGs, which is the base image.
* @param secondImage The second of the two PNGs, which acts as the stencil.
*
* @return The watermarked image.
*/
PNG watermark(PNG firstImage, PNG secondImage) {

  for (unsigned x = 0; x < secondImage.width(); x++) {
    
    for (unsigned y = 0; y < secondImage.height(); y++) {
      
      HSLAPixel & stencil_pixel = secondImage.getPixel(x, y);
      HSLAPixel & base_pixel = firstImage.getPixel(x, y);
      
      if( 1.0 == stencil_pixel.l )
      {
        double luminance_upper_bound = 1.0;
        double watermark_luminance_offset = 0.2;
        // luminance + 0.2, but not exceed upper bound 1.0
        double new_base_pixel_luminance = min( base_pixel.l + watermark_luminance_offset, luminance_upper_bound );
        
        // update base pixel luminance
        base_pixel.l = new_base_pixel_luminance;
      }
      
      
      
    }
    //end of for y loop
  }
  //end of for x loop



  return firstImage;
}
//end of function PNG watermark(PNG firstImage, PNG secondImage)
