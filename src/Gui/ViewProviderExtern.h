/***************************************************************************
 *   Copyright (c) 2004 J�rgen Riegel <juergen.riegel@web.de>              *
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


#ifndef __ViewProviderExtern_H__
#define __ViewProviderExtern_H__

#include "ViewProvider.h"


class SoMaterial;

namespace App
{
  class Material;
}


namespace Gui {


class GuiExport ViewProviderExtern:public ViewProvider
{
   PROPERTY_HEADER(Gui::ViewProviderExtern);

public:
  /// constructor.
  ViewProviderExtern();

  /// destructor.
  virtual ~ViewProviderExtern();

  void setModeByString(const char* name, const char* ivFragment);
  void setModeByFile(const char* name, const char* ivFileName);
  void setModeBySoInput(const char* name, SoInput &ivFileInput);

  
  virtual std::vector<std::string> getModes(void);

  virtual void update(void){}

  /// Set the transparency
  virtual void setTransparency(float);
  /// Set the color
  virtual void setColor(const App::Color &c);

protected:

  std::vector<std::string> modes;

};






} // namespace Gui

#endif // __ViewProviderExtern_H__

