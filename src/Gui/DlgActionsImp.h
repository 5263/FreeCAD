/***************************************************************************
 *   Copyright (c) 2004 Werner Mayer <werner.wm.mayer@gmx.de>              *
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


#ifndef GUI_DIALOG_DLGACTIONS_IMP_H
#define GUI_DIALOG_DLGACTIONS_IMP_H

#include "ui_DlgActions.h"
#include "PropertyPage.h"


namespace Gui {
namespace Dialog {

/** This class implements the creation of user defined actions executing a recorded macro.
 * It is possible to use these actions in own toolbars or commandbars.
 * @see Gui::Action
 * @see MacroCommand
 * @see Command
 * \author Werner Mayer
 */
class DlgCustomActionsImp : public CustomizeActionPage, public Ui_DlgCustomActions
{ 
Q_OBJECT

public:
  DlgCustomActionsImp( QWidget* parent = 0 );
  ~DlgCustomActionsImp();

Q_SIGNALS:
  void addMacroAction( const QString& );
  void removeMacroAction( const QString& );
  void replaceMacroAction( const QString& );

protected:
  /** Trigger for reparent event. */
  bool event(QEvent* e);
  void showEvent(QShowEvent* e);

protected Q_SLOTS:
  /** Enables/disables buttons for deletion */
  void on_listBoxActions_highlighted( Q3ListBoxItem *i );
  /** Opens a iconview to select a pixmap */
  void on_buttonChoosePixmap_clicked();
  /** Adds a custom action */
  void on_buttonAddAction_clicked();
  /** Deletes a custom action */
  void on_buttonRemoveAction_clicked();
  /** Shows the setup of the action */
  void on_buttonReplaceAction_clicked();
  void onAddMacroAction(const QString&);
  void onRemoveMacroAction(const QString&);

private:
  /** Shows all actions and their pixmaps if available  */
  void showActions();
   /** Name for the new created action */
  QString newActionName();

private:
  bool bShown; /**< For internal use only*/
  QString m_sPixmap; /**< Name of the specified pixmap */
};

} // namespace Dialog
} // namespace Gui

#endif // GUI_DIALOG_DLGACTIONS_IMP_H
