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

#include <Base/Interpreter.h>
#include <Base/Parameter.h>
#include <Base/Console.h>

#include <App/Application.h>

#include <Gui/Application.h>

#include "ViewProvider.h"


void CreateCommands(void);

/* module functions */
static PyObject *                                 /* returns object */
message(PyObject *self, PyObject *args)           /* self unused in modules */
{                                                 /* args from python call */
    char *fromPython, result[64];
    if (! PyArg_ParseTuple(args, "(s)", &fromPython))  /* convert Python -> C */
        return NULL;                              /* null=raise exception */
    else {
        strcpy(result, "Hello, ");                /* build up C string */
        strcat(result, fromPython);               /* add passed Python string */
        return Py_BuildValue("s", result);        /* convert C -> Python */
    }
}

/* registration table  */
static struct PyMethodDef hello_methods[] = {
    {"message", message, 1},       /* method name, C func ptr, always-tuple */
    {NULL, NULL}                   /* end of table marker */
};

/* Python entry */
extern "C" {
void GuiMeshExport initMeshGui() {
  if ( !Gui::ApplicationWindow::Instance )
  {
    PyErr_SetString(PyExc_ImportError, "Cannot load Gui module in console application.");
    return;
  }

  (void) Py_InitModule("MeshGui", hello_methods);   /* mod name, table ptr */

  // load needed modules
  Base::Interpreter().loadModule("Mesh");
  Base::Console().Log("AppMeshGui loaded\n");

  // instanciating the commands
  CreateCommands();

  // Register view provider
  Gui::ViewProviderInventorFeatureFactory().AddProducer("MeshImportSTL", new Gui::ViewProviderInventorFeatureProducer<MeshGui::ViewProviderInventorMesh>);
  Gui::ViewProviderInventorFeatureFactory().AddProducer("Mesh"         , new Gui::ViewProviderInventorFeatureProducer<MeshGui::ViewProviderInventorMesh>);

  return;
}
} // extern "C" {
