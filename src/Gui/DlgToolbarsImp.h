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


#ifndef GUI_DIALOG_DLGTOOLBARS_IMP_H
#define GUI_DIALOG_DLGTOOLBARS_IMP_H

#include "ui_DlgToolbars.h"
#include "PropertyPage.h"

namespace Gui {
class Command;
class CustomToolBar;
class ToolBarItem;

namespace Dialog {

/** This class implements the creation of user defined toolbars.
 * In the left panel are shown all command groups with their command objects.
 * If any changeable toolbar was created in the left panel are shown all commands 
 * of the currently edited toolbar, otherwise it is emtpy.
 * All changes to a toolbar is done immediately.
 * 
 * \author Werner Mayer
 */
class DlgCustomToolbars : public CustomizeActionPage, public Ui_DlgCustomToolbars
{ 
  Q_OBJECT

protected:
  DlgCustomToolbars( QWidget* parent = 0 );
  virtual ~DlgCustomToolbars();

protected:
  virtual void refreshActionList();
  virtual void refreshToolBarList();

protected Q_SLOTS:
  void on_toolbarsCombobox_activated(const QString &);
  void on_availableActions_doubleClicked(Q3ListViewItem*);
  void on_moveActionRightButton_clicked();
  void on_moveActionLeftButton_clicked();
  void on_moveActionUpButton_clicked();
  void on_moveActionDownButton_clicked();
  void on_toolbarActions_clicked( Q3ListViewItem *i );
  void on_availableActions_clicked( Q3ListViewItem *i );
  void on_createToolbarButton_clicked();
  void on_deleteToolbarButton_clicked();
  void onAddMacroAction(const QString&);
  void onRemoveMacroAction(const QString&);

private:
  void refreshFullActionList();

protected:
  /** List of all toolbars */
  ToolBarItem* _toolBars;
};

/** This class implements the creation of user defined toolbars.
 * @see DlgCustomToolbars
 * @see DlgCustomCmdbarsImp
 * \author Werner Mayer
 */
class DlgCustomToolbarsImp : public DlgCustomToolbars
{ 
  Q_OBJECT

public:
  DlgCustomToolbarsImp( QWidget* parent = 0 );
  ~DlgCustomToolbarsImp();
};

} // namespace Dialog
} // namespace Gui

#endif // GUI_DIALOG_DLGTOOLBARS_IMP_H
