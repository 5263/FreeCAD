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


#ifndef FEATURE_MESH_Export_H
#define FEATURE_MESH_Export_H

#include "MeshFeature.h"

#include <App/PropertyStandard.h>
#include <App/PropertyLinks.h>

namespace Mesh
{

/**
 * The Export class reads the any supported mesh format
 * into the FreeCAD workspace.
 * @author Werner Mayer
 */
class AppMeshExport Export : public Mesh::Feature
{
  PROPERTY_HEADER(Mesh::Export);

public:
  Export();

  App::PropertyLink   Source;
  App::PropertyString FileName;
  App::PropertyString Format;
  /// Get the mesh kernel of the feature of 'Source'.
  const MeshCore::MeshKernel& getMesh() const;
  //const char* getViewProviderName(void){return "MeshGui::ViewProviderExport";}

  /** @name methods override Feature */
  //@{
  /// recalculate the Feature
  virtual int execute(void);
  //@}
};

}

#endif // FEATURE_MESH_Export_H 
