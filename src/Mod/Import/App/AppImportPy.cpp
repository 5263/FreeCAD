/***************************************************************************
 *   (c) J�rgen Riegel (juergen.riegel@web.de) 2002                        *   
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License (LGPL)   *
 *   as published by the Free Software Foundation; either version 2 of     *
 *   the License, or (at your option) any later version.                   *
 *   for detail see the LICENCE text file.                                 *
 *                                                                         *
 *   FreeCAD is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        * 
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with FreeCAD; if not, write to the Free Software        * 
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
 *   USA                                                                   *
 *                                                                         *
 *   Juergen Riegel 2002                                                   *
 ***************************************************************************/

 
#include "PreCompiled.h"
#ifndef _PreComp_
#	include <stdio.h>
#	include <Python.h>
# include <BRep_Builder.hxx>
# include <BRepTools.hxx>
#endif

#include <Base/Console.h>
#include <Base/Interpreter.h>

#include <App/Application.h>
#include <App/Topology.h>



/* module functions */
static PyObject *                                 /* returns object */
open(PyObject *self, PyObject *args)              /* self unused in modules */
{                                                 /* args from python call */
	std::string strResult;

  if (! PyArg_ParseTuple(args, ""))			  /* convert Python -> C */
    return NULL;                              /* null=raise exception */
    
	strResult += "The Import module\n";

	Base::PyBuf bufTemp(strResult.c_str());
	return Py_BuildValue("s", bufTemp.str);   /* convert C -> Python */
    
}

/* module functions */
static PyObject *                                 /* returns object */
save(PyObject *self, PyObject *args)          /* self unused in modules */
{                                                 /* args from python call */
	char* str;

    if (! PyArg_ParseTuple(args, "s",&str))		  /* convert Python -> C */
        return NULL;                              /* null=raise exception */
  
	TopoDS_Shape ResultShape;
	BRep_Builder aBuilder;

	BRepTools::Read(ResultShape,(const Standard_CString)str,aBuilder);

  return new App::TopoShapePy(ResultShape);		  /* convert C -> Python */
    
}



/* registration table  */
struct PyMethodDef Import_methods[] = {
    {"open", open, 1},				/* method name, C func ptr, always-tuple */
    {"save", save, 1},

    {NULL, NULL}                   /* end of table marker */
};
