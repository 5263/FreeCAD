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


#include "PreCompiled.h"
#ifndef _PreComp_
#endif

#include <App/Application.h>
#include <Base/Console.h>

#include <stdio.h>
#include <Python.h>

#include "FeatureMeshImport.h"
#include "FeatureMeshTransform.h"
#include "FeatureMeshTransformDemolding.h"
#include "FeatureMeshCurvature.h"


/* registration table  */
extern struct PyMethodDef Mesh_Import_methods[];


/* Python entry */
extern "C" {
void AppMeshExport initMesh() {

  (void) Py_InitModule("Mesh", Mesh_Import_methods);   /* mod name, table ptr */

  

  Base::Console().Log("AppMesh loaded\n");
	App::FeatureFactory().AddProducer("MeshImport",            new App::FeatureProducer<Mesh::FeatureMeshImport>);
	App::FeatureFactory().AddProducer("Mesh",                  new App::FeatureProducer<Mesh::MeshFeature>      );
	App::FeatureFactory().AddProducer("MeshTransform"         ,new App::FeatureProducer<Mesh::FeatureMeshTransform>      );
	App::FeatureFactory().AddProducer("MeshTransformDemolding",new App::FeatureProducer<Mesh::FeatureMeshTransformDemolding>      );
	App::FeatureFactory().AddProducer("MeshCurvature"         ,new App::FeatureProducer<Mesh::FeatureMeshCurvature>      );

  return;
}

} // extern "C" 
