/***************************************************************************
 *   Copyright (c) 2007 Werner Mayer <werner.wm.mayer@gmx.de>              *
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
# include <BRep_Builder.hxx>
# include <BRepBuilderAPI_MakeEdge.hxx>
# include <Geom_Line.hxx>
# include <gp_Pnt.hxx>
# include <gp_Lin.hxx>
# include <TopoDS_Compound.hxx>
# include <TopoDS_Edge.hxx>
#endif

#include "FeaturePartLineSet.h"

using namespace Part;

PROPERTY_SOURCE(Part::LineSet, Part::Feature)


LineSet::LineSet()
{
    gp_Lin line;
    line.SetLocation(gp_Pnt(0.0,0.0,0.0));
    ADD_PROPERTY(Lines,(line));
}

LineSet::~LineSet()
{
}

short LineSet::mustExecute() const
{
    if (Lines.isTouched())
        return 1;
    return 0;
}

App::DocumentObjectExecReturn *LineSet::execute(void)
{
    TopoDS_Compound aCompound;
    BRep_Builder aBuilder;
    aBuilder.MakeCompound(aCompound);

    const std::vector<gp_Lin>& lines = Lines.getValues();

    for (std::vector<gp_Lin>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
        const gp_Pnt& p = it->Location();
        const gp_Dir& d = it->Direction(); 
        // Convert into OCC representation
        gp_Pnt pnt1 = p;
        gp_Pnt pnt2(p.X()+d.X(), p.Y()+d.Y(), p.Z()+d.Z());

        // Create directly the underlying line geometry
        BRepBuilderAPI_MakeEdge makeEdge(pnt1,pnt2);
        bool ok = false;
        const char* error=0;
        switch ( makeEdge.Error() )
        {
        case BRepBuilderAPI_EdgeDone:
            ok = true;
            break; // ok
        case BRepBuilderAPI_PointProjectionFailed:
            error = "Point projection failed";
            break;
        case BRepBuilderAPI_ParameterOutOfRange:
            error = "Parameter out of range";
            break;
        case BRepBuilderAPI_DifferentPointsOnClosedCurve:
            error = "Different points on closed curve";
            break;
        case BRepBuilderAPI_PointWithInfiniteParameter:
            error = "Point with infinite parameter";
            break;
        case BRepBuilderAPI_DifferentsPointAndParameter:
            error = "Different point and parameter";
            break;
        case BRepBuilderAPI_LineThroughIdenticPoints:
            error = "Line through identic points";
            break;
        }

        // Error 
        if (!ok) return new App::DocumentObjectExecReturn(error);
    
        // add created edge to the compound
        TopoDS_Edge edge = makeEdge.Edge();
        aBuilder.Add(aCompound, edge);
    }

    setShape(aCompound);

    return App::DocumentObject::StdReturn;
}

