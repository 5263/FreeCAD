/***************************************************************************
 *   Copyright (c) 2004 Werner Mayer <werner.wm.mayer@gmx.de>              *
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

#ifndef __VIEWPROVIDERMESHTRANSFORMDEMOLDING_H__
#define __VIEWPROVIDERMESHTRANSFORMDEMOLDING_H__

#ifndef _PreComp_
# include <Inventor/nodes/SoRotation.h>
#endif

class SoSeparator;
class SbVec3f;
class SoSwitch;
class SoCoordinate3;
class SoDragger;
class SoNormal;
class SoIndexedFaceSet;
class SoFaceSet;
class SoPath;
class SoLocateHighlight;
class SoTrackballDragger;
class SoTransformerManip;

namespace Gui {
  class View3DInventorViewer;
}


namespace Mesh {
  class MeshWithProperty;
  class MeshPropertyColor;
}

#include "ViewProvider.h"
#include <Base/Vector3D.h>

namespace MeshGui {

/** Like Mesh viewprovider but with manipulator
 */
class ViewProviderMeshTransformDemolding : public ViewProviderMesh
{
public:
  ViewProviderMeshTransformDemolding();
  virtual ~ViewProviderMeshTransformDemolding();


  /** 
   * Extracts the mesh data from the feature \a pcFeature and creates
   * an Inventor node \a SoNode with these data. 
   */
  virtual void attach(App::Feature *);

  /// set the viewing mode
  virtual void setMode(const char* ModeName);
  /// returns a vector of all possible modes
  virtual std::vector<std::string> getModes(void);
  /// Update the Mesh representation
  //virtual void updateData(void);

protected:
  void calcMaterialIndex(const SbRotation &rot);
  void calcNormalVector(void);

  static void sValueChangedCallback(void *, SoDragger *);
  void valueChangedCallback(void);

  static void sDragEndCallback(void *, SoDragger *);
  void DragEndCallback(void);

  SoTrackballDragger *pcTrackballDragger;
  SoTransform        *pcTransformDrag;
  SoMaterial         *pcColorMat;
  std::vector<SbVec3f> normalVector;
  Base::Vector3D            center;

};

} // namespace MeshGui


#endif // __VIEWPROVIDERMESHTRANSFORMDEMOLDING_H__

