/***************************************************************************
 *   Copyright (c) 2007                                                    *
 *   Joachim Zettler <Joachim.Zettler@gmx.de>                              *
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


#ifndef __PRECOMPILED__
#define __PRECOMPILED__

#include <FCConfig.h>


// Exporting of App classes
#ifdef FC_OS_WIN32
    # define AppCamExport   __declspec(dllexport)
    # define CamExport      __declspec(dllexport)
    # define AppPartExport  __declspec(dllimport)
    # define PartExport     __declspec(dllimport)
    # define AppMeshExport  __declspec(dllimport)
    # define MeshExport     __declspec(dllimport)
#else // for Linux
    # define AppCamExport
    # define CamExport
    # define AppPartExport
    # define PartExport
    # define AppMeshExport
    # define MeshExport
#endif


#ifdef _PreComp_

    // here get the warnings of too long specifiers disabled (needed for VC6)
    #ifdef _MSC_VER
        # pragma warning( disable : 4251 )
        # pragma warning( disable : 4275 )
        # pragma warning( disable : 4503 )
        # pragma warning( disable : 4786 )  // specifier longer then 255 chars
        # pragma warning( disable : 4101 )
    #endif

    // STL
    #include <vector>
    #include <list>
    #include <map>
    #include <string>
    #include <set>
    #include <algorithm>
    #include <stack>
    #include <queue>
    #include <bitset>

    #include <Python.h>

    #ifdef FC_OS_WIN32
        #include <windows.h>
    #endif

    // Xerces
    #include <xercesc/util/XercesDefs.hpp>

    // OpenCasCade Base
    #include <Standard_Failure.hxx>

    // OpenCascade View
    #include <Mod/Part/App/OpenCascadeAll.h>

#endif //_PreComp_

#endif //__PRECOMPILED__


