/***************************************************************************
 *   Copyright (c) 2007 Werner Mayer <wmayer@users.sourceforge.net>        *
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



#ifndef PART_FEATUREPARTBOOLEAN_H
#define PART_FEATUREPARTBOOLEAN_H

#include <App/PropertyLinks.h>

#include "PartFeature.h"

namespace Part
{


class Boolean : public Part::Feature
{
    PROPERTY_HEADER(Part::Boolean);

public:
    Boolean();

    App::PropertyLink Base;
    App::PropertyLink Tool;


    /** @name methods overide Feature */
    //@{
    /// recalculate the Feature
    App::DocumentObjectExecReturn *execute(void);
    short mustExecute() const;
    //@}

protected:
    virtual TopoDS_Shape runOperation(TopoDS_Shape, TopoDS_Shape) const = 0;
};

}


#endif // PART_FEATUREPARTBOOLEAN_H
