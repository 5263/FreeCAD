/***************************************************************************
 *   Copyright (c) 2005 Werner Mayer <werner.wm.mayer@gmx.de>              *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#ifndef COIN_SOFCCOLORLEGEND_H
#define COIN_SOFCCOLORLEGEND_H

#include <Inventor/nodes/SoSeparator.h>
#include <App/ColorModel.h>

class SoCoordinate3;
class SoMFString;
class SbVec2s;

namespace Gui {

class GuiExport SoFCColorLegend : public SoSeparator {
  typedef SoSeparator inherited;

  SO_NODE_HEADER(Gui::SoFCColorLegend);

public:
  static void initClass(void);
  SoFCColorLegend(void);

  void setMarkerLabel( const SoMFString& label );
  void setViewerSize( const SbVec2s& size );

  /**
   * Sets the range of the colorbar from the maximum \a fMax to the minimum \a fMin.
   * \a prec indicates the post decimal positions, \a prec should be in between 0 and 6.
   */
  void setRange( float fMin, float fMax, int prec=3 );

  unsigned short getColorIndex (float fVal) const { return _cColRamp.getColorIndex(fVal);  }
  App::Color getColor (float fVal) const { return _cColRamp.getColor(fVal); }
  float getMinValue (void) const { return _cColRamp.getMinValue(); }
  float getMaxValue (void) const { return _cColRamp.getMaxValue(); }
  unsigned long countColors (void) const { return _cColRamp.getCtColors(); }

//  virtual void handleEvent(SoHandleEventAction * action);
//  virtual void GLRenderBelowPath(SoGLRenderAction * action);
//  virtual void GLRenderInPath(SoGLRenderAction * action);

protected:
  virtual ~SoFCColorLegend();
//  virtual void redrawHighlighted(SoAction * act, SbBool  flag);

  SoCoordinate3* coords;
  SoSeparator* labels;

private:
  float _fMaxX, _fMaxY;
  App::ColorRamp _cColRamp;
};

} // namespace Gui


#endif // COIN_SOFCCOLORLEGEND_H

