/***************************************************************************
 *   Copyright (c) 2005 Werner Mayer <wmayer[at]users.sourceforge.net>     *
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
# include <qobject.h>
#endif

#include "Workbench.h"
#include <Gui/MenuManager.h>
#include <Gui/ToolBarManager.h>

using namespace PartGui;

TYPESYSTEM_SOURCE(PartGui::Workbench, Gui::StdWorkbench)

Workbench::Workbench()
{
}

Workbench::~Workbench()
{
}

Gui::MenuItem* Workbench::setupMenuBar() const
{
    Gui::MenuItem* root = StdWorkbench::setupMenuBar();
    Gui::MenuItem* item = root->findItem("&Windows");

    Gui::MenuItem* part = new Gui::MenuItem;
    root->insertItem(item, part);
    part->setCommand(QT_TR_NOOP("&Part"));
    *part << "Part_Primitives" << "Part_Boolean" << "Part_Extrude"
          << "Part_Revolve" << "Part_Fillet" << "Separator" << "Part_ShapeInfo";

    Gui::MenuItem* partSimple = new Gui::MenuItem;
    root->insertItem(item, partSimple);
    partSimple->setCommand(QT_TR_NOOP("&Simple"));
    *partSimple << "Part_SimpleCylinder";

    Gui::MenuItem* solids = new Gui::MenuItem;
    root->insertItem(item, solids);
    solids->setCommand(QT_TR_NOOP("&Parametric"));
    *solids << "Part_Box" << "Part_Cylinder" << "Part_Sphere" << "Part_Cone"
            << "Part_Torus" << "Separator" << "Part_Primitives";

    return root;
}

Gui::ToolBarItem* Workbench::setupToolBars() const
{
    Gui::ToolBarItem* root = StdWorkbench::setupToolBars();

    Gui::ToolBarItem* solids = new Gui::ToolBarItem(root);
    solids->setCommand(QT_TR_NOOP("Solids"));
    *solids << "Part_Box" << "Part_Cylinder" << "Part_Sphere" << "Part_Cone" << "Part_Torus";

    Gui::ToolBarItem* tool = new Gui::ToolBarItem(root);
    tool->setCommand(QT_TR_NOOP("Part tools"));
    *tool << "Part_Extrude" << "Part_Revolve" << "Part_Fillet";

    Gui::ToolBarItem* boolop = new Gui::ToolBarItem(root);
    boolop->setCommand(QT_TR_NOOP("Boolean"));
    *boolop << "Part_Boolean" << "Part_Cut" << "Part_Fuse" << "Part_Common" << "Part_Section";

    return root;
}

Gui::ToolBarItem* Workbench::setupCommandBars() const
{
    // Part tools
    Gui::ToolBarItem* root = new Gui::ToolBarItem;
    return root;
}

