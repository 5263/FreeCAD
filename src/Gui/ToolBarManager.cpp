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


#include "PreCompiled.h"

#ifndef _PreComp_
# include <qpopupmenu.h>
#endif

#include "ToolBarManager.h"
#include "Application.h"
#include "Command.h"

using namespace Gui;

ToolBarItem::ToolBarItem()
{
}

ToolBarItem::ToolBarItem( ToolBarItem* item )
{
  if ( item )
    item->appendItem( this );
}

ToolBarItem::~ToolBarItem()
{
  ToolBarItem* item;
  for ( item = _items.first(); item; item = _items.next() )
  {
    delete item;
  }

  _items.clear();
}

void ToolBarItem::setCommand( const QString& name )
{
  _name = name;
}

QString ToolBarItem::command() const
{
  return _name;
}

bool ToolBarItem::hasItems() const
{
  return _items.count() > 0;
}

ToolBarItem* ToolBarItem::findItem( const QString& name )
{
  if ( _name == name )
  {
    return this;
  }
  else
  {
    ToolBarItem* item;
    for ( item = _items.first(); item; item = _items.next() )
    {
      if ( item->_name == name )
        return item;
    }
  }

  return 0;
}

void ToolBarItem::appendItem( const ToolBarItem* item )
{
  _items.append( item );
}

bool ToolBarItem::insertItem( const ToolBarItem* before, const ToolBarItem* item)
{
  int pos = _items.find( before );
  if ( pos > -1 )
  {
    return _items.insert( pos, item );
  }
  else
    return false;
}

void ToolBarItem::removeItem( const ToolBarItem* item )
{
  _items.remove( item );
}

ToolBarItem& ToolBarItem::operator<< ( const ToolBarItem* item )
{
  appendItem(item);
  return *this;
}

ToolBarItem& ToolBarItem::operator<< ( const QString& command )
{
  ToolBarItem* item = new ToolBarItem(this);
  item->setCommand( command );;
  return *this;
}

QPtrList<ToolBarItem> ToolBarItem::getItems() const
{
  return _items;
}

// -----------------------------------------------------------

ToolBarManager* ToolBarManager::_instance=0;

ToolBarManager* ToolBarManager::getInstance()
{
  if ( !_instance )
    _instance = new ToolBarManager;
  return _instance;
}

ToolBarManager::ToolBarManager()
{
}

ToolBarManager::~ToolBarManager()
{
}

void ToolBarManager::setup( ToolBarItem* toolBar ) const
{
  if ( !toolBar )
    return; // empty menu bar

  QPtrList<QToolBar> tbs;
  QPtrList<QDockWindow> dws = ApplicationWindow::Instance->dockWindows();
  QDockWindow* dw;
  for ( dw = dws.first(); dw; dw = dws.next() )
  {
    if ( dw->inherits("QToolBar") )
    {
      if ( toolBar->findItem( dw->name() ) )
      {
        tbs.append( reinterpret_cast<QToolBar*>(dw) );
      }
      else
      {
//        ApplicationWindow::Instance->removeDockWindow( dw );
//        delete dw;
      }
    }
  }

  CommandManager& mgr = ApplicationWindow::Instance->commandManager();
  QPtrList<ToolBarItem> items = toolBar->getItems();

  ToolBarItem* item;
  for ( item = items.first(); item; item = items.next() )
  {
    QToolBar* bar;
    for ( bar = tbs.first(); bar; bar = tbs.next() )
    {
      if ( bar->name() == item->command() )
        break;
    }

    if ( !bar )
    {
      bar = new QToolBar( item->command(), ApplicationWindow::Instance, ApplicationWindow::Instance, false, item->command().latin1() );
      ApplicationWindow::Instance->addDockWindow( bar );

      QPtrList<ToolBarItem> subitems = item->getItems();
      ToolBarItem* subitem;
      for ( subitem = subitems.first(); subitem; subitem = subitems.next() )
      {
        if ( subitem->command() == "Separator" )
          bar->addSeparator();
        else
          mgr.addTo( subitem->command().latin1(), bar );
      }
    }
  }
}
