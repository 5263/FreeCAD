/***************************************************************************
 *   Copyright (c) 2005 Werner Mayer <werner.wm.mayer@gmx.de>              *
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


#ifndef GUI_DIALOG_DLGKEYBOARD_IMP_H
#define GUI_DIALOG_DLGKEYBOARD_IMP_H

#include "ui_DlgKeyboard.h"
#include "PropertyPage.h"

#ifndef __Qt4All__
# include "Qt4All.h"
#endif

#ifndef __Qt3All__
# include "Qt3All.h"
#endif

namespace Gui {
class Command;
class CommandBase;
namespace Dialog {

/** Shows an overview of all available commands of all groups and modules.
 * You can customize your workbenches just by drag&dropping any commands
 * onto the toolbars or commandbars. But you cannot modify any standard toolbars or
 * commandbars such as "File, View, ...". It is only poosible to
 * customize your own toolbars or commandbars.
 * \author Werner Mayer
 */
class DlgCustomKeyboardImp : public CustomizeActionPage, public Ui_DlgCustomKeyboard
{
    Q_OBJECT;

public:
    DlgCustomKeyboardImp( QWidget* parent = 0 );
    ~DlgCustomKeyboardImp();

protected Q_SLOTS:
    void on_categoryBox_activated(int index);
    //void on_listBoxCommands_highlighted( const QString& );
    void on_buttonAssign_clicked();
    void on_buttonReset_clicked();
    void on_buttonResetAll_clicked();
    void on_editShortcut_textChanged(const QString&);
    void onAddMacroAction(const QString&);
    void onRemoveMacroAction(const QString&);

private:
    //QMap<QString, QString> _cmdGroups;
    //QMap<QString, QMap<QString, CommandBase*> > _groupCommands;
};

} // namespace Dialog
} // namespace Gui

#endif // GUI_DIALOG_DLGKEYBOARD_IMP_H
