/***************************************************************************
 *   Copyright (c) 2010 J�rgen Riegel <juergen.riegel@web.de>              *
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
# include <QRegExp>
#endif

#include "DlgSettingsUnitsImp.h"
#include "NavigationStyle.h"
#include "PrefWidgets.h"
#include <App/Application.h>
#include <Base/Console.h>
#include <Base/Parameter.h>
#include <Base/UnitsApi.h>

using namespace Gui::Dialog;
using namespace Base;

/**
 *  Constructs a DlgSettingsUnitsImp which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
DlgSettingsUnitsImp::DlgSettingsUnitsImp(QWidget* parent)
    : PreferencePage( parent )
{
    this->setupUi(this);
    this->setEnabled(false);
    retranslate();


    tableWidget->setRowCount(10);
    for(int i = 0 ; i<9;i++){
        QTableWidgetItem *newItem = new QTableWidgetItem(UnitsApi::getQuntityName((Base::UnitsApi::QuantityType)i));
        tableWidget->setItem(i, 0, newItem);
        
        newItem = new QTableWidgetItem(UnitsApi::getPrefUnitOf((Base::UnitsApi::QuantityType)i));
        tableWidget->setItem(i, 1, newItem);

    }

}

/** 
 *  Destroys the object and frees any allocated resources
 */
DlgSettingsUnitsImp::~DlgSettingsUnitsImp()
{
    // no need to delete child widgets, Qt does it all for us
}

void DlgSettingsUnitsImp::saveSettings()
{
    // must be done as very first because we create a new instance of NavigatorStyle
    // where we set some attributes afterwards
    ParameterGrp::handle hGrp = App::GetApplication().GetParameterGroupByPath
        ("User parameter:BaseApp/Preferences/Units");
    //hGrp->SetASCII("NavigationStyle", "");
}

void DlgSettingsUnitsImp::loadSettings()
{
    QString exe = QString::fromUtf8(App::Application::Config()["ExeName"].c_str());
    QString viewtext = comboBox_ViewSystem->itemText(0);
    comboBox_ViewSystem->setItemText(0, QString::fromAscii("%1 %2").arg(exe).arg(viewtext));
    QString resettext = comboBox_ResetSystem->itemText(0);
    comboBox_ResetSystem->setItemText(0, QString::fromAscii("%1 %2").arg(exe).arg(resettext));
    ParameterGrp::handle hGrp = App::GetApplication().GetParameterGroupByPath
        ("User parameter:BaseApp/Preferences/Units");
    //std::string model = hGrp->GetASCII("NavigationStyle","");
}

/**
 * Sets the strings of the subwidgets using the current language.
 */
void DlgSettingsUnitsImp::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        retranslateUi(this);
        retranslate();
    }
    else {
        QWidget::changeEvent(e);
    }
}

void DlgSettingsUnitsImp::retranslate()
{
}

#include "moc_DlgSettingsUnitsImp.cpp"
