/***************************************************************************
 *   Copyright (c) 2008 Werner Mayer <wmayer[at]users.sourceforge.net>     *
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
# include <BRepFilletAPI_MakeFillet.hxx>
# include <TopExp_Explorer.hxx>
# include <TopoDS.hxx>
# include <TopoDS_Edge.hxx>
#endif


#include "FeatureFillet.h"


using namespace PartDesign;


PROPERTY_SOURCE(PartDesign::Fillet, Part::Feature)

const App::PropertyFloatConstraint::Constraints floatRadius = {0.0f,FLT_MAX,0.1f};

Fillet::Fillet()
{
    ADD_PROPERTY(Base,(0));
    ADD_PROPERTY(Radius,(0.2f));
    Radius.setConstraints(&floatRadius);
}

short Fillet::mustExecute() const
{
    if (Base.isTouched() || Radius.isTouched())
        return 1;
    return 0;
}

App::DocumentObjectExecReturn *Fillet::execute(void)
{
    App::DocumentObject* link = Base.getValue();
    if (!link)
        return new App::DocumentObjectExecReturn("No object linked");
    if (!link->getTypeId().isDerivedFrom(Part::Feature::getClassTypeId()))
        return new App::DocumentObjectExecReturn("Linked object is not a Part object");
    Part::Feature *base = static_cast<Part::Feature*>(Base.getValue());

    float radius = Radius.getValue();

    BRepFilletAPI_MakeFillet mkFillet(base->Shape.getValue());
    TopExp_Explorer xp(base->Shape.getValue(), TopAbs_EDGE);

    for (;xp.More();xp.Next()) {
        TopoDS_Edge edge = TopoDS::Edge(xp.Current());
        mkFillet.Add(radius, radius, edge);
    }

    try {
        TopoDS_Shape shape = mkFillet.Shape();
        if (shape.IsNull())
            return new App::DocumentObjectExecReturn("Resulting shape is null");
        this->Shape.setValue(shape);
        return App::DocumentObject::StdReturn;
    }
    catch (Standard_Failure) {
        Handle_Standard_Failure e = Standard_Failure::Caught();
        return new App::DocumentObjectExecReturn(e->GetMessageString());
    }
}
