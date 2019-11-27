/**
 * @file PNG.h
 *
 * @author University of Illinois CS 225 Course Staff
 * @version 2018r1 - Updated for CS 400
 */

#pragma once

#include <string>
#include <vector>
#include "HSLAPixel.h"

using namespace std;

namespace uiuc {
  class PNG {
  public:
    /**
      * Creates an empty PNG image.
      */
    PNG();

    /**
      * Creates a PNG image of the specified dimensions.
      * @param width Width of the new image.
      * @param height Height of the new image.
      */
    PNG(unsigned int width, unsigned int height);

    /**
      * Copy constructor: creates a new PNG image that is a copy of
      * another.
      * @param other PNG to be copied.
      */
    PNG(PNG const & other);

    /**
      * Destructor: frees all memory associated with a given PNG object.
      * Invoked by the system.
      */
    ~PNG();

    /**
      * Assignment operator for setting two PNGs equal to one another.
      * @param other Image to copy into the current image.
      * @return The current image for assignment chaining.
      */
    PNG const & operator= (PNG const & other);

    /**
      * Equality operator: checks if two images are the same.
      * @param other Image to be checked.
      * @return Whether the current image is equal to the other image.
      */
    bool operator== (PNG const & other) const;

    /**
      * Inequality operator: checks if two images are different.
      * @param other Image to be checked.
      * @return Whether the current image differs from the other image.
      */
    bool operator!= (PNG const & other) const;


    /**
      * Reads in a PNG image from a file.
      * Overwrites any current image content in the PNG.
      * @param fileName Name of the file to be read from.
      * @return true, if the image was successfully read and loaded.
      */
    bool readFromFile(string const & fileName);

    /**
      * Writes a PNG image to a file.
      * @param fileName Name of the file to be written.
      * @return true, if the image was successfully written.
      */
    bool writeToFile(string const & fileName);

    /**
      * Pixel access operator. Gets a reference to the pixel at the given
      * coordinates in the image. (0,0) is the upper left corner.
      * This reference allows the image to be changed.
      * @param x X-coordinate for the pixel reference to be grabbed from.
      * @param y Y-coordinate for the pixel reference to be grabbed from.
      * @return A reference to the pixel at the given coordinates.
      */
    HSLAPixel & getPixel(unsigned int x, unsigned int y) const;

    /**
      * Gets the width of this image.
      * @return Width of the image.
      */
    unsigned int width() const;

    /**
      * Gets the height of this image.
      * @return Height of the image.
      */
    unsigned int height() const;

    /**
      * Resizes the image to the given coordinates. Attempts to preserve
      * existing pixel data in the image when doing so, but will crop if
      * necessary. No pixel interpolation is done.
      * @param newWidth New width of the image.
      * @param newHeight New height of the image.
      */
    void resize(unsigned int newWidth, unsigned int newHeight);

    /**
     * Computes a hash of the contents of the image.
     */
    std::size_t computeHash() const;

  private:
    unsigned int width_;            /*< Width of the image */
    unsigned int height_;           /*< Height of the image */
    HSLAPixel *imageData_;          /*< Array of pixels */
    HSLAPixel defaultPixel_;        /*< Default pixel, returned in cases of errors */

    /**
     * Copeies the contents of `other` to self
     */
     void _copy(PNG const & other);
  };

  std::ostream & operator<<(std::ostream & out, PNG const & pixel);
  std::stringstream & operator<<(std::stringstream & out, PNG const & pixel);
}

