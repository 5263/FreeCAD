/**************************************************************************\
 *
 *  This file is part of the Coin GUI toolkit libraries.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation.  See the
 *  file LICENSE.LGPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you want to use this library with software that is incompatible
 *  licensewise with the LGPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

// @configure_input@
// includes the FreeCAD configuration
#include "../../../Config.h"

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include <soQtdefs.h>
#include <Inventor/Qt/widgets/SoAnyThumbWheel.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif // ! M_PI

/*!
  \class SoAnyThumbWheel SoAnyThumbWheel.h
  \brief The ThumbWheel class is a helper class for managing thumb wheel
  GUI widgets.
  \internal
*/

/*!
  \var int SoAnyThumbWheel::dirtyTables
  If this flag is set, the internal tables needs to be recalculated.
*/

/*!
  \var int SoAnyThumbWheel::dirtyVariables
  If this flag is set, the internal variables that are calculated from the
  tables and wheel settings need to be recalculated.
*/

inline
int
int8clamp(float f) {
  assert(f >= 0.0f);
  if (f >= 255.0f)
    return 255;
  return (int) floor(f);
} // int8clamp()

// ************************************************************************

/*!
  Constructor.
*/

SoAnyThumbWheel::SoAnyThumbWheel(
  void)
: diameter(0)
, width(0)
, byteorder(ABGR)
, boundaryhandling(ACCUMULATE)
, movement(AUTHENTIC)
, dirtyTables(1)
, dirtyVariables(1)
{
  assert(sizeof(int) == 4 && "FIXME: use int32 datatype instead");

//  this->disabledred = 1.0f;
//  this->disabledgreen = 1.0f;
//  this->disabledblue = 1.0f;

  this->red = 220.0f / 255.0f;
  this->green = 180.0f / 255.0f;
  this->blue = 180.0f / 255.0f;
  
  this->light = 1.3f;
  this->front = 1.2f;
  this->normal = 1.0f;
  this->shade = 0.8f;

  for (int i = 0; i < NUMTABLES; i++)
    this->tables[i] = NULL;
} // SoAnyThumbWheel()

/*!
  Destructor.
*/

SoAnyThumbWheel::~SoAnyThumbWheel(
  void)
{
  for (int i = 0; i < NUMTABLES; i++)
    delete [] this->tables[i];
} // ~ThumbWheel()

// ************************************************************************

/*!
*/

void
SoAnyThumbWheel::setSize(
  const int diameter,
  const int width)
{
  if (this->diameter != diameter) {
    this->dirtyTables = 1;
    this->dirtyVariables = 1;
  } else if (this->width != width) {
    this->dirtyVariables = 1; // embossed squares grows...
  } else {
    return;
  }
  this->diameter = diameter;
  this->width = width;
} // setSize()

/*!
*/

void
SoAnyThumbWheel::getSize(
  int & diameter,
  int & width) const
{
  diameter = this->diameter;
  width = this->width;
} // getSize()

// ************************************************************************

/*!
*/

void
SoAnyThumbWheel::setColor(
  const float red,
  const float green,
  const float blue)
{
  this->red = red;
  this->green = green;
  this->blue = blue;
} // setColor()

/*!
*/

void
SoAnyThumbWheel::getColor(
  float & red,
  float & green,
  float & blue) const
{
  red = this->red;
  green = this->green;
  blue = this->blue;
} // GetColor()

// ************************************************************************

/*!
*/

void
SoAnyThumbWheel::setColorFactors(
  const float light,
  const float front,
  const float normal,
  const float shade)
{
  this->light = light;
  this->front = front;
  this->normal = normal;
  this->shade = shade;
} // setColorFactors()

/*!
*/

void
SoAnyThumbWheel::getColorFactors(
  float & light,
  float & front,
  float & normal,
  float & shade) const
{
  light = this->light;
  front = this->front;
  normal = this->normal;
  shade = this->shade;
} // getColorFactors()

// ************************************************************************

/*!
  This method returns the number of bitmaps required to represent all the
  possible rotations of the thumb wheel.
*/

int
SoAnyThumbWheel::getNumBitmaps(
  void) const
{
  return this->width - 4 + 2 + 1;
} // getNumBitmaps()

// ************************************************************************

/*!
  This method draws thumb wheel bitmap \a number. \a direction decides
  if it should be rendered in the vertical or horizontal position.
*/

void
SoAnyThumbWheel::drawBitmap(
  const int number,
  void * bitmap,
  Alignment alignment) const
{
  if (number == 0) {
    drawDisabledWheel(number, bitmap, alignment);
  } else {
    drawEnabledWheel(number, bitmap, alignment);
  }
}

// ************************************************************************

/*!
  This method calculates the thumb wheel value based on the users mouse
  interaction.  origpos and origval is the mouseposition at the moment of
  pressing the button, and origval is the thumbwheels original value at
  that point.  deltapos is the number of pixels the mouse has moved since
  pressing the button.
  
  The point in doing it this way is that the thumbwheel can easily be made
  to behave like an authentic wheel when all of these parameters are known.

  After doing a CalculateValue, you should redraw the wheel if the value has
  changed (since last time, not compared to origval).
*/

float
SoAnyThumbWheel::calculateValue(
  const float origval,
  const int origpos,
  const int deltapos) const
{
  this->validate();

//  fprintf(stderr, "calc(%8.4f, %d, %d)\n", origval, origpos, deltapos);

  float diff = 0.0f;
  switch (this->movement) {
  case UNIFORM:
    diff = this->unistep * deltapos;
    break;
  case AUTHENTIC:
    int newpos = origpos + deltapos;
    if (newpos < 0) newpos = 0;
    if (newpos >= diameter) newpos = diameter - 1;
    diff = this->tables[RAD][newpos] - this->tables[RAD][origpos];
    break;
  } // switch (this->movement)

  switch (this->boundaryhandling) {
  case MODULATE:
    while ((origval + diff) < 0.0f         ) diff += 2.0f * M_PI;
    while ((origval + diff) > (2.0f * M_PI)) diff -= 2.0f * M_PI;
    break;
  case CLAMP:
    if    ((origval + diff) < 0.0f         ) diff = 0.0f - origval;
    if    ((origval + diff) > (2.0f * M_PI)) diff = (2.0f * M_PI) - origval;
    break;
  case ACCUMULATE:
    // nothing - just add the difference on top of original value
    break;
  } // switch (this->boundaryhandling)

  return origval + diff;
} // calculateValue()

// ************************************************************************

/*!
  This method returns the bitmap you need to display to represent a thumb
  wheel with the given value and active state.
*/

int
SoAnyThumbWheel::getBitmapForValue(
  const float value,
  const State state) const
{
  this->validate();

  if (state == DISABLED)
    return 0; // only one disabled bitmap in this implementation

  float squarerange = (2.0f * M_PI) / (float) numsquares;
  float normalizedmodval = fmod(value, squarerange) / squarerange;
  if (normalizedmodval < 0.0f) normalizedmodval += 1.0f;
  int bitmap = 1 + (int) (normalizedmodval * (float) (this->width - 4 + 2));
  return bitmap;
} // getBitmapForValue()

// ************************************************************************

/*!
  This method sets whether to set up uints in ARGB or BGRA mode before
  storing them in the 32-bit frame buffer.
*/

void
SoAnyThumbWheel::setGraphicsByteOrder(
  const GraphicsByteOrder byteorder)
{
  this->byteorder = byteorder;
} // setGraphicsByteOrder()

/*!
*/

SoAnyThumbWheel::GraphicsByteOrder
SoAnyThumbWheel::getGraphicsByteOrder(
  void) const
{
  return this->byteorder;
} // getGraphicsByteOrder()

// ************************************************************************

/*!
*/

void
SoAnyThumbWheel::setMovement(
  const Movement movement)
{
  this->movement = movement;
} // setMovement()

/*!
*/

SoAnyThumbWheel::Movement
SoAnyThumbWheel::getMovement(
  void) const
{
  return this->movement;
} // getMovement()

// ************************************************************************

void
SoAnyThumbWheel::setBoundaryHandling(
  const BoundaryHandling handling)
{
  this->boundaryhandling = handling;
} // setBoundaryHandling()

/*!
*/

SoAnyThumbWheel::BoundaryHandling
SoAnyThumbWheel::getBoundaryHandling(
  void) const
{
  return this->boundaryhandling;
} // getBoundaryHandling()

// ************************************************************************

/*!
  This method validates the ThumbWheel object, recalculating the tables and
  state variables if necessary.  If object is already valid, no calculations
  are performed.
*/

void
SoAnyThumbWheel::validate(// private
  void) const
{
  int i;

  if (this->dirtyTables) {
    assert(this->dirtyVariables);
    for (i = 0; i < NUMTABLES; i++) {
      if (this->tables[i]) delete [] this->tables[i];
      this->tables[i] = new float [ this->diameter ];
    }

    float radius = ((float) this->diameter + 1.0f) / 2.0f;
//    float range = 2.0f * M_PI;
    float acos0times2 = 2.0f*acos(0.0f);

    for (i = 0; i < this->diameter; i++) {
      if ((float) i <= radius) {
        this->tables[COS][i] = (radius - (float) i) / radius;
        this->tables[RAD][i] = acos(this->tables[COS][i]);
      } else {
        this->tables[COS][i] = ((float) i - radius) / radius;
        this->tables[RAD][i] = acos0times2 - acos(this->tables[COS][i]);
      }
      this->tables[SIN][i] = sqrt(1.0f - this->tables[COS][i] * this->tables[COS][i]);
    }

    this->dirtyTables = 0;
  }

  if (this->dirtyVariables) {
    assert(this->dirtyTables == 0);
    if ((this->diameter % 2) == 0)
      this->unistep = this->tables[RAD][this->diameter/2] -
                      this->tables[RAD][(this->diameter/2)-1];
    else
      this->unistep = (this->tables[RAD][(this->diameter/2)+1] -
                       this->tables[RAD][(this->diameter/2)-1]) / 2.0f;

    this->squarespacing = 2.0f * this->unistep;
    this->shadelength = this->unistep; // remove?

    int squares = (int) floor(((2.0f * M_PI) /
                         ((((float) width - 4.0f) * this->unistep) + this->squarespacing)) + 0.5f);
    this->numsquares = squares;
    this->squarelength = ((2.0f * M_PI) / (float) squares) - this->squarespacing;

    this->dirtyVariables = 0;
  }
} // validate()

// ************************************************************************

inline unsigned int
SoAnyThumbWheel::swapWord(
  unsigned int orig) const
{
  // orig is RGBA
  unsigned int copy;
  if (this->byteorder == ABGR) {
    copy = (orig & 0x000000ff) <<  24;
    copy |= (orig & 0x0000ff00) << 8;
    copy |= (orig & 0x00ff0000) >> 8;
    copy |= (orig & 0xff000000) >> 24;
  } else if (this->byteorder == ARGB) {
    copy = orig >> 8;             // RGB
    copy |= (orig & 0xff) << 24;  // A
  } else if (this->byteorder == BGRA) {
    copy = orig & 0x00ff00ff;          // G & A
    copy |= (orig & 0x0000ff00) << 16; // B
    copy |= (orig & 0xff000000) >> 16; // R
  } else {
    copy = orig; // ?
  }
  return copy;
} // swapWord()

/*!
  This method draws a wheel that has been disabled from being rotated.
*/

void
SoAnyThumbWheel::drawDisabledWheel(// private
  const int number,
  void * bitmap,
  Alignment alignment) const
{
  assert(number == 0);

  this->validate();

  unsigned int * buffer = (unsigned int *) bitmap;

  for (int j = 0; j < this->diameter; j++) {
    unsigned int light, front, normal, shade;
    light  = (unsigned int) int8clamp(floor(255.0f * this->tables[SIN][j] * 1.15f));
    front = 0;
    normal = (unsigned int) int8clamp(floor(255.0f * this->tables[SIN][j]));
    shade  = (unsigned int) int8clamp(floor(255.0f * this->tables[SIN][j] * 0.85f));

    // rgbx
    light  =  (light << 24) |  (light << 16) |  (light << 8);
    front  =  (front << 24) |  (front << 16) |  (front << 8);
    normal = (normal << 24) | (normal << 16) | (normal << 8);
    shade  =  (shade << 24) |  (shade << 16) |  (shade << 8);

    if (this->byteorder != RGBA) {
      light  = this->swapWord( light);
      front  = this->swapWord( front);
      normal = this->swapWord(normal);
      shade  = this->swapWord( shade);
    }

    if (alignment == VERTICAL) {
      buffer[j*this->width] = light;
      for (int i = 1; i < (width - 1); i++)
        buffer[(j*this->width)+i] = normal;
      buffer[(j*this->width)+(this->width-1)] = shade;
    } else {
      buffer[j] = light;
      for (int i = 1; i < (this->width-1); i++)
        buffer[j+(i*this->diameter)] = normal;
      buffer[j+((this->width-1)*this->diameter)] = shade;
    }
  }
} // drawDisabledWheel()

// ************************************************************************

/*
  This method draws the thumb wheel.
  Shading/lighting aliasing can still be done better...
  Can also optimize the drawing to use less computing power...
*/

void
SoAnyThumbWheel::drawEnabledWheel(
  const int number,
  void * bitmap,
  Alignment alignment) const
{
  this->validate();

  int numEnabledBitmaps = this->width - 4 + 2;
  assert(number > 0 && number <= numEnabledBitmaps);

  float modulo = (2.0f * M_PI) / numsquares;
  float radian = modulo - (((2.0f * M_PI) / (float) numsquares) * (((float) (number - 1)) / (float) numEnabledBitmaps));

  int newsquare = 1;
  unsigned int * buffer = (unsigned int *) bitmap;
  for (int j = 0; j < this->diameter; j++) {
    unsigned int light, front, normal, shade, color;

    light  = (int8clamp(floor(255.0f * this->red * this->tables[SIN][j] * this->light)) << 24) +
             (int8clamp(floor(255.0f * this->green * this->tables[SIN][j] * this->light)) << 16) +
             (int8clamp(floor(255.0f * this->blue * this->tables[SIN][j] * this->light)) << 8);
    front  = (int8clamp(floor(255.0f * this->red * this->tables[SIN][j] * this->front)) << 24) +
             (int8clamp(floor(255.0f * this->green * this->tables[SIN][j] * this->front)) << 16) +
             (int8clamp(floor(255.0f * this->blue * this->tables[SIN][j] * this->front)) << 8);
    normal = (int8clamp(floor(255.0f * this->red * this->tables[SIN][j] * this->normal)) << 24) +
             (int8clamp(floor(255.0f * this->green * this->tables[SIN][j] * this->normal)) << 16) +
             (int8clamp(floor(255.0f * this->blue * this->tables[SIN][j] * this->normal)) << 8);
    shade  = (int8clamp(floor(255.0f * this->red * this->tables[SIN][j] * this->shade)) << 24) +
             (int8clamp(floor(255.0f * this->green * this->tables[SIN][j] * this->shade)) << 16) +
             (int8clamp(floor(255.0f * this->blue * this->tables[SIN][j] * this->shade)) << 8);

    if (this->byteorder != RGBA) {
      light   = this->swapWord(light);
      normal  = this->swapWord(normal);
      front   = this->swapWord(front);
      shade   = this->swapWord(shade);
    }

    static int flag = 0;
    if (newsquare) {
      color=front;
      newsquare = 0;
      flag = 1;
    } else {
      if (flag == 1) {
        if (j < (this->diameter * 2 / 3))
          color = shade;
        else
          color = normal;
        flag = 0;
      } else {
        color = normal;
      }
    }

    if (alignment == VERTICAL) {
      buffer[(this->width*j)] = front;
      buffer[(this->width*j)+1] = front;
      if (flag == 1) buffer[(this->width*j)+2] = front;
      else                buffer[(this->width*j)+2] = shade;

      for (int i = 3; i < (this->width-2); i++)
        buffer[(j*this->width)+i] = color;

      if (flag == 1) buffer[(this->width*j)+this->width-3] = front;
      else                buffer[(this->width*j)+this->width-3] = normal;
      buffer[(this->width*j)+this->width-2] = front;
      buffer[(this->width*j)+this->width-1] = front;
    } else {

      buffer[j] = front;
      buffer[j+this->diameter] = front;
      if (flag == 1) buffer[j+(this->diameter*2)] = front;
      else                buffer[j+(this->diameter*2)] = shade;

      for (int i = 2; i < (this->width-2); i++)
        buffer[j+(i*this->diameter)] = color;

      if (flag == 1) buffer[j+(this->diameter*(this->width-3))] = front;
      else                buffer[j+(this->diameter*(this->width-3))] = normal;
      buffer[j+(this->diameter*(this->width-2))] = front;
      buffer[j+(this->diameter*(this->width-1))] = front;
    }

    if (j < (this->diameter - 1)) {
      radian += this->tables[RAD][j+1] - this->tables[RAD][j];
      if (radian > modulo) {
        if (alignment == VERTICAL) {
          int color = 0;
          if (j > (this->diameter * 2 / 3))
            color = light;
          else if (j > (this->diameter / 3))
            color = front;
          if (color != 0)
            for (int i = 3; i < (this->width-2); i++)
              buffer[(j*this->width)+i] = color;
        } else {
          int color = 0;
          if (j > (this->diameter * 2 / 3))
            color = light;
          else if (j > (this->diameter / 3))
            color = front;
          if (color != 0)
            for (int i = 3; i < (this->width-2); i++)
              buffer[j+(this->diameter*i)] = color;
        }
        radian = fmod(radian, modulo);
        newsquare = 1;
      }
    }
  }
} // drawEnabledWheel()

// ************************************************************************
