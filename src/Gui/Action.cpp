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
# include <boost/signals.hpp>
# include <boost/bind.hpp>
#endif

#include "Action.h"
#include "Application.h"
#include "Command.h"
#include "DlgUndoRedo.h"
#include "MainWindow.h"
#include "WhatsThis.h"
#include "Workbench.h"
#include "WorkbenchManager.h"

#include <Base/Exception.h>

using namespace Gui;
using namespace Gui::Dialog;

/**
 * Constructs an action called \a name with parent \a parent. It also stores a pointer
 * to the command object.
 */
Action::Action ( Command* pcCmd,QObject * parent )
  : QObject(parent), _action(0), _pcCmd(pcCmd)
{
  _action = new QAction( this );
  _action->setObjectName(QString::fromAscii(_pcCmd->getName()));
  connect(_action, SIGNAL(triggered(bool)), this, SLOT(onActivated()));
}

Action::~Action()
{ 
  delete _action;
}

/**
 * Adds this action to widget \a w.
 */
void Action::addTo(QWidget *w)
{
  w->addAction(_action);
}

/**
 * Activates the command.
 */
void Action::onActivated () 
{
    _pcCmd->invoke(0);
}

/**
 * Sets whether the command is toggled.
 */
void Action::onToggled ( bool b)
{
  _pcCmd->invoke( b ? 1 : 0 );
} 

void Action::setCheckable ( bool b )
{
  _action->setCheckable(b);
  if ( b ) {
    disconnect(_action, SIGNAL(triggered(bool)), this, SLOT(onActivated()));
    connect(_action, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
  } else {
    connect(_action, SIGNAL(triggered(bool)), this, SLOT(onActivated()));
    disconnect(_action, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
  }
}

void Action::setChecked ( bool b )
{
  _action->setChecked(b);
}

bool Action::isChecked() const
{
  return _action->isChecked();
}

/**
 * Sets whether the action is enabled.
 */
void Action::setEnabled ( bool b) 
{
  _action->setEnabled(b);
}

void Action::setVisible ( bool b) 
{
  _action->setVisible( b );
}

void Action::setShortcut ( const QKeySequence & key )
{
  _action->setShortcut(key);
}

QKeySequence Action::shortcut() const
{
  return _action->shortcut();
}

void Action::setIcon ( const QIcon & icon)
{
  _action->setIcon(icon);
}

void Action::setStatusTip ( const QString & s)
{
  _action->setStatusTip(s);
}

QString Action::statusTip() const
{
  return _action->statusTip();
}

void Action::setText ( const QString & s)
{
  _action->setText(s);
}

QString Action::text() const
{
  return _action->text();
}

void Action::setToolTip ( const QString & s)
{
  _action->setToolTip(s);
}
  
QString Action::toolTip() const
{
  return _action->toolTip();
}

void Action::setWhatsThis ( const QString & s)
{
  _action->setWhatsThis(s);
}

QString Action::whatsThis() const
{
  return _action->whatsThis();
}

// --------------------------------------------------------------------

/**
 * Constructs an action called \a name with parent \a parent. It also stores a pointer
 * to the command object.
 */
ActionGroup::ActionGroup ( Command* pcCmd,QObject * parent)
  : Action(pcCmd, parent), _group(0), _dropDown(false)
{
  _group = new QActionGroup( this );
  connect(_group, SIGNAL(triggered(QAction*)), this, SLOT(onActivated (QAction*)));
}

ActionGroup::~ActionGroup()
{ 
  delete _group;
}

/**
 * Adds this action to widget \a w.
 */
void ActionGroup::addTo(QWidget *w)
{
  // When adding an action that has defined a menu then shortcuts
  // of the menu actions don't work. To make this working we must 
  // set the menu explicitly. This means calling QAction::setMenu()
  // and adding this action to the widget doesn't work.
  if ( _dropDown ) {
    if (w->inherits("QMenu")) {
      QMenu* menu = qobject_cast<QMenu*>(w);
      menu = menu->addMenu(_action->text());
      menu->addActions(_group->actions());
    } else if (w->inherits("QToolBar")) {
      w->addAction(_action);
      QToolButton* tb = w->findChildren<QToolButton*>().last();
      tb->setPopupMode(QToolButton::MenuButtonPopup);
      tb->addActions(_group->actions());
    } else {
      w->addActions(_group->actions()); // no drop-down 
    }
  } else {
    w->addActions(_group->actions());
  }
}

void ActionGroup::setEnabled( bool b )
{
  Action::setEnabled(b);
  _group->setEnabled(b);
}

void ActionGroup::setVisible( bool b )
{
  Action::setVisible(b);
  _group->setVisible(b);
}

QAction* ActionGroup::addAction(const QString& text)
{
  int index = _group->actions().size();
  QAction* action = _group->addAction(text);
  action->setData(QVariant(index));
  return action;
}

QList<QAction*> ActionGroup::actions() const
{
  return _group->actions();
}

int ActionGroup::checkedAction() const
{
  QAction* checked = _group->checkedAction();
  return checked ? checked->data().toInt() : -1;
}

void ActionGroup::setCheckedAction(int i)
{
  _group->actions()[i]->setChecked(true);
}

/**
 * Activates the command.
 */
void ActionGroup::onActivated (QAction* a) 
{
    _pcCmd->invoke(a->data().toInt());
}

// --------------------------------------------------------------------

namespace Gui {

/**
 * The WorkbenchActionEvent class is used to send an event of which workbench must be activated.
 * We cannot activate the workbench directly as we will destroy the widget that emits the signal.
 * @author Werner Mayer
 */
class WorkbenchActionEvent : public QEvent
{
public:
  WorkbenchActionEvent(QAction* a)
    : QEvent(QEvent::User), act(a)
  { }
  ~WorkbenchActionEvent()
  { }
  QAction* action() const
  { return act; }
private:
  QAction* act;
};
}

WorkbenchComboBox::WorkbenchComboBox(WorkbenchGroup* wb, QWidget* parent) : QComboBox(parent), group(wb)
{
  connect(this, SIGNAL(activated(int)), this, SLOT(onActivated(int)));
  connect(getMainWindow(), SIGNAL(workbenchActivated(const QString&)), 
          this, SLOT(onWorkbenchActivated(const QString&)));
}

WorkbenchComboBox::~WorkbenchComboBox()
{
}

void WorkbenchComboBox::actionEvent ( QActionEvent* e )
{
    QAction *action = e->action();
    switch (e->type()) {
    case QEvent::ActionAdded:
        {
            if (action->isVisible()) {
                QIcon icon = action->icon();
                if (icon.isNull())
                    this->addItem(action->text(), action->data());
                else
                    this->addItem(icon, action->text(), action->data());
                if (action->isChecked())
                    this->setCurrentIndex(action->data().toInt());
            }
            break;
        }
    case QEvent::ActionChanged:
        {
            QVariant data = action->data();
            int index = this->findData(data);
            // added a workbench
            if (index < 0 && action->isVisible()) {
                QString text = action->text();
                QIcon icon = action->icon();
                if (icon.isNull())
                    this->addItem(action->text(), data);
                else
                    this->addItem(icon, action->text(), data);
            }
            // removed a workbench
            else if (index >=0 && !action->isVisible()) {
                this->removeItem(index);
            }
            break;
        }
    case QEvent::ActionRemoved:
        {
            //Nothing needs to be done
            break;
        }
    default:
        break;
    }
}

void WorkbenchComboBox::onActivated(int i)
{
    // Send the event to the workbench group to delay the destruction of the emitting widget.
    int index = itemData(i).toInt();
    WorkbenchActionEvent* ev = new WorkbenchActionEvent(this->actions()[index]);
    QApplication::postEvent(this->group, ev);
}

void WorkbenchComboBox::onActivated(QAction* action)
{
    // set the according item to the action
    QVariant data = action->data();
    int index = this->findData(data);
    setCurrentIndex(index);
}

void WorkbenchComboBox::onWorkbenchActivated(const QString& name)
{
    QList<QAction*> a = actions();
    for (QList<QAction*>::Iterator it = a.begin(); it != a.end(); ++it) {
        if ((*it)->objectName() == name) {
            (*it)->trigger();
            break;
        }
    }
}

WorkbenchGroup::WorkbenchGroup (  Command* pcCmd, QObject * parent )
  : ActionGroup( pcCmd, parent )
{
    for (int i=0; i<50; i++) {
        QAction* action = _group->addAction(QLatin1String(""));
        action->setVisible(false);
        action->setCheckable(true);
        action->setData(QVariant(i)); // set the index
    }

    Application::Instance->signalActivateWorkbench.connect(boost::bind(&WorkbenchGroup::slotActivateWorkbench, this, _1));
    Application::Instance->signalAddWorkbench.connect(boost::bind(&WorkbenchGroup::slotAddWorkbench, this, _1));
    Application::Instance->signalRemoveWorkbench.connect(boost::bind(&WorkbenchGroup::slotRemoveWorkbench, this, _1));
}

WorkbenchGroup::~WorkbenchGroup()
{
}

void WorkbenchGroup::addTo(QWidget *w)
{
    refreshWorkbenchList();
    if (w->inherits("QToolBar")) {
        QToolBar* bar = qobject_cast<QToolBar*>(w);
        QComboBox* box = new WorkbenchComboBox(this, w);
        box->setToolTip(_action->toolTip());
        box->setStatusTip(_action->statusTip());
        box->setWhatsThis(_action->whatsThis());
        box->addActions(_group->actions());
        connect(_group, SIGNAL(triggered(QAction*)), box, SLOT(onActivated (QAction*)));
        bar->addWidget(box);
    }
    else if (w->inherits("QMenu")) {
        QMenu* menu = qobject_cast<QMenu*>(w);
        menu = menu->addMenu(_action->text());
        menu->addActions(_group->actions());
    }
}

void WorkbenchGroup::refreshWorkbenchList()
{
    QString active = QString::fromAscii(WorkbenchManager::instance()->active()->name().c_str());
    QStringList items = Application::Instance->workbenches();
    
    QList<QAction*> workbenches = _group->actions();
    int numWorkbenches = std::min<int>(workbenches.count(), items.count());

    // sort by workbench menu text
    QMap<QString, QString> menuText;
    for (int index = 0; index < numWorkbenches; index++) {
        QString text = Application::Instance->workbenchMenuText(items[index]);
        menuText[text] = items[index];
    }

    int i=0;
    for (QMap<QString, QString>::Iterator it = menuText.begin(); it != menuText.end(); ++it, i++) {
        QPixmap px = Application::Instance->workbenchIcon(it.value());
        QString tip = Application::Instance->workbenchToolTip(it.value());
        workbenches[i]->setObjectName(it.value());
        workbenches[i]->setIcon(px);
        workbenches[i]->setText(it.key());
        workbenches[i]->setToolTip(tip);
        workbenches[i]->setStatusTip(tr("Select the '%1' workbench").arg(it.key()));
        workbenches[i]->setVisible(true);
        if ( items[i] == active )
        workbenches[i]->setChecked(true);
    }

    // if less workbenches than actions
    for (int index = numWorkbenches; index < workbenches.count(); index++) {
        workbenches[i]->setObjectName(QString());
        workbenches[i]->setIcon(QIcon());
        workbenches[i]->setText(QString());
        workbenches[index]->setVisible(false);
    }
}

void WorkbenchGroup::customEvent( QEvent* e )
{
    if (e->type() == QEvent::User) {
        Gui::WorkbenchActionEvent* ce = (Gui::WorkbenchActionEvent*)e;
        ce->action()->trigger();
    }
}

void WorkbenchGroup::slotActivateWorkbench(const char* /*name*/)
{
}

void WorkbenchGroup::slotAddWorkbench(const char* name)
{
    QList<QAction*> workbenches = _group->actions();
    for (QList<QAction*>::Iterator it = workbenches.begin(); it != workbenches.end(); ++it) {
        if (!(*it)->isVisible()) {
            QString wb = QString::fromAscii(name);
            QPixmap px = Application::Instance->workbenchIcon(wb);
            QString text = Application::Instance->workbenchMenuText(wb);
            QString tip = Application::Instance->workbenchToolTip(wb);
            (*it)->setIcon(px);
            (*it)->setObjectName(wb);
            (*it)->setText(text);
            (*it)->setToolTip(tip);
            (*it)->setStatusTip(tr("Select the '%1' workbench").arg(wb));
            (*it)->setVisible(true); // do this at last
            break;
        }
    }
}

void WorkbenchGroup::slotRemoveWorkbench(const char* name)
{
    QString workbench = QString::fromAscii(name);
    QList<QAction*> workbenches = _group->actions();
    for (QList<QAction*>::Iterator it = workbenches.begin(); it != workbenches.end(); ++it) {
        if ((*it)->objectName() == workbench) {
            (*it)->setObjectName(QString());
            (*it)->setIcon(QIcon());
            (*it)->setText(QString());
            (*it)->setToolTip(QString());
            (*it)->setStatusTip(QString());
            (*it)->setVisible(false); // do this at last
            break;
        }
    }
}

// --------------------------------------------------------------------

RecentFilesAction::RecentFilesAction ( Command* pcCmd, QObject * parent )
  : ActionGroup( pcCmd, parent ), visibleItems(4), maximumItems(20)
{
    restore();
}

RecentFilesAction::~RecentFilesAction()
{
    save();
}

/** Adds the new item to the recent files. */
void RecentFilesAction::appendFile(const QString& filename)
{
    // restore the list of recent files
    QStringList files = this->files();

    // if already inside remove and prepend it
    files.removeAll(filename);
    files.prepend(filename);
    setFiles(files);
}

/**
 * Set the list of recent files. For each item an action object is
 * created and added to this action group. 
 */
void RecentFilesAction::setFiles( const QStringList& files )
{
    QList<QAction*> recentFiles = _group->actions();

    int numRecentFiles = std::min<int>(recentFiles.count(), files.count());
    for ( int index = 0; index < numRecentFiles; index++ ) {
        QFileInfo fi(files[index]);
        recentFiles[index]->setText(QString::fromAscii("&%1 %2").arg(index+1).arg(fi.fileName()));
        recentFiles[index]->setStatusTip(tr("Open file %1").arg(files[index]));
        recentFiles[index]->setToolTip(files[index]); // set the full name that we need later for saving
        recentFiles[index]->setData(QVariant(index));
        recentFiles[index]->setVisible(true);
    }

    // if less file names than actions
    numRecentFiles = std::min<int>(numRecentFiles, this->visibleItems);
    for (int index = numRecentFiles; index < recentFiles.count(); index++)
        recentFiles[index]->setVisible(false);
}

/**
 * Returns the list of defined recent files.
 */
QStringList RecentFilesAction::files() const
{
    QStringList files;
    QList<QAction*> recentFiles = _group->actions();
    for ( int index = 0; index < recentFiles.count(); index++ ) {
        QString file = recentFiles[index]->toolTip();
        if (file.isEmpty())
            break;
        files.append(file);
    }

    return files;
}

void RecentFilesAction::activateFile(int id)
{
    // restore the list of recent files
    QStringList files = this->files();
    if (id < 0 || id >= files.count())
        return; // no valid item

    QString filename = files[id];
    QFileInfo fi(filename);
    if (!fi.exists() || !fi.isFile()) {
        QMessageBox::critical(getMainWindow(), tr("File not found"), tr("The file '%1' cannot be opened.").arg(filename));
        files.removeAll(filename);
        setFiles(files);
    } else {
        // invokes appendFile()
        Application::Instance->open(filename.toUtf8());
    }
}

void RecentFilesAction::resizeList(int size)
{
    this->visibleItems = size;
    int diff = this->visibleItems - this->maximumItems;
    // create new items if needed
    for (int i=0; i<diff; i++)
        _group->addAction(QLatin1String(""))->setVisible(false);
    setFiles(files());
}

/** Loads all recent files from the preferences. */
void RecentFilesAction::restore()
{
    ParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("Preferences");
    if (hGrp->HasGroup("RecentFiles")) {
        hGrp = hGrp->GetGroup("RecentFiles");
        // we want at least 20 items but we do only show the number of files
        // that is defined in user parameters
        this->visibleItems = hGrp->GetInt("RecentFiles", this->visibleItems);
    }

    int count = std::max<int>(this->maximumItems, this->visibleItems);
    for (int i=0; i<count; i++)
        _group->addAction(QLatin1String(""))->setVisible(false);
    std::vector<std::string> MRU = hGrp->GetASCIIs("MRU");
    QStringList files;
    for (std::vector<std::string>::iterator it = MRU.begin(); it!=MRU.end();++it)
        files.append(QString::fromUtf8(it->c_str()));
    setFiles(files);
}

/** Saves all recent files to the preferences. */
void RecentFilesAction::save()
{
    ParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")
                                ->GetGroup("Preferences")->GetGroup("RecentFiles");
    int count = hGrp->GetInt("RecentFiles", this->visibleItems); // save number of files
    hGrp->Clear();
    hGrp->SetInt("RecentFiles", count); // restore

    // count all set items
    QList<QAction*> recentFiles = _group->actions();
    for ( int index = 0; index < recentFiles.count(); index++ ) {
        QString key = QString::fromAscii("MRU%1").arg(index);
        QString value = recentFiles[index]->toolTip();
        if (value.isEmpty())
            break;
        hGrp->SetASCII(key.toAscii(), value.toUtf8());
    }
}

// --------------------------------------------------------------------

UndoAction::UndoAction ( Command* pcCmd,QObject * parent )
  : Action(pcCmd, parent)
{
  _toolAction = new QAction(this);
  _toolAction->setMenu(new UndoDialog());
  connect(_toolAction, SIGNAL(triggered(bool)), this, SLOT(onActivated()));
}

UndoAction::~UndoAction()
{
  QMenu* menu = _toolAction->menu();
  delete menu;
  delete _toolAction;
}

void UndoAction::addTo ( QWidget * w )
{
  if (w->inherits("QToolBar")) {
    _toolAction->setText(_action->text());
    _toolAction->setToolTip(_action->toolTip());
    _toolAction->setStatusTip(_action->statusTip());
    _toolAction->setWhatsThis(_action->whatsThis());
    _toolAction->setShortcut(_action->shortcut());
    _toolAction->setIcon(_action->icon());
    w->addAction(_toolAction);
  } else {
    w->addAction(_action);
  }
}

void UndoAction::setEnabled  ( bool b )
{
  Action::setEnabled(b);
  _toolAction->setEnabled(b);
}

void UndoAction::setVisible ( bool b )
{
  Action::setVisible(b);
  _toolAction->setVisible(b);
}

// --------------------------------------------------------------------

RedoAction::RedoAction ( Command* pcCmd,QObject * parent )
  : Action(pcCmd, parent)
{
  _toolAction = new QAction(this);
  _toolAction->setMenu(new RedoDialog());
  connect(_toolAction, SIGNAL(triggered(bool)), this, SLOT(onActivated()));
}

RedoAction::~RedoAction()
{
  QMenu* menu = _toolAction->menu();
  delete menu;
  delete _toolAction;
}

void RedoAction::addTo ( QWidget * w )
{
  if (w->inherits("QToolBar")) {
    _toolAction->setText(_action->text());
    _toolAction->setToolTip(_action->toolTip());
    _toolAction->setStatusTip(_action->statusTip());
    _toolAction->setWhatsThis(_action->whatsThis());
    _toolAction->setShortcut(_action->shortcut());
    _toolAction->setIcon(_action->icon());
    w->addAction(_toolAction);
  } else {
    w->addAction(_action);
  }
}

void RedoAction::setEnabled  ( bool b )
{
  Action::setEnabled(b);
  _toolAction->setEnabled(b);
}

void RedoAction::setVisible ( bool b )
{
  Action::setVisible(b);
  _toolAction->setVisible(b);
}

// --------------------------------------------------------------------

DockWidgetAction::DockWidgetAction ( Command* pcCmd, QObject * parent )
  : Action(pcCmd, parent), _menu(0)
{
}

DockWidgetAction::~DockWidgetAction()
{
  delete _menu;
}

void DockWidgetAction::addTo ( QWidget * w )
{
    if (!_menu) {
      _menu = new QMenu();
      _action->setMenu(_menu);
      connect(_menu, SIGNAL(aboutToShow()), getMainWindow(), SLOT(onDockWindowMenuAboutToShow()));
    }
    
    w->addAction(_action);
}

// --------------------------------------------------------------------

ToolBarAction::ToolBarAction ( Command* pcCmd, QObject * parent )
  : Action(pcCmd, parent), _menu(0)
{
}

ToolBarAction::~ToolBarAction()
{
  delete _menu;
}

void ToolBarAction::addTo ( QWidget * w )
{
    if (!_menu) {
      _menu = new QMenu();
      _action->setMenu(_menu);
      connect(_menu, SIGNAL(aboutToShow()), getMainWindow(), SLOT(onToolBarMenuAboutToShow()));
    }
    
    w->addAction(_action);
}

// --------------------------------------------------------------------

WindowAction::WindowAction ( Command* pcCmd, QObject * parent )
  : ActionGroup(pcCmd, parent), _menu(0)
{
}

WindowAction::~WindowAction()
{
}

void WindowAction::addTo ( QWidget * w )
{
  QMenu* menu = qobject_cast<QMenu*>(w);
  if ( !menu ) {
    if (!_menu) {
      _menu = new QMenu();
      _action->setMenu(_menu);
      _menu->addActions(_group->actions());
      connect( _menu, SIGNAL( aboutToShow()), getMainWindow(), SLOT( onWindowsMenuAboutToShow() ) );
    }
    
    w->addAction(_action);
  } else {
    menu->addActions(_group->actions());
    connect( menu, SIGNAL( aboutToShow()), getMainWindow(), SLOT( onWindowsMenuAboutToShow() ) );
  }
}

#include "moc_Action.cpp"
