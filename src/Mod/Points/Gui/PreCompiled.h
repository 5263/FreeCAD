#ifndef __PRECOMPILED_GUI__
#define __PRECOMPILED_GUI__

#include "../../../FCConfig.h"

// Importing of App classes
#ifdef FC_OS_WIN32
# define PointsAppExport __declspec(dllimport)
# define PointsGuiExport __declspec(dllexport)
#else // for Linux
# define PointsAppExport
# define PointsGuiExport
#endif

#ifdef _PreComp_

/// here get the warnings of to long specifieres disabled (needed for VC6)
#ifdef _MSC_VER
# pragma warning( disable : 4251 )
# pragma warning( disable : 4503 )
# pragma warning( disable : 4786 )  // specifier longer then 255 chars
#endif

// standard
#include <stdio.h>
#include <assert.h>

// STL
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

// Xerces
#include <xercesc/util/XercesDefs.hpp>

#include <Python.h>
#ifdef FC_OS_WIN32
# include <windows.h>
#endif

// Inventor
#ifndef __InventorAll__
# include <Gui/InventorAll.h>
#endif
 
// Qt Toolkit
#ifndef __Qt4All__
# include <Gui/Qt4All.h>
#endif

#ifndef __Qt3All__
# include <Gui/Qt3All.h>
#endif

#endif  //_PreComp_

#endif // __PRECOMPILED_GUI__ 
