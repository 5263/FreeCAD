/** \file DlgSettings3DViewImp.cpp
 *  \brief  
 *  \author $Author$
 *  \version $Revision$
 *  \date    $Date$
 *   
 */

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
 *   Juergen Riegel 2003                                                   *
 ***************************************************************************/


/** Precompiled header stuff
 *  on some compilers the precompiled header option gain significant compile 
 *  time! So every external header (libs and system) should included in 
 *  Precompiled.h. For systems without precompilation the header needed are
 *  included in the else fork.
 */
#include "PreCompiled.h"

#ifndef _PreComp_
#	include <qbutton.h>
#	include <qlabel.h>
#	include <qlayout.h>
#endif

#include "PropertyPage.h"
#include "DlgSettings3DViewImp.h"
#include "Widgets.h"
#include "../Base/Console.h"

using namespace Gui::Dialog;

/* 
 *  Constructs a CDlgSettings3DViewImp which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
CDlgSettings3DViewImp::CDlgSettings3DViewImp( QWidget* parent,  const char* name, WFlags fl )
    : DlgSettings3DView( parent, name, fl )
{
	append(UseAntialiasing->getHandler());
	append(FCCheckBox_UseInventorViewer->getHandler());
	append(CheckBox_CornerCoordSystem->getHandler());
	append(CheckBox_ShowFPS->getHandler());
	append(CheckBox_UseSimpleBackground->getHandler());
	append(CheckBox_ShowViewerDecoration->getHandler());
	append(FCLineEdit_UserDefinedViewerExt->getHandler());
	append(CheckBox_UseAutoRotation->getHandler());

}

/*  
 *  Destroys the object and frees any allocated resources
 */
CDlgSettings3DViewImp::~CDlgSettings3DViewImp()
{
    // no need to delete child widgets, Qt does it all for us
}

void CDlgSettings3DViewImp::ChooseDir()
{
	QString cPath = FileDialog::getOpenFileName( QString::null, "Inventor (*.iv)");

	FCLineEdit_UserDefinedViewerExt->setText(cPath.latin1());
}

void  CDlgSettings3DViewImp::WarnInventor(bool b)
{
	if(b)
		Base::Console().Warning("The inventor Viewer is highly experimental. Usage can cause FreeCAD to crash!\n");
}



#include "DlgSettings3DView.cpp"
#include "moc_DlgSettings3DView.cpp"
#include "moc_DlgSettings3DViewImp.cpp"
