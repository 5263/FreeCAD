/***************************************************************************
 *   Copyright (c) 2014 Nathan Miller         <Nathan.A.Mill[at]gmail.com> *
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


#ifndef PART_FEATUREFILLING_H
#define PART_FEATUREFILLING_H
#endif

#include <App/PropertyStandard.h>
#include <App/PropertyUnits.h>
#include "../../Part/App/PartFeature.h"

namespace Part
{

class Filling :  public Part::Feature
{
    PROPERTY_HEADER(Part::Filling);

public:
    Filling();

    //Properties

    App::PropertyLinkList Border;                //Border Edges (Must Be Closed) (C0 is required for the border)
    App::PropertyLinkListEnum BOrd {C0, G1, G2}; //Order of Constraint
    App::PropertyLinkList Curves;                //Other Constraint Curves
    App::PropertyLinkListEnum COrd {C0, G1, G2}; //Order of Constraint
    App::PropertyLinkList Points;                //Constraint Points (on Surface)

    // recalculate the feature
    App::DocumentObjectExecReturn *execute(void);
    short mustExecute() const;
    /// returns the type name of the view provider
    const char* getViewProviderName(void) const {
        return "PartGui::ViewProviderFilling";
    }

};
} //Namespace Part
