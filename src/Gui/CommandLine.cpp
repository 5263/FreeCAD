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


#include "PreCompiled.h"

#ifndef _PreComp_
# include <qcursor.h>
#endif

#include "CommandLine.h"
#include "Application.h"
#include "PrefWidgets.h"
#include "Command.h"
#include "../Base/Interpreter.h"
#include "../Base/Console.h"
#include "../Base/Exception.h"

using Base::Interpreter;
using namespace Gui;

ConsoleValidator::ConsoleValidator ( QWidget * parent, const char * name )
: QValidator(parent, name)
{
}

ConsoleValidator::~ConsoleValidator ()
{
}

/**
 * Validates the user input. The string \a txt should not be empty.
 */
QValidator::State ConsoleValidator::validate ( QString & txt, int & i) const
{
  if (txt.isEmpty())
    return Intermediate;
  else
    return Acceptable;
}

void ConsoleValidator::fixup ( QString & txt) const
{
}

// --------------------------------------------------------------------

/**
 * Constructs the command line. It's possible to run Python scripts from there;
 * it's also possible to drag and drop the command name from menus.
 */
CommandLineBase::CommandLineBase(void)
: QComboBox(true, NULL), FCWindowParameter("command line")
{
  // run commands
  _astrRunCmds.push_back("run");
  _astrRunCmds.push_back("script");
  _astrRunCmds.push_back("start");
  _astrRunCmds.push_back("exec");
  _astrRunCmds.push_back("execute");
  _astrRunCmds.push_back("execfile");

  loadHistory();
  setMaximumWidth(400);
  setMinimumWidth(200);
  setAutoCompletion ( true );
  setValidator(new ConsoleValidator(this));
  setAcceptDrops(true);

  connect(lineEdit(), SIGNAL(returnPressed ()), this, SLOT(onLaunchCommand()));
}

/**
 * Destroys the command line.
 */
CommandLineBase::~CommandLineBase(void)
{
  saveHistory();
}

/** 
 * Saves the history in the preferences.
 */
void CommandLineBase::saveHistory()
{
  // write the recent commands into file
  FCParameterGrp::handle hGrp = GetApplication().GetSystemParameter().
          GetGroup("BaseApp")->GetGroup("WindowSettings");
  int iMaxCnt = hGrp->GetInt("SizeCmdLine", 20);

  FCParameterGrp::handle hCmdGrp = GetWindowParameter()->GetGroup("CommandList");

  // copy from list box first
  std::list<std::string> alCmdList;
  for (int ii=0; ii < count(); ii++)
    alCmdList.push_back(text(ii).latin1());
  while ( int(alCmdList.size()) > iMaxCnt )
    alCmdList.erase( alCmdList.begin() );

  long i=0;
  for (std::list<std::string>::iterator it = alCmdList.begin(); it != alCmdList.end(); ++it, i++)
  {
    char szBuf[200];
    sprintf(szBuf, "Command %ld", i);
    hCmdGrp->SetASCII(szBuf, it->c_str());
  }
}

/** 
 * Loads the history from the preferences.
 */
void CommandLineBase::loadHistory()
{
  // get the recent commands
  FCParameterGrp::handle hCmdGrp = GetWindowParameter()->GetGroup("CommandList");
  std::vector<std::string> cmd = hCmdGrp->GetASCIIs("Command");

  int i=0;
  for (std::vector<std::string>::iterator it = cmd.begin(); it != cmd.end(); ++it, i++)
  {
    try
    {
      insertItem(it->c_str());
    }
    catch(...)
    {
      Base::Console().Error("Invalid entry found\n");
    }
  }

  lineEdit()->clear();
}

/** 
 * Launches the command.
 */
void CommandLineBase::onLaunchCommand()
{
  // launch the python command
  try
  {
    bool flag = false;
    QString cmd = text(currentItem());
    for (std::vector<std::string>::iterator it = _astrRunCmds.begin(); it != _astrRunCmds.end(); ++it)
    {
      // if one of the run commands is used
      if (cmd.lower().startsWith(it->c_str()))
      {
        cmd = cmd.right(cmd.length() - (it->length()+1));
        Interpreter().LaunchFile(cmd.latin1());
        flag = true;
        break;
      }
    }

    if (!flag)
      Interpreter().Launch(text(currentItem()).latin1());
  }
  catch (const FCException& rclE)
  {
    QString txt = rclE.what();
    QString err = QString("'%1' is not defined!").arg(text(currentItem()));
    QMessageBox::warning(this, txt, err);
  }
#ifndef FC_DEBUG
  catch (...)
  {
    QMessageBox::critical(this, tr("Error"), tr("A really nesty error occurred in the running script"));
  }
#endif

  // hold the focus
  setFocus();
  // and clear the command line for the next command
  lineEdit()->clear();
}

/** 
 * Clears the history.
 */
void CommandLineBase::onClearHistory()
{
  clear();
}

/** 
 * Shows the history.
 */
void CommandLineBase::onShowHistory()
{
  popup();
}

void CommandLineBase::reparent(QWidget* parent)
{
  _pcSingleton->QComboBox::reparent(parent, QPoint());
}

// Singleton stuff
CommandLineBase * CommandLineBase::_pcSingleton = 0;


void CommandLineBase::Destruct(void)
{
  // not initialized or double destruct!
  assert(_pcSingleton);
  delete _pcSingleton;
}

CommandLineBase & CommandLineBase::Instance(void)
{
  // not initialized?
  if(!_pcSingleton)
  {
    _pcSingleton = new CommandLineBase;
  }

  return *_pcSingleton;
}

/**
 * Scrolls through the items.
 */
void CommandLineBase::wheelEvent ( QWheelEvent * e )
{
  if (count() < 1) return;
  int delta = e->delta() > 0 ? 1 : -1;
  int iCur = (currentItem() + delta) % count();
  if (iCur < 0)  iCur = count() - 1;
  setCurrentItem(iCur);
}

/**
 * Changes the current item.
 */
void CommandLineBase::keyPressEvent ( QKeyEvent * e )
{
  switch (e->key())
  {
    case Key_Up:
    {
      // show last command
      if (currentText().isEmpty() && count() > 0)
      {
        int item = currentItem();
        setCurrentItem(item);
        break;
      }
    }
    default:
      QComboBox::keyPressEvent(e);
  }
}

/**
 * Shows up the context menu.
 */
void CommandLineBase::contextMenuEvent ( QContextMenuEvent * e )
{
  // context menu
  QPopupMenu popup;
  int iOpen  = popup.insertItem (tr( "Open Console" ), this,  SLOT(onShowHistory ())); 
  int iClear = popup.insertItem (tr( "Clear Console" ), this, SLOT(onClearHistory())); 
  popup.setItemEnabled(iOpen,  count() > 0);
  popup.setItemEnabled(iClear, count() > 0);

  popup.exec(QCursor::pos());
}

/**
 * Drops the event \a e and writes the right Python command.
 */
void CommandLineBase::dropEvent ( QDropEvent      * e )
{
  QString action;
  if (FCActionDrag::decode(e, action))
  {
    if (!action.isEmpty())
    {
      FCActionDrag::actions.clear();
      FCCommandManager& rclMan = ApplicationWindow::Instance->GetCommandManager();
      FCCommand* pCmd = rclMan.GetCommandByName(action.latin1());

      if (pCmd)
      {
        lineEdit()->setText(QString("Gui.RunCommand(\"%1\")").arg(pCmd->GetName()));
      }
    }
  }

  QComboBox::dropEvent(e);
}

void CommandLineBase::dragEnterEvent ( QDragEnterEvent * e )
{
  if (FCActionDrag::canDecode(e))
    e->accept(true);
  else
    QComboBox::dragEnterEvent(e);
}

bool CommandLineBase::eventFilter       ( QObject* o, QEvent* e )
{
  if ( o == lineEdit() )
  {
    // get the editor's mouse events
    switch (e->type())
    {
      case QEvent::DragEnter:
        // divert the event to combo box (itself)
        if (acceptDrops())
          dragEnterEvent ((QDragEnterEvent*)e);
        break;
      case QEvent::Drop:
        if (acceptDrops())
          dropEvent ((QDropEvent*)e);
        break;
      default:
        break;
    }
  }

  return QComboBox::eventFilter(o, e);
}

void CommandLineBase::show()
{
  FCParameterGrp::handle hGrp = GetApplication().GetSystemParameter().
    GetGroup("BaseApp")->GetGroup("WindowSettings");
  bool show = (hGrp->GetBool("ShowCmdLine", true));

  if ( !show )
    QComboBox::hide();
  else
    QComboBox::show();
}

#include "moc_CommandLine.cpp"
