/***************************************************************************
 *   Copyright (c) J�rgen Riegel          (juergen.riegel@web.de) 2006     *
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



#ifndef __FEATURETEST_H__
#define __FEATURETEST_H__


#include "Feature.h"
#include "PropertyStandard.h"
#include "PropertyGeo.h"
#include "PropertyLinks.h"

namespace App
{

/// The testing feature
class FeatureTest :public AbstractFeature
{
  PROPERTY_HEADER(App::FeatureTest);

public:
  FeatureTest();

  // Standard Properties (PorpertyStandard.h)
  App::PropertyInteger      Integer;
  App::PropertyFloat        Float;
  App::PropertyBool         Bool;
  App::PropertyString       String;

  // special types
  App::PropertyDistance     Distance;
  App::PropertyAngle        Angle;

  // Constraint types
  App::PropertyEnumeration       Enum;
  App::PropertyIntegerConstraint ConstraintInt;
  App::PropertyFloatConstraint   ConstraintFloat;
 
  // Standard Properties (PorpertyStandard.h)
  App::PropertyIntegerList IntegerList;
  App::PropertyFloatList   FloatList;

  // Standard Properties (PropertyLinks.h)
  App::PropertyLink    Link;

  // Standard Properties (PropertyGeo.h)
  App::PropertyMatrix  Matrix;
  App::PropertyVector  Vector;

  App::PropertyString  ExecResult;
  
  
  /** @name methods overide Feature */
  //@{
  /// recalculate the Feature
  virtual int execute(void);
  /// returns the type name of the ViewProvider
  //FIXME: Propably it makes sense to have a view provider for unittests (e.g. Gui::ViewProviderTest)
  virtual const char* getViewProviderName(void) const {
    return "Gui::ViewProviderDocumentObject";
  }
  //@}
};

/// The exception testing feature
class FeatureTestException :public FeatureTest
{
  PROPERTY_HEADER(App::FeatureTestException);

public:
  FeatureTestException();

  /// this property defines which kind of exceptio the feature throw on you
  App::PropertyInteger ExceptionType;

  /// recalculate the Feature and throw an exeption
  virtual int execute(void);
  /// returns the type name of the ViewProvider
  virtual const char* getViewProviderName(void) const {
    return "Gui::ViewProviderDocumentObject";
  }
};



} //namespace App

#endif // __FEATURETEST_H__
