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

// includes the FreeCAD configuration
#include "../../../Config.h"

#include <Inventor/Qt/devices/SoQtMouse.h>
#include <Inventor/Qt/devices/SoGuiMouseP.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>

/*!
  \class SoQtMouse SoQtMouse.h Inventor/Qt/devices/SoQtMouse.h
  \brief The SoQtMouse class is the mouse input device abstraction.
  \ingroup devices

  The SoQtMouse class is the glue between native mouse handling and
  mouse interaction in the Inventor scenegraph.

  All components derived from the SoQtRenderArea have got an
  SoQtMouse device attached by default.
*/

// *************************************************************************

SOQT_OBJECT_SOURCE(SoQtMouse);

// *************************************************************************

/*!
  \enum SoQtMouse::Events
  Enumeration over supported mouse events.
*/
/*!
  \var SoQtMouse::Events SoQtMouse::BUTTON_PRESS
  Maskbit for mousebutton press events.
*/
/*!
  \var SoQtMouse::Events SoQtMouse::BUTTON_RELEASE
  Maskbit for mousebutton release events.
*/
/*!
  \var SoQtMouse::Events SoQtMouse::POINTER_MOTION
  Maskbit for mousepointer motion events.
*/
/*!
  \var SoQtMouse::Events SoQtMouse::BUTTON_MOTION
  Maskbit for mousepointer motion events with one or more mousebuttons
  pressed.
*/
/*!
  \var SoQtMouse::Events SoQtMouse::ALL_EVENTS

  Mask which includes all the maskbits in the enum (ie use this to
  signal interest in all kinds of events for the mouse device).
*/

// *************************************************************************

/*!
  \fn SoQtMouse::SoQtMouse(int mask)

  Constructor. The \a mask argument should contain the set of
  SoQtMouse::Events one is interested in tracking.
*/

/*!
  \fn SoQtMouse::~SoQtMouse()
  Destructor.
*/

/*!
  \fn const SoEvent * SoQtMouse::translateEvent(QEvent* event)

  Translates a native event from the underlying toolkit into a generic
  event.

  This is then returned in the form of an instance of a subclass of
  the Inventor API's SoEvent class, either an SoMouseButtonEvent or an
  SoLocation2Event, depending on whether the native event is a
  mousebutton press / release, or a mousecursor movement event.

  The mapping of the mousebuttons upon generation of
  SoMouseButtonEvent events will be done as follows:

  <ul>
  <li>left mousebutton: SoMouseButtonEvent::BUTTON1</li>
  <li>right mousebutton: SoMouseButtonEvent::BUTTON2</li>
  <li>middle mousebutton, if available: SoMouseButtonEvent::BUTTON3</li>
  <li>forward motion on a wheel mouse: SoMouseButtonEvent::BUTTON4</li>
  <li>backward motion on a wheel mouse: SoMouseButtonEvent::BUTTON5</li>
  </ul>

  Note that the rightmost mousebutton will always map to
  SoMouseButtonEvent::BUTTON2, even on a 3-button mouse.
*/

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

SoGuiMouseP::SoGuiMouseP(SoQtMouse * p)
{
  this->pub = p;

  // Allocate system-neutral event objects once and reuse.
  this->buttonevent = new SoMouseButtonEvent;
  this->locationevent = new SoLocation2Event;
}

SoGuiMouseP::~SoGuiMouseP()
{
  delete this->buttonevent;
  delete this->locationevent;
}

#endif // !DOXYGEN_SKIP_THIS

// *************************************************************************
