/***************************************************************************
 *   Copyright (c) 2008 J�rgen Riegel (juergen.riegel@web.de)              *
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
# include <TopExp_Explorer.hxx>
# include <QMessageBox>
#endif

#include <Gui/Application.h>
#include <Gui/Command.h>
#include <Gui/Control.h>
#include <Gui/Selection.h>
#include <Gui/MainWindow.h>
#include <Gui/FileDialog.h>

#include <Mod/Part/App/Part2DObject.h>
#include "TaskChamfer.h"


using namespace std;

//===========================================================================
// Part_Pad
//===========================================================================

/* Sketch commands =======================================================*/
//DEF_STD_CMD_A(CmdPartDesignNewSketch);
//
//CmdPartDesignNewSketch::CmdPartDesignNewSketch()
//	:Command("PartDesign_NewSketch")
//{
//    sAppModule      = "PartDesign";
//    sGroup          = QT_TR_NOOP("PartDesign");
//    sMenuText       = QT_TR_NOOP("Create sketch");
//    sToolTipText    = QT_TR_NOOP("Create a new sketch");
//    sWhatsThis      = sToolTipText;
//    sStatusTip      = sToolTipText;
//    sPixmap         = "Sketcher_NewSketch";
//}
//
//
//void CmdPartDesignNewSketch::activated(int iMsg)
//{
//    const char camstring[] = "#Inventor V2.1 ascii \\n OrthographicCamera { \\n viewportMapping ADJUST_CAMERA \\n position 0 0 87 \\n orientation 0 0 1  0 \\n nearDistance 37 \\n farDistance 137 \\n aspectRatio 1 \\n focalDistance 87 \\n height 119 }";
//
//    std::string FeatName = getUniqueObjectName("Sketch");
//
//    std::string cam(camstring);
//
//    openCommand("Create a new Sketch");
//    doCommand(Doc,"App.activeDocument().addObject('Sketcher::SketchObject','%s')",FeatName.c_str());
//    doCommand(Gui,"Gui.activeDocument().activeView().setCamera('%s')",cam.c_str());
//    doCommand(Gui,"Gui.activeDocument().setEdit('%s')",FeatName.c_str());
//    
//    //getDocument()->recompute();
//}
//
//bool CmdPartDesignNewSketch::isActive(void)
//{
//    if (getActiveGuiDocument())
//        return true;
//    else
//        return false;
//}
//

//===========================================================================
// PartDesign_Pad
//===========================================================================
DEF_STD_CMD_A(CmdPartDesignPad);

CmdPartDesignPad::CmdPartDesignPad()
  : Command("PartDesign_Pad")
{
    sAppModule    = "PartDesign";
    sGroup        = QT_TR_NOOP("PartDesign");
    sMenuText     = QT_TR_NOOP("Pad");
    sToolTipText  = QT_TR_NOOP("Pad a selected sketch");
    sWhatsThis    = sToolTipText;
    sStatusTip    = sToolTipText;
    sPixmap       = "PartDesign_Pad";
}

void CmdPartDesignPad::activated(int iMsg)
{
    unsigned int n = getSelection().countObjectsOfType(Part::Part2DObject::getClassTypeId());
    if (n != 1) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Wrong selection"),
            QObject::tr("Select a sketch or 2D object."));
        return;
    }

    std::string FeatName = getUniqueObjectName("Pad");

    std::vector<App::DocumentObject*> Sel = getSelection().getObjectsOfType(Part::Part2DObject::getClassTypeId());
    Part::Part2DObject* sketch = static_cast<Part::Part2DObject*>(Sel.front());
    const TopoDS_Shape& shape = sketch->Shape.getValue();
    if (shape.IsNull()) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Wrong selection"),
            QObject::tr("The shape of the selected object is empty."));
        return;
    }

    // count free wires
    int ctWires=0;
    TopExp_Explorer ex;
    for (ex.Init(shape, TopAbs_WIRE); ex.More(); ex.Next()) {
        ctWires++;
    }
    if (ctWires == 0) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Wrong selection"),
            QObject::tr("The shape of the selected object is not a wire."));
        return;
    }

    App::DocumentObject* support = sketch->Support.getValue();

    openCommand("Make Pad");
    doCommand(Doc,"App.activeDocument().addObject(\"PartDesign::Pad\",\"%s\")",FeatName.c_str());
    doCommand(Doc,"App.activeDocument().%s.Sketch = App.activeDocument().%s",FeatName.c_str(),sketch->getNameInDocument());
    doCommand(Doc,"App.activeDocument().%s.Length = 10.0",FeatName.c_str());
    updateActive();
    if (isActiveObjectValid()) {
        doCommand(Gui,"Gui.activeDocument().hide(\"%s\")",sketch->getNameInDocument());
        if (support)
            doCommand(Gui,"Gui.activeDocument().hide(\"%s\")",support->getNameInDocument());
    }
    //doCommand(Gui,"Gui.activeDocument().setEdit('%s')",FeatName.c_str());

    commitCommand();
    adjustCameraPosition();
}

bool CmdPartDesignPad::isActive(void)
{
    return hasActiveDocument();
}

//===========================================================================
// PartDesign_Pocket
//===========================================================================
DEF_STD_CMD_A(CmdPartDesignPocket);

CmdPartDesignPocket::CmdPartDesignPocket()
  : Command("PartDesign_Pocket")
{
    sAppModule    = "PartDesign";
    sGroup        = QT_TR_NOOP("PartDesign");
    sMenuText     = QT_TR_NOOP("Pocket");
    sToolTipText  = QT_TR_NOOP("create a pocket with the selected sketch");
    sWhatsThis    = sToolTipText;
    sStatusTip    = sToolTipText;
    sPixmap       = "PartDesign_Pocket";
}

void CmdPartDesignPocket::activated(int iMsg)
{
    unsigned int n = getSelection().countObjectsOfType(Part::Part2DObject::getClassTypeId());
    if (n != 1) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Wrong selection"),
            QObject::tr("Select a sketch or 2D object."));
        return;
    }

    std::string FeatName = getUniqueObjectName("Pocket");

    std::vector<App::DocumentObject*> Sel = getSelection().getObjectsOfType(Part::Part2DObject::getClassTypeId());
    Part::Part2DObject* sketch = static_cast<Part::Part2DObject*>(Sel.front());
    const TopoDS_Shape& shape = sketch->Shape.getValue();
    if (shape.IsNull()) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Wrong selection"),
            QObject::tr("The shape of the selected object is empty."));
        return;
    }

    // count free wires
    int ctWires=0;
    TopExp_Explorer ex;
    for (ex.Init(shape, TopAbs_WIRE); ex.More(); ex.Next()) {
        ctWires++;
    }
    if (ctWires == 0) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Wrong selection"),
            QObject::tr("The shape of the selected object is not a wire."));
        return;
    }
    App::DocumentObject* support = sketch->Support.getValue();
    if (support == 0) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("No Support"),
            QObject::tr("The sketch has to have a support for the pocket feature.\nCreate the sketch on a face."));
        return;
    }

    openCommand("Make Pocket");
    doCommand(Doc,"App.activeDocument().addObject(\"PartDesign::Pocket\",\"%s\")",FeatName.c_str());
    doCommand(Doc,"App.activeDocument().%s.Sketch = App.activeDocument().%s",FeatName.c_str(),sketch->getNameInDocument());
    doCommand(Doc,"App.activeDocument().%s.Length = 5.0",FeatName.c_str());
    updateActive();
    if (isActiveObjectValid()) {
        doCommand(Gui,"Gui.activeDocument().hide(\"%s\")",sketch->getNameInDocument());
        doCommand(Gui,"Gui.activeDocument().hide(\"%s\")",support->getNameInDocument());
    }
    commitCommand();
}

bool CmdPartDesignPocket::isActive(void)
{
    return hasActiveDocument();
}

//===========================================================================
// PartDesign_Revolution
//===========================================================================
DEF_STD_CMD_A(CmdPartDesignRevolution);

CmdPartDesignRevolution::CmdPartDesignRevolution()
  : Command("PartDesign_Revolution")
{
    sAppModule    = "PartDesign";
    sGroup        = QT_TR_NOOP("PartDesign");
    sMenuText     = QT_TR_NOOP("Revolution");
    sToolTipText  = QT_TR_NOOP("Revolve a selected sketch");
    sWhatsThis    = sToolTipText;
    sStatusTip    = sToolTipText;
    sPixmap       = "PartDesign_Revolution";
}

void CmdPartDesignRevolution::activated(int iMsg)
{
    unsigned int n = getSelection().countObjectsOfType(Part::Part2DObject::getClassTypeId());
    if (n != 1) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Wrong selection"),
            QObject::tr("Select a sketch or 2D object."));
        return;
    }

    std::string FeatName = getUniqueObjectName("Revolution");

    std::vector<App::DocumentObject*> Sel = getSelection().getObjectsOfType(Part::Part2DObject::getClassTypeId());
    Part::Part2DObject* sketch = static_cast<Part::Part2DObject*>(Sel.front());
    const TopoDS_Shape& shape = sketch->Shape.getValue();
    if (shape.IsNull()) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Wrong selection"),
            QObject::tr("The shape of the selected object is empty."));
        return;
    }

    // count free wires
    int ctWires=0;
    TopExp_Explorer ex;
    for (ex.Init(shape, TopAbs_WIRE); ex.More(); ex.Next()) {
        ctWires++;
    }
    if (ctWires == 0) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Wrong selection"),
            QObject::tr("The shape of the selected object is not a wire."));
        return;
    }

    App::DocumentObject* support = sketch->Support.getValue();

    openCommand("Make Revolution");
    doCommand(Doc,"App.activeDocument().addObject(\"PartDesign::Revolution\",\"%s\")",FeatName.c_str());
    doCommand(Doc,"App.activeDocument().%s.Sketch = App.activeDocument().%s",FeatName.c_str(),sketch->getNameInDocument());
    doCommand(Doc,"App.activeDocument().%s.Base = App.Vector(0,0,0)",FeatName.c_str());
    doCommand(Doc,"App.activeDocument().%s.Axis = App.Vector(0,1,0)",FeatName.c_str());
    doCommand(Doc,"App.activeDocument().%s.Angle = 360.0",FeatName.c_str());
    updateActive();
    if (isActiveObjectValid()) {
        doCommand(Gui,"Gui.activeDocument().hide(\"%s\")",sketch->getNameInDocument());
        if (support)
            doCommand(Gui,"Gui.activeDocument().hide(\"%s\")",support->getNameInDocument());
    }
    commitCommand();
    adjustCameraPosition();
}

bool CmdPartDesignRevolution::isActive(void)
{
    return hasActiveDocument();
}

//===========================================================================
// PartDesign_Fillet
//===========================================================================
DEF_STD_CMD_A(CmdPartDesignFillet);

CmdPartDesignFillet::CmdPartDesignFillet()
  :Command("PartDesign_Fillet")
{
    sAppModule    = "PartDesign";
    sGroup        = QT_TR_NOOP("PartDesign");
    sMenuText     = QT_TR_NOOP("Fillet");
    sToolTipText  = QT_TR_NOOP("Make a fillet on an edge, face or body");
    sWhatsThis    = sToolTipText;
    sStatusTip    = sToolTipText;
    sPixmap       = "Part_Fillet";
}

void CmdPartDesignFillet::activated(int iMsg)
{
    //unsigned int n = getSelection().countObjectsOfType(Part::Feature::getClassTypeId());

	std::vector<Gui::SelectionObject> selection = getSelection().getSelectionEx();

    if (selection.size() != 1) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Wrong selection"),
            QObject::tr("Select an edge, face or body. Only one body is allowed."));
        return;
    }

	if ( ! selection[0].isObjectTypeOf(Part::Feature::getClassTypeId())){
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Wrong object type"),
            QObject::tr("Fillet works only on parts"));
        return;
    }
	std::string SelString = selection[0].getAsPropertyLinkSubString();
    std::string FeatName = getUniqueObjectName("Fillet");

    openCommand("Make Fillet");
    doCommand(Doc,"App.activeDocument().addObject(\"PartDesign::Fillet\",\"%s\")",FeatName.c_str());
    doCommand(Doc,"App.activeDocument().%s.Base = %s",FeatName.c_str(),SelString.c_str());
    doCommand(Gui,"Gui.activeDocument().hide(\"%s\")",selection[0].getFeatName());
    updateActive();
    commitCommand();
}

bool CmdPartDesignFillet::isActive(void)
{
    return hasActiveDocument();
}

//===========================================================================
// PartDesign_Chamfer
//===========================================================================
DEF_STD_CMD_A(CmdPartDesignChamfer);

CmdPartDesignChamfer::CmdPartDesignChamfer()
  :Command("PartDesign_Chamfer")
{
    sAppModule    = "PartDesign";
    sGroup        = QT_TR_NOOP("Part");
    sMenuText     = QT_TR_NOOP("Chamfer...");
    sToolTipText  = QT_TR_NOOP("Chamfer the selected edges of a shape");
    sWhatsThis    = sToolTipText;
    sStatusTip    = sToolTipText;
    sPixmap       = "Part_Chamfer";
}

void CmdPartDesignChamfer::activated(int iMsg)
{
    Gui::Control().showDialog(new PartDesignGui::TaskChamfer());
}

bool CmdPartDesignChamfer::isActive(void)
{
    return (hasActiveDocument() && !Gui::Control().activeDialog());
}


void CreatePartDesignCommands(void)
{
    Gui::CommandManager &rcCmdMgr = Gui::Application::Instance->commandManager();

    rcCmdMgr.addCommand(new CmdPartDesignPad());
    rcCmdMgr.addCommand(new CmdPartDesignPocket());
    rcCmdMgr.addCommand(new CmdPartDesignRevolution());
    rcCmdMgr.addCommand(new CmdPartDesignFillet());
    //rcCmdMgr.addCommand(new CmdPartDesignNewSketch());
    rcCmdMgr.addCommand(new CmdPartDesignChamfer());
 }
