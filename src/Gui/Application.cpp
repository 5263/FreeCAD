/***************************************************************************
 *   Copyright (c) 2004 J�rgen Riegel <juergen.riegel@web.de>              *
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
# include <qapplication.h>
# include <qlabel.h>
# include <qmenubar.h>
# include <qstatusbar.h>
# include <qstyle.h>
# include <qstylefactory.h>
# include <qtabbar.h>
# include <qtimer.h>
# include <qvbox.h>
#endif


// FreeCAD Base header
#include "../Base/Console.h"
#include "../Base/Interpreter.h"
#include "../Base/Parameter.h"
#include "../Base/Exception.h"
#include "../Base/EnvMacros.h"
#include "../Base/Factory.h"
#include "../App/Application.h"

#include "Application.h"
#include "Document.h"
#include "View.h"
//#include "CommandStd.h"
//#include "CommandView.h"
#include "../Base/Documentation.h"

#include "Icons/developers.h"
#include "Icons/FCIcon.xpm"
#include "Splashscreen.h"
#include "WidgetFactory.h"
#include "CustomWidgets.h"
#include "Command.h"
#include "Tree.h"
#include "PropertyView.h"

#include "CommandLine.h"
#include "DlgDocTemplatesImp.h"
#include "DlgTipOfTheDayImp.h"
#include "DlgUndoRedo.h"
#include "DlgOnlineHelpImp.h"
#include "ToolBox.h"
#include "HelpView.h"
#include "ReportView.h"
#include "Macro.h"
#include "Themes.h"
#include "ProgressBar.h"
#include "Window.h" 

#include "Inventor/Qt/SoQt.h"

#include "Language/Translator.h"

#include "GuiInitScript.h"

using Base::Console;
using Base::Interpreter;
using namespace Gui;
using namespace Gui::DockWnd;
using Gui::Dialog::DlgOnlineHelpImp;

static ApplicationWindow* stApp;
//static QWorkspace* stWs;


ApplicationWindow* ApplicationWindow::Instance = 0L;

namespace Gui {

// Pimpl class
struct ApplicationWindowP
{
  ApplicationWindowP()
    : toolbars(0L), viewbar(0L), _pcActiveDocument(0L),
    _bIsClosing(false), _bControlButton(false)
  {
    // create the macro manager
    _pcMacroMngr = new MacroManager();
  }

  ~ApplicationWindowP()
  {
    viewbar = 0L;
    delete toolbars;
    delete _pcWidgetMgr;
    delete _pcMacroMngr;
  }

  QPopupMenu* toolbars;
  Gui::CustomPopupMenu* viewbar;
  Gui::CustomPopupMenu* windows;
  QValueList<int> wndIDs;
  std::map<int, QWidget*> mCheckBars;
  /// list of all handled documents
  std::list<Gui::Document*>         lpcDocuments;
  /// list of windows
  /// Active document
  Gui::Document*   _pcActiveDocument;
  Gui::CustomWidgetManager*		 _pcWidgetMgr;
  Gui::DockWindowManager* _pcDockMgr;
  MacroManager*  _pcMacroMngr;
  QLabel *         _pclSizeLabel, *_pclActionLabel;
  ToolBox*        _pcStackBar;
  /// workbench python dictionary
  PyObject*		 _pcWorkbenchDictionary;
  QString			 _cActiveWorkbenchName;
  QTimer *		 _pcActivityTimer; 
  /// List of all registered views
  std::list<Gui::BaseView*>					_LpcViews;
  bool _bIsClosing;
  // store it if the CTRL button is pressed or released
  bool _bControlButton;
  /// Handels all commands 
  CommandManager _cCommandManager;
  QWorkspace* _pWorkspace;
  QTabBar* _tabs;
  QMap<QObject*, QTab*> _tabIds;
};

} // namespace Gui

ApplicationWindow::ApplicationWindow()
    : QMainWindow( 0, "Main window", WDestructiveClose )
{
  FCParameterGrp::handle hPGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp");
  hPGrp = hPGrp->GetGroup("Preferences")->GetGroup("General");

  std::string language = hPGrp->GetASCII("Language", "English");
  Gui::Translator::setLanguage( language.c_str() );
  GetWidgetFactorySupplier();

  // seting up Python binding
  (void) Py_InitModule("FreeCADGui", ApplicationWindow::Methods);

  // init the Inventor subsystem
  SoQt::init(this);

  d = new ApplicationWindowP;
  QVBox* vb = new QVBox( this );
  vb->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
  d->_pWorkspace = new QWorkspace( vb, "workspace" );

  QPixmap backgnd(( const char** ) background );
  d->_pWorkspace->setPaletteBackgroundPixmap( backgnd );
  d->_tabs = new QTabBar( vb, "tabBar" );
  d->_tabs->setShape( QTabBar::RoundedBelow );
  d->_pWorkspace->setScrollBarsEnabled( true );
  setCentralWidget( vb );
  connect( d->_pWorkspace, SIGNAL( windowActivated ( QWidget * ) ), this, SLOT( onWindowActivated( QWidget* ) ) );
  connect( d->_tabs, SIGNAL( selected( int) ), this, SLOT( onTabSelected(int) ) );

//	setCaption( "Qt FreeCAD" );
  setCaption( "FreeCAD" );
  setIcon(QPixmap(FCIcon));

  d->_cActiveWorkbenchName="<none>";

  // global access 
  Instance = this;

  stApp = this;

  // instanciate the workbench dictionary
  d->_pcWorkbenchDictionary = PyDict_New();

    // attach the console observer
  Base::Console().AttacheObserver( new GuiConsoleObserver(this) );


  // setting up the Bitmap manager
//	QString tmpWb = _cActiveWorkbenchName;
//	_cBmpFactory.GetPixmap("Function");
/*
  QDir dir(GetApplication().GetHomePath()); 
  QString root = dir.path();
  GetDocumentationManager().AddProvider(new FCDocProviderDirectory("FCDoc:/"          ,(root + "/Doc/Online\\"   ).latin1()));
  GetDocumentationManager().AddProvider(new FCDocProviderDirectory("FCDoc:/Framework/",(root + "/Doc/FrameWork\\").latin1()));

  std::string test =  GetDocumentationManager().Retrive("FCDoc:/index", Html );

  test =  GetDocumentationManager().Retrive("FCDoc:/Framework/index", Html );
*/

  // labels and progressbar
  d->_pclActionLabel = new QLabel("", statusBar(), "Action");
  d->_pclActionLabel->setFixedWidth(120);
  statusBar()->addWidget(d->_pclActionLabel,0,true);
  statusBar()->addWidget(ProgressBar::instance(),0,true);
  //ProgressBar::Instance().setFixedWidth(200);
  d->_pclSizeLabel = new QLabel("Dimension", statusBar(), "Dimension");
  d->_pclSizeLabel->setFixedWidth(120);
  statusBar()->addWidget(d->_pclSizeLabel,0,true);

  // update gui timer
  d->_pcActivityTimer = new QTimer( this );
    connect( d->_pcActivityTimer, SIGNAL(timeout()),this, SLOT(updateCmdActivity()) );
    d->_pcActivityTimer->start( 300, TRUE );


  // Command Line +++++++++++++++++++++++++++++++++++++++++++++++++++
  CommandLine().reparent(statusBar());
  statusBar()->addWidget(&CommandLine(), 0, true);
  statusBar()->message( tr("Ready"), 2001 );

  // Cmd Button Group +++++++++++++++++++++++++++++++++++++++++++++++
  d->_pcStackBar = new ToolBox(this,"Cmd_Group");
  d->_pcWidgetMgr = new Gui::CustomWidgetManager(commandManager(), d->_pcStackBar);
  d->_pcDockMgr = new Gui::DockWindowManager();
  d->_pcDockMgr->addDockWindow( "Toolbox",d->_pcStackBar, Qt::DockRight );

  // Help View ++++++++++++++++++++++++++++++++++++++++++++++++++++++
  QString home = DlgOnlineHelpImp::getStartpage();
  HelpView* pcHelpView = new HelpView( home, this, "HelpViewer" );
  d->_pcDockMgr->addDockWindow("Help view", pcHelpView, Qt::DockRight );


  // Tree Bar  ++++++++++++++++++++++++++++++++++++++++++++++++++++++	
  TreeView* pcTree = new TreeView(0,this,"Raw_tree");
  pcTree->setMinimumWidth(210);
  d->_pcDockMgr->addDockWindow("Tree view", pcTree, Qt::DockLeft );

  // PropertyView  ++++++++++++++++++++++++++++++++++++++++++++++++++++++	
  PropertyView* pcPropView = new PropertyView(0,0,"PropertyView");
  pcPropView->setMinimumWidth(210);
  d->_pcDockMgr->addDockWindow("Property editor", pcPropView, Qt::DockLeft );

  // Report View
  Gui::DockWnd::ReportView* pcOutput = new Gui::DockWnd::ReportView(this,"ReportView");
  d->_pcDockMgr->addDockWindow("Report View", pcOutput, Qt::DockBottom );

  createStandardOperations();



  // misc stuff
  loadWindowSettings();
  //  resize( 800, 600 );
  //setBackgroundPixmap(QPixmap((const char*)FCBackground));
  //setUsesBigPixmaps (true);

  FCParameterGrp::handle hGrp = App::GetApplication().GetParameterGroupByPath("User parameter:BaseApp/Macro");
/*
  if (hGrp->HasGroup("Macros"))
  {
    hGrp = hGrp->GetGroup("Macros");
    std::vector<FCHandle<FCParameterGrp> > macros = hGrp->GetGroups();
    for (std::vector<FCHandle<FCParameterGrp> >::iterator it = macros.begin(); it!=macros.end(); ++it )
    {
      MacroCommand* pScript = new MacroCommand((*it)->GetGroupName());
      pScript->SetScriptName((*it)->GetASCII("Script").c_str());
      pScript->SetMenuText((*it)->GetASCII("Menu").c_str());
      pScript->SetToolTipText((*it)->GetASCII("Tooltip").c_str());
      pScript->SetWhatsThis((*it)->GetASCII("WhatsThis").c_str());
      pScript->SetStatusTip((*it)->GetASCII("Statustip").c_str());
      if ((*it)->GetASCII("Pixmap", "nix") != "nix") pScript->SetPixmap((*it)->GetASCII("Pixmap").c_str());
        pScript->SetAccel((*it)->GetInt("Accel"));
      d->_cCommandManager.AddCommand(pScript);
    }
  }
*/
  // load recent file list
  hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("Preferences");
  if (hGrp->HasGroup("RecentFiles"))
  {
    hGrp = hGrp->GetGroup("RecentFiles");
    StdCmdMRU* pCmd = dynamic_cast<StdCmdMRU*>(d->_cCommandManager.getCommandByName("Std_MRU"));
    if (pCmd)
    {
      int maxCnt = hGrp->GetInt("RecentFiles", 4);
      pCmd->setMaxCount( maxCnt );
      std::vector<std::string> MRU = hGrp->GetASCIIs("MRU");
      for (std::vector<std::string>::iterator it = MRU.begin(); it!=MRU.end();++it)
      {
        pCmd->addRecentFile( it->c_str() );
      }
    }
  }
}

ApplicationWindow::~ApplicationWindow()
{
  std::vector<Command*> macros = d->_cCommandManager.getModuleCommands("Macro");
  if (macros.size() > 0)
  {
    FCParameterGrp::handle hGrp = App::GetApplication().GetParameterGroupByPath("User parameter:BaseApp/Macro")->GetGroup("Macros");
/*
    for (std::vector<Command*>::iterator it = macros.begin(); it!=macros.end(); ++it )
    {
      MacroCommand* pScript = (MacroCommand*)(*it);
      FCParameterGrp::handle hMacro = hGrp->GetGroup(pScript->GetName());
      hMacro->SetASCII("Script", pScript->GetScriptName());
      hMacro->SetASCII("Menu", pScript->GetMenuText());
      hMacro->SetASCII("Tooltip", pScript->GetToolTipText());
      hMacro->SetASCII("WhatsThis", pScript->GetWhatsThis());
      hMacro->SetASCII("Statustip", pScript->GetStatusTip());
      hMacro->SetASCII("Pixmap", pScript->GetPixmap());
      hMacro->SetInt("Accel", pScript->GetAccel());
    }*/
  }

  // save recent file list
  Command* pCmd = d->_cCommandManager.getCommandByName("Std_MRU");
  if (pCmd)
  {
    char szBuf[200];
    int i=0;
    FCParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("Preferences")->GetGroup("RecentFiles");
    hGrp->Clear();
    hGrp->SetInt("RecentFiles", ((StdCmdMRU*)pCmd)->maxCount());

    QStringList files = ((StdCmdMRU*)pCmd)->recentFiles();
    if ( files.size() > 0 )
    {
      for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it, i++ )
      {
        sprintf(szBuf, "MRU%d", i);
        hGrp->SetASCII(szBuf, (*it).latin1());
      }
    }
  }

  saveWindowSettings();
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// creating std commands
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



void ApplicationWindow::createStandardOperations()
{
  // register the application Standard commands from CommandStd.cpp
  Gui::CreateStdCommands();
  Gui::CreateViewStdCommands();
  Gui::CreateTestCommands();
}

void ApplicationWindow::onPolish()
{
  d->viewbar  = d->_pcWidgetMgr->getPopupMenu("&View");
  d->viewbar->setCanModify(true);
  d->toolbars = new QPopupMenu(d->viewbar, "Toolbars");

  connect(d->viewbar,  SIGNAL(aboutToShow (   )), this, SLOT(onShowView(     )));

  d->windows = d->_pcWidgetMgr->getPopupMenu("&Windows");
  connect(d->windows, SIGNAL( aboutToShow()), this, SLOT( onWindowsMenuAboutToShow() ) );
}

bool ApplicationWindow::isCustomizable () const
{
  return true;
}

void ApplicationWindow::customize ()
{
  commandManager().runCommandByName("Std_DlgCustomize");
}

void ApplicationWindow::tileHorizontal()
{
  // primitive horizontal tiling
  QWidgetList windows = d->_pWorkspace->windowList();
  if ( !windows.count() )
    return;
    
  int heightForEach = d->_pWorkspace->height() / windows.count();
  int y = 0;
  for ( int i = 0; i < int(windows.count()); ++i ) 
  {
    QWidget *window = windows.at(i);
    if ( window->testWState( WState_Maximized ) ) 
    {
      // prevent flicker
      window->hide();
      window->showNormal();
    }

    int preferredHeight = window->minimumHeight()+window->parentWidget()->baseSize().height();
    int actHeight = QMAX(heightForEach, preferredHeight);
  
    window->parentWidget()->setGeometry( 0, y, d->_pWorkspace->width(), actHeight );
    y += actHeight;
  }
}

void ApplicationWindow::tile()
{
  d->_pWorkspace->tile();
}

void ApplicationWindow::cascade()
{
  d->_pWorkspace->cascade();
}

void ApplicationWindow::onShowView()
{
  Gui::CustomPopupMenu* menu = d->viewbar;
  menu->update(commandManager());
  menu->setCheckable(true);
  d->mCheckBars.clear();

  // toolbars
  menu->insertItem(tr("Toolbars"), createDockWindowMenu( OnlyToolBars ));
  menu->insertSeparator();

  connect( menu, SIGNAL( aboutToShow() ), this, SLOT( menuAboutToShow() ) );

  QPtrList<QDockWindow> wnds = dockWindows ();
  QDockWindow* dw;
  for ( dw = wnds.first(); dw; dw = wnds.next() )
  {
    if ( !dw->inherits("QToolBar") )
    {
      QString label = dw->caption();
      int id = menu->insertItem( label, dw, SLOT( toggleVisible() ) );
      menu->setItemChecked( id, dw->isVisible() );
    }
  }

  // status bar
  menu->insertSeparator();
  QWidget* w = statusBar();
  int id = menu->insertItem( tr("Status bar"), this, SLOT( onToggleStatusBar() ) );
  d->mCheckBars[id] = w;
  menu->setItemChecked(id, w->isVisible());
}

void ApplicationWindow::onToggleStatusBar()
{
  QWidget* w = statusBar();
  w->isVisible() ? w->hide() : w->show();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// document observers
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ApplicationWindow::OnDocNew(App::Document* pcDoc)
{
  d->lpcDocuments.push_back( new Gui::Document(pcDoc,this) );
}

void ApplicationWindow::OnDocDelete(App::Document* pcDoc)
{
  Gui::Document* pcGDoc;

  for(std::list<Gui::Document*>::iterator It = d->lpcDocuments.begin();It != d->lpcDocuments.end();It++)
  {
    if( ((*It)->getDocument()) == pcDoc)
    {
      pcGDoc = *It;
      d->lpcDocuments.erase(It);
      delete pcGDoc;
    }
  }

}

void ApplicationWindow::addWindow( MDIView* view )
{
  // make workspace parent of view
  view->reparent( d->_pWorkspace, QPoint() );
  connect( view, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );
  // show the very first window in maximized mode
  if ( d->_pWorkspace->windowList().isEmpty() )
    view->showMaximized();
  else
    view->show();

  // being informed when the view is destroyed
  connect( view, SIGNAL( destroyed() ), this, SLOT( onWindowDestroyed() ) );

  // add a new tab to our tabbar
  QTab* tab = new QTab;
  d->_tabIds[ view ] = tab;
  // extract file name if possible
  QFileInfo fi( view->caption() );
  if ( fi.isFile() && fi.exists() )
    tab->setText( fi.fileName() );
  else
    tab->setText( view->caption() );
  if ( view->icon() )
    tab->setIconSet( *view->icon() );
  d->_tabs->setToolTip( d->_tabs->count(), view->caption() );
  d->_tabs->addTab( tab );
  d->_tabs->show();
  d->_tabs->update();
//  d->_tabs->blockSignals( true ); // avoid that selected() signal is emitted
  d->_tabs->setCurrentTab( tab );
//  d->_tabs->blockSignals( false );
}

void ApplicationWindow::onWindowDestroyed()
{
  QObject* obj = (QObject*)sender();
  QMap<QObject*, QTab*>::Iterator it = d->_tabIds.find( obj );

  if ( it != d->_tabIds.end() )
  {
    QTab* tab = it.data();
    d->_tabs->removeTab( tab );
    if ( d->_tabs->count() == 0 )
      d->_tabs->hide();
  }
}

void ApplicationWindow::onWindowActivated( QWidget* w )
{
  MDIView* view = dynamic_cast<MDIView*>(w);
  if ( !view ) return; // either no MDIView or no valid object
  view->setActive();

  QMap<QObject*, QTab*>::Iterator it = d->_tabIds.find( view );
  if ( it != d->_tabIds.end() )
  {
    QTab* tab = it.data();
    d->_tabs->blockSignals( true ); // avoid that selected() signal is emitted
    d->_tabs->setCurrentTab( tab );
    d->_tabs->blockSignals( false );
  }
}

void ApplicationWindow::onTabSelected( int i)
{
  QTab* tab = d->_tabs->tab( i );
  if ( tab )
  {
    // get appropriate MDIView
    for ( QMap<QObject*, QTab*>::ConstIterator it = d->_tabIds.begin(); it != d->_tabIds.end(); ++it )
    {
      if ( it.data() == tab )
      {
        MDIView* view = dynamic_cast<MDIView*>(it.key());
        if ( !view ) return; // either no MDIView or no valid object
        view->setFocus();
        break;
      }
    }
  }
}

void ApplicationWindow::onWindowsMenuAboutToShow()
{
  QPopupMenu* windowsMenu = d->windows;
  QWidgetList windows = d->_pWorkspace->windowList( QWorkspace::CreationOrder );

  // remove old window items first
  while ( d->wndIDs.size() > 0 )
  {
    int id = d->wndIDs.front();
    d->wndIDs.pop_front();
    windowsMenu->removeItem( id );
  }

  // append new window items
  if ( windows.count() > 0 )
  {
    // insert separator before last item 
    int idx = windowsMenu->count() - 2;
    int pos = windowsMenu->insertSeparator( idx );
    d->wndIDs.push_back( pos );

    bool act = false;
    for ( int i = 0; i < int(windows.count()); ++i ) 
    {
      QString txt = QString("&%1 %2").arg( i+1 ).arg( windows.at(i)->caption() );
      idx = windowsMenu->count() - 2;

      act |= (d->_pWorkspace->activeWindow() == windows.at(i));

      if ( (act && d->wndIDs.size() < 10) || (!act && d->wndIDs.size() < 9))
      {
        int id = windowsMenu->insertItem( txt, this, SLOT( onWindowsMenuActivated( int ) ), 0, -1, idx );
        windowsMenu->setItemParameter( id, i );
        windowsMenu->setItemChecked( id, d->_pWorkspace->activeWindow() == windows.at(i) );
        d->wndIDs.push_back( id );
      }
    }
  }
}

void ApplicationWindow::onWindowsMenuActivated( int id )
{
  QWidget* w = d->_pWorkspace->windowList().at( id );
  if ( w )
    w->showNormal();
  w->setFocus();
}

QWidgetList ApplicationWindow::windows( QWorkspace::WindowOrder order ) const
{
  return d->_pWorkspace->windowList( order );
}

void ApplicationWindow::onLastWindowClosed(Gui::Document* pcDoc)
{
  if(!d->_bIsClosing)
  {
    // GuiDocument has closed the last window and get destructed
    d->lpcDocuments.remove(pcDoc);
    //lpcDocuments.erase(pcDoc);
    delete pcDoc;

    // last document closed?
    if(d->lpcDocuments.size() == 0 )
      // reset active document
      setActiveDocument(0);
    else
      setActiveDocument(d->lpcDocuments.front());
  }
}

// set text to the pane
void ApplicationWindow::setPaneText(int i, QString text)
{
  if (i==1)
    d->_pclActionLabel->setText(text);
  else if (i==2)
    d->_pclSizeLabel->setText(text);
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// view handling
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



/// send Messages to the active view
bool ApplicationWindow::sendMsgToActiveView(const char* pMsg)
{
  MDIView* pView = activeView();

  if(pView){
    return pView->onMsg(pMsg);
  }else
    return false;
}

bool ApplicationWindow::sendHasMsgToActiveView(const char* pMsg)
{
  MDIView* pView = activeView();

  if(pView){
    return pView->onHasMsg(pMsg);
  }else
    return false;
}


MDIView* ApplicationWindow::activeView(void)
{
  MDIView * pView = reinterpret_cast <MDIView *> ( d->_pWorkspace->activeWindow() );
  return pView;
}


/// Geter for the Active View
Gui::Document* ApplicationWindow::activeDocument(void)
{
  return d->_pcActiveDocument;
  /*
  MDIView* pView = GetActiveView();

  if(pView)
    return pView->GetGuiDocument();
  else
    return 0l;*/
}

void ApplicationWindow::setActiveDocument(Gui::Document* pcDocument)
{
  d->_pcActiveDocument=pcDocument;

  Console().Log("Activate Document (%p) \n",d->_pcActiveDocument);

  // notify all views attached to the application (not views belong to a special document)
  for(std::list<Gui::BaseView*>::iterator It=d->_LpcViews.begin();It!=d->_LpcViews.end();It++)
    (*It)->setDocument(pcDocument);
}

void ApplicationWindow::attachView(Gui::BaseView* pcView)
{
  d->_LpcViews.push_back(pcView);
}


void ApplicationWindow::detachView(Gui::BaseView* pcView)
{
  d->_LpcViews.remove(pcView);
}

void ApplicationWindow::onUpdate(void)
{
  // update all documents
  for(std::list<Gui::Document*>::iterator It = d->lpcDocuments.begin();It != d->lpcDocuments.end();It++)
  {
    (*It)->onUpdate();
  }
  // update all the independed views
  for(std::list<Gui::BaseView*>::iterator It2 = d->_LpcViews.begin();It2 != d->_LpcViews.end();It2++)
  {
    (*It2)->onUpdate();
  }
}

/// get calld if a view gets activated, this manage the whole activation scheme
void ApplicationWindow::viewActivated(MDIView* pcView)
{
  Console().Log("Activate View (%p) Type=\"%s\" \n",pcView,pcView->getName());
  // set the new active document
  if(pcView->isPassiv())
    setActiveDocument(0);
  else
    setActiveDocument(pcView->getGuiDocument());
}


void ApplicationWindow::updateActive(void)
{
  activeDocument()->onUpdate();
}

void ApplicationWindow::onUndo()
{
  puts("ApplicationWindow::slotUndo()");
}

void ApplicationWindow::onRedo()
{
  puts("ApplicationWindow::slotRedo()");
}

void ApplicationWindow::closeEvent ( QCloseEvent * e )
{
  if(d->lpcDocuments.size() == 0)
  {
    e->accept();
  }else{
    // ask all documents if closable
    for (std::list<Gui::Document*>::iterator It = d->lpcDocuments.begin();It!=d->lpcDocuments.end();It++)
    {
      (*It)->canClose ( e );
  //			if(! e->isAccepted() ) break;
      if(! e->isAccepted() ) return;
    }
  }

  // ask all passiv views if closable
  for (std::list<Gui::BaseView*>::iterator It2 = d->_LpcViews.begin();It2!=d->_LpcViews.end();It2++)
  {
    if((*It2)->canClose() )
      e->accept();
    else 
      e->ignore();

//		if(! e->isAccepted() ) break;
    if(! e->isAccepted() ) return;
  }

  if( e->isAccepted() )
  {
    d->_bIsClosing = true;

    std::list<Gui::Document*>::iterator It;

    // close all views belonging to a document
    for (It = d->lpcDocuments.begin();It!=d->lpcDocuments.end();It++)
    {
      (*It)->closeAllViews();
    }

    //detache the passiv views
    //SetActiveDocument(0);
    std::list<Gui::BaseView*>::iterator It2 = d->_LpcViews.begin();
    while (It2!=d->_LpcViews.end())
    {
      (*It2)->onClose();
      It2 = d->_LpcViews.begin();
    }

    // remove all documents
    for (It = d->lpcDocuments.begin();It!=d->lpcDocuments.end();It++)
    {
      delete(*It);
    }

    d->_pcActivityTimer->stop();

    activateWorkbench("<none>");
    QMainWindow::closeEvent( e );
  }
}

/**
 *  Activate the named workbench by calling the methodes in the 
 *  python workbench object. If the workbench is allready active
 *  nothing get called!
 */
void ApplicationWindow::activateWorkbench(const char* name)
{
  // net buffer because of char* <-> const char*
  Base::PyBuf Name(name);

  // close old workbench
  if(d->_cActiveWorkbenchName != "")
  {
    Base::PyBuf OldName ( d->_cActiveWorkbenchName.latin1());
    PyObject* pcOldWorkbench = PyDict_GetItemString(d->_pcWorkbenchDictionary, OldName.str);
    assert(pcOldWorkbench);
    customWidgetManager()->hideToolBox();
    Interpreter().RunMethodVoid(pcOldWorkbench, "Stop");
  }
  // get the python workbench object from the dictionary
  PyObject* pcWorkbench = PyDict_GetItemString(d->_pcWorkbenchDictionary, Name.str);

  try{
    // test if the workbench in
    assert(pcWorkbench);
    if (!pcWorkbench)
    {
      QString exc = tr("Workbench '%1' does not exist").arg(name);
      throw Base::Exception(exc.latin1());
    }

    // rename with new workbench before(!!!) calling "Start"
    d->_cActiveWorkbenchName = name;

    // running the start of the workbench object
    Interpreter().RunMethodVoid(pcWorkbench, "Start");
    d->_pcWidgetMgr->update(name);
    customWidgetManager()->showToolBox();

    // update the Std_Workbench command and its action object
    Command* pCmd = d->_cCommandManager.getCommandByName("Std_Workbench");
    if (pCmd)
    {
      ((StdCmdWorkbench*)pCmd)->activate( name );
    }

    show();
  }
  catch (const Base::Exception& rclE)
  {
    Console().Error("%s\n", rclE.what());
  }
}

void ApplicationWindow::updateWorkbenchEntrys(void)
{
//	PyObject *key, *value;
//	int pos = 0;
//     
//  Command* pCmd = d->_cCommandManager.GetCommandByName("Std_Workbench");
//  if (pCmd)
//  {
//  	// remove all items from the command
//    ((StdCmdWorkbench*)pCmd)->Clear();
//
//  	// insert all items
//    while (PyDict_Next(d->_pcWorkbenchDictionary, &pos, &key, &value)) {
//		  /* do something interesting with the values... */
//      ((StdCmdWorkbench*)pCmd)->AddItem(PyString_AsString(key));
//	  }
//  }
}

std::vector<std::string> ApplicationWindow::workbenches(void)
{
  PyObject *key, *value;
  int pos = 0;
  std::vector<std::string> wb;
  // insert all items
  while (PyDict_Next(d->_pcWorkbenchDictionary, &pos, &key, &value)) {
    /* do something interesting with the values... */
    wb.push_back(PyString_AsString(key));
  }
  return wb;
}

void ApplicationWindow::appendRecentFile(const char* file)
{
  Command* pCmd = d->_cCommandManager.getCommandByName("Std_MRU");
  if (pCmd)
  {
    ((StdCmdMRU*)pCmd)->addRecentFile(file);
  }
}

void ApplicationWindow::updateCmdActivity()
{
  static QTime cLastCall;

  if(cLastCall.elapsed() > 250 && isVisible () )
  {
    //puts("testActive");
    d->_cCommandManager.testActive();
    // remember last call
    cLastCall.start();
  }

  d->_pcActivityTimer->start( 300, TRUE );	
}

void ApplicationWindow::loadWindowSettings()
{
  loadDockWndSettings();

  FCParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("MainWindow");
  int w = hGrp->GetInt("Width", 1024);
  int h = hGrp->GetInt("Height", 768);
  int x = hGrp->GetInt("PosX", pos().x());
  int y = hGrp->GetInt("PosY", pos().y());
  bool max = hGrp->GetBool("Maximized", false);
  resize( w, h );
  move(x, y);
  if (max) showMaximized();
  //setBackgroundPixmap(QPixmap((const char*)FCBackground));

  updatePixmapsSize();
  updateStyle();
}

void ApplicationWindow::updatePixmapsSize(void)
{
  FCParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp");
  hGrp = hGrp->GetGroup("Preferences")->GetGroup("General");
  bool bigPixmaps = hGrp->GetBool("BigPixmaps", false);
  if (bigPixmaps != usesBigPixmaps())
    setUsesBigPixmaps (bigPixmaps);
}

void ApplicationWindow::updateStyle(void)
{
  QStyle& curStyle = QApplication::style();
  FCParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp");
  hGrp = hGrp->GetGroup("Preferences")->GetGroup("General");

  QString style = hGrp->GetASCII( "WindowStyle", curStyle.name() ).c_str();
  if ( style == QString( curStyle.name() ) )
    return; // already set
  
  QStyle* newStyle = QStyleFactory::create( style );
  if ( newStyle != 0 )
  {
    QApplication::setStyle( newStyle );
  }
}

void ApplicationWindow::saveWindowSettings()
{
  FCParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("MainWindow");
  if (isMaximized())
  {
    hGrp->SetBool("Maximized", true);
  }
  else
  {
    hGrp->SetInt("Width", width());
    hGrp->SetInt("Height", height());
    hGrp->SetInt("PosX", pos().x());
    hGrp->SetInt("PosY", pos().y());
    hGrp->SetBool("Maximized", false);
  }

  saveDockWndSettings();
}

void ApplicationWindow::loadDockWndSettings()
{
  FCParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("MainWindow");
  QString str = hGrp->GetASCII("Layout", "").c_str();

  if ( !str.isEmpty() )
  {
    QTextStream ts( &str, IO_ReadOnly );
    ts >> *this;
  }
/*
  // open file
  std::string FileName(GetApplication().GetHomePath());
  FileName += "FreeCAD.xml";
  QFile* datafile = new QFile(FileName.c_str());
  if (!datafile->open(IO_ReadOnly)) 
  {
    // error opening file
    bool bMute = GuiConsoleObserver::bMute;
    GuiConsoleObserver::bMute = true;
    Console().Warning((tr("Error: Cannot open file '%1' "
                             "(Maybe you're running FreeCAD the first time)\n").arg(FileName.c_str())));
    GuiConsoleObserver::bMute = bMute;
    datafile->close();
    delete (datafile);
    return;
  }

  // open dom document
  QDomDocument doc("DockWindows");
  if (!doc.setContent(datafile)) 
  {
    Console().Warning("Error:  is not a valid file\n");
    datafile->close();
    delete (datafile);
    return;
  }

  datafile->close();
  delete (datafile);

  // check the doc type and stuff
  if (doc.doctype().name() != "DockWindows") 
  {
    // wrong file type
    Console().Warning("Error: is not a valid file\n");
    return;
  }

  QDomElement root = doc.documentElement();
  if (root.attribute("application") != QString("FreeCAD")) 
  {
    // right file type, wrong application
    Console().Warning("Error: wrong file\n");
    return;
  }

  readDockConfig(root);*/
}

void ApplicationWindow::saveDockWndSettings()
{
  QString str;
  QTextStream ts( &str, IO_WriteOnly );
  ts << *this;

  FCParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("MainWindow");
  hGrp->SetASCII("Layout", str.latin1());
/*
  // save dock window settings
  QDomDocument doc("DockWindows");

  // create the root element
  QDomElement root = doc.createElement(doc.doctype().name());
  root.setAttribute("version", "0.1");
  root.setAttribute("application", "FreeCAD");

  writeDockConfig(root);
  doc.appendChild(root);

  // save into file
  std::string FileName(GetApplication().GetHomePath());
  FileName += "FreeCAD.xml";
  QFile* datafile = new QFile (FileName.c_str());
  if (!datafile->open(IO_WriteOnly)) 
  {
    // error opening file
    Console().Warning("Error: Cannot open file\n");
    datafile->close();
    delete (datafile);
    return;
  }

  // write it out
  QTextStream textstream(datafile);
  doc.save(textstream, 0);
  datafile->close();
  delete (datafile);*/
}

bool ApplicationWindow::isClosing(void)
{
  return d->_bIsClosing;
}

Gui::CustomWidgetManager* ApplicationWindow::customWidgetManager(void) 
{ 
  return d->_pcWidgetMgr; 
}

QString ApplicationWindow::activeWorkbench(void)
{
  return d->_cActiveWorkbenchName;
}

MacroManager *ApplicationWindow::macroManager(void)
{
  return d->_pcMacroMngr;
}

CommandManager &ApplicationWindow::commandManager(void)
{
  return d->_cCommandManager;
}

void ApplicationWindow::languageChange()
{
  CommandManager& rclMan = commandManager();
  std::vector<Command*> cmd = rclMan.getAllCommands();
  for ( std::vector<Command*>::iterator it = cmd.begin(); it != cmd.end(); ++it )
  {
    (*it)->languageChange();
  }

  // and finally update the menu bar since QMenuBar owns no "text" property
  Gui::CustomWidgetManager* cw = customWidgetManager();
  const QMap<int, QString>& mi = cw->menuBarItems();
 
  QMenuBar* mb = menuBar();
  uint cnt = mb->count();
  for ( uint i=0; i<cnt; i++ )
  {
    int id = mb->idAt( i );
    QMap<int, QString>::ConstIterator it = mi.find( id );
    if ( it != mi.end() )
      mb->changeItem( id, tr( it.data()/*mb->text( id )*/ ) );
  }
}

//**************************************************************************
// Init, Destruct and singelton

QApplication* ApplicationWindow::_pcQApp = NULL ;

QSplashScreen *ApplicationWindow::_splash = NULL;


void ApplicationWindow::initApplication(void)
{
//	std::map<std::string,std::string> &Config = GetApplication().Config();


  new Base::ScriptProducer( "FreeCADGuiInit", FreeCADGuiInit );
}

void messageHandler( QtMsgType type, const char *msg )
{
//  bool mute = GuiConsoleObserver::bMute;
//  GuiConsoleObserver::bMute = false;

  switch ( type )
  {
    case QtDebugMsg:
      Base::Console().Message( msg );
      break;
    case QtWarningMsg:
      Base::Console().Warning( msg );
      break;
    case QtFatalMsg:
      Base::Console().Error( msg );
      abort();                    // deliberately core dump
  }

//  GuiConsoleObserver::bMute = mute;
}

void ApplicationWindow::runApplication(void)
{
  // A new QApplication
  Console().Log("Creating GUI Application...\n");
  // if application not yet created by the splasher
  int argc = App::Application::GetARGC();
  qInstallMsgHandler( messageHandler );
  if (!_pcQApp)  _pcQApp = new QApplication (argc, App::Application::GetARGV());

  startSplasher();
  ApplicationWindow * mw = new ApplicationWindow();
  _pcQApp->setMainWidget(mw);

  // runing the Gui init script
  Interpreter().Launch(Base::ScriptFactory().ProduceScript("FreeCADGuiInit"));
  // show the main window
  Console().Log("Showing GUI Application...\n");
  mw->onPolish();
  mw->show();
  stopSplasher();
  showTipOfTheDay();

  _pcQApp->connect( _pcQApp, SIGNAL(lastWindowClosed()), _pcQApp, SLOT(quit()) );

  // run the Application event loop
  Console().Log("Running event loop...\n");
  _pcQApp->exec();
  Console().Log("event loop left\n");
}

void ApplicationWindow::startSplasher(void)
{
  // startup splasher
  // when running in verbose mode no splasher
  if ( ! (App::Application::Config()["Verbose"] == "Strict") && (App::Application::Config()["RunMode"] == "Gui") )
  {
    FCParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("Preferences")->GetGroup("General");
#ifdef FC_DEBUG
  bool splash = false;
#else
  bool splash = true;
#endif
    if (hGrp->GetBool("AllowSplasher", splash))
    {
      QPixmap pixmap(( const char** ) splash_screen );
      _splash = new SplashScreen( pixmap );
      _splash->show();
    }
  }
}

void ApplicationWindow::stopSplasher(void)
{
  if ( _splash )
  {
    _splash->finish( Instance );
    delete _splash;
    _splash = 0L;
  }
}

void ApplicationWindow::showTipOfTheDay( bool force )
{
  // tip of the day?
  FCParameterGrp::handle
  hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("Preferences")->GetGroup("General");

#ifdef FC_DEBUG
  bool tip = false;
#else
  bool tip = true;
#endif

  tip = hGrp->GetBool("Tipoftheday", tip);
  if ( tip || force)
  {
    Gui::Dialog::DlgTipOfTheDayImp dlg(Instance, "Tipofday");
    dlg.exec();
  }
}

void ApplicationWindow::destruct(void)
{
  Console().Log("Destruct GuiApplication\n");

  delete _pcQApp;
}



//**************************************************************************
// Python stuff

// FCApplication Methods						// Methods structure
PyMethodDef ApplicationWindow::Methods[] = {
  {"MenuAppendItems",       (PyCFunction) ApplicationWindow::sMenuAppendItems,         1},
  {"MenuRemoveItems",       (PyCFunction) ApplicationWindow::sMenuRemoveItems,         1},
  {"MenuDelete",            (PyCFunction) ApplicationWindow::sMenuDelete,              1},
  {"ToolbarAppendItems",    (PyCFunction) ApplicationWindow::sToolbarAppendItems,      1},
  {"ToolbarRemoveItems",    (PyCFunction) ApplicationWindow::sToolbarRemoveItems,      1},
  {"ToolbarDelete",         (PyCFunction) ApplicationWindow::sToolbarDelete,           1},
  {"CommandbarAppendItems", (PyCFunction) ApplicationWindow::sCommandbarAppendItems,   1},
  {"CommandbarRemoveItems", (PyCFunction) ApplicationWindow::sCommandbarRemoveItems,   1},
  {"CommandbarDelete",      (PyCFunction) ApplicationWindow::sCommandbarDelete,        1},
  {"WorkbenchAdd",          (PyCFunction) ApplicationWindow::sWorkbenchAdd,            1},
  {"WorkbenchDelete",       (PyCFunction) ApplicationWindow::sWorkbenchDelete,         1},
  {"WorkbenchActivate",     (PyCFunction) ApplicationWindow::sWorkbenchActivate,       1},
  {"WorkbenchGet",          (PyCFunction) ApplicationWindow::sWorkbenchGet,            1},
  {"UpdateGui",             (PyCFunction) ApplicationWindow::sUpdateGui,               1},
  {"CreateDialog",          (PyCFunction) ApplicationWindow::sCreateDialog,            1},
  {"CommandAdd",            (PyCFunction) ApplicationWindow::sCommandAdd,              1},
  {"RunCommand",            (PyCFunction) ApplicationWindow::sRunCommand,              1},
  {"SendMsgToActiveView",   (PyCFunction) ApplicationWindow::sSendActiveView,          1},

  {NULL, NULL}		/* Sentinel */
};

PYFUNCIMP_S(ApplicationWindow,sSendActiveView)
{
  char *psCommandStr;
  if (!PyArg_ParseTuple(args, "s",&psCommandStr))     // convert args: Python->C 
    return NULL;                                      // NULL triggers exception 

  if(!Instance->sendMsgToActiveView(psCommandStr))
    Base::Console().Warning("Unknown view command: %s\n",psCommandStr);

  Py_INCREF(Py_None);
  return Py_None;
} 

PYFUNCIMP_S(ApplicationWindow,sUpdateGui)
{
  if (!PyArg_ParseTuple(args, ""))     // convert args: Python->C 
    return NULL;                                      // NULL triggers exception 

  qApp->processEvents();

  Py_INCREF(Py_None);
  return Py_None;
} 

PYFUNCIMP_S(ApplicationWindow,sCreateDialog)
{
  char* fn = 0;
  if (!PyArg_ParseTuple(args, "s", &fn))     // convert args: Python->C 
    return NULL;                                      // NULL triggers exception 

  PyObject* pPyResource=0L;
  try{
    pPyResource = new PyResource();
    ((PyResource*)pPyResource)->load(fn);
  } catch (const Base::Exception& e)
  {
    PyErr_SetString(PyExc_AssertionError, e.what());
  }

  return pPyResource;
} 

PYFUNCIMP_S(ApplicationWindow,sMenuAppendItems)
{
  PyObject* pObject;
  char* psMenuName;
  char* parent = 0;
  int bModify=1;
  int bRemovable=1;
  // convert args: Python->C 
  if (!PyArg_ParseTuple(args, "sO|iis", &psMenuName, &pObject, &bModify,
                                        &bRemovable, &parent ))     
    return NULL;                             // NULL triggers exception 
  if (!PyList_Check(pObject))
  {
    PyErr_SetString(PyExc_AssertionError, "Expected a list as second argument");
    return NULL;                             // NULL triggers exception 
  }

  QStringList items;
  int nSize = PyList_Size(pObject);
  for (int i=0; i<nSize;++i)
  {
    PyObject* item = PyList_GetItem(pObject, i);
    if (!PyString_Check(item))
      continue;

    char* pItem = PyString_AsString(item);

    items.push_back(pItem);
  }

  try{
    Instance->d->_pcWidgetMgr->addPopupMenu(psMenuName, items, parent);
    Instance->d->_pcWidgetMgr->getPopupMenu(psMenuName)->setCanModify(bModify!=0);
    Instance->d->_pcWidgetMgr->getPopupMenu(psMenuName)->setRemovable(bRemovable!=0);
  }catch(const Base::Exception& e) {
    PyErr_SetString(PyExc_AssertionError, e.what());		
    return NULL;
  }catch(...){
    PyErr_SetString(PyExc_RuntimeError, "unknown error");
    return NULL;
  }

  Py_INCREF(Py_None);
  return Py_None;
} 

PYFUNCIMP_S(ApplicationWindow,sMenuRemoveItems)
{
  PyObject* pObject;
  char* psMenuName;
  if (!PyArg_ParseTuple(args, "sO", &psMenuName, 
                                     &pObject))     // convert args: Python->C 
    return NULL;                             // NULL triggers exception 
  if (!PyList_Check(pObject))
  {
    PyErr_SetString(PyExc_AssertionError, "Expected a list as second argument");
    return NULL;                             // NULL triggers exception 
  }

  QStringList items;
  int nSize = PyList_Size(pObject);
  for (int i=0; i<nSize;++i)
  {
    PyObject* item = PyList_GetItem(pObject, i);
    if (!PyString_Check(item))
      continue;

    char* pItem = PyString_AsString(item);

    items.push_back(pItem);
  }

  Instance->d->_pcWidgetMgr->removeMenuItems( psMenuName, items );

  Py_INCREF(Py_None);
  return Py_None;
} 

PYFUNCIMP_S(ApplicationWindow,sMenuDelete)
{
    char *psMenuName;
    if (!PyArg_ParseTuple(args, "s", &psMenuName))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 
  Instance->customWidgetManager()->removePopupMenu(psMenuName);

  Py_INCREF(Py_None);
  return Py_None;
}

PYFUNCIMP_S(ApplicationWindow,sToolbarAppendItems)
{
  PyObject* pObject;
  char* psToolbarName;
  int bModify=1;
  int bVisible=1;
  int bRemovable=1;
  // convert args: Python->C 
  if (!PyArg_ParseTuple(args, "sO|iii", &psToolbarName, &pObject, &bModify,
                                        &bRemovable, &bVisible ))     
    return NULL;                             // NULL triggers exception 
  if (!PyList_Check(pObject))
  {
    PyErr_SetString(PyExc_AssertionError, "Expected a list as second argument");
    return NULL;                                      // NULL triggers exception 
  }

  QStringList items;
  int nSize = PyList_Size(pObject);
  for (int i=0; i<nSize;++i)
  {
    PyObject* item = PyList_GetItem(pObject, i);
    if (!PyString_Check(item))
      continue;

    char* pItem = PyString_AsString(item);

    items.push_back(pItem);
  }

  try{
    Instance->d->_pcWidgetMgr->addToolBar( psToolbarName, items );
    if (!bVisible)
      Instance->d->_pcWidgetMgr->getToolBar(psToolbarName)->hide();
    Instance->d->_pcWidgetMgr->getToolBar(psToolbarName)->setCanModify(bModify!=0);
    Instance->d->_pcWidgetMgr->getToolBar(psToolbarName)->setRemovable(bRemovable!=0);
  }catch(const Base::Exception& e) {
    PyErr_SetString(PyExc_AssertionError, e.what());		
    return NULL;
  }catch(...){
    PyErr_SetString(PyExc_RuntimeError, "unknown error");
    return NULL;
  }

  Py_INCREF(Py_None);
  return Py_None;
} 

PYFUNCIMP_S(ApplicationWindow,sToolbarRemoveItems)
{
  PyObject* pObject;
  char* psToolbarName;
  if (!PyArg_ParseTuple(args, "sO", &psToolbarName, 
                                     &pObject))     // convert args: Python->C 
    return NULL;                             // NULL triggers exception 
  if (!PyList_Check(pObject))
  {
    PyErr_SetString(PyExc_AssertionError, "Expected a list as second argument");
    return NULL;                             // NULL triggers exception 
  }

  QStringList items;
  int nSize = PyList_Size(pObject);
  for (int i=0; i<nSize;++i)
  {
    PyObject* item = PyList_GetItem(pObject, i);
    if (!PyString_Check(item))
      continue;

    char* pItem = PyString_AsString(item);

    items.push_back(pItem);
  }

  Instance->d->_pcWidgetMgr->removeToolBarItems( psToolbarName, items );

  Py_INCREF(Py_None);
  return Py_None;
} 

PYFUNCIMP_S(ApplicationWindow,sToolbarDelete)
{
    char *psToolbarName;
    if (!PyArg_ParseTuple(args, "s", &psToolbarName))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 
  Instance->customWidgetManager()->removeToolBar(psToolbarName);

  Py_INCREF(Py_None);
    return Py_None;
}

PYFUNCIMP_S(ApplicationWindow,sCommandbarAppendItems)
{
  PyObject* pObject;
  char* psToolbarName;
  int bModify=1;
  int bRemovable=1;
  // convert args: Python->C 
  if (!PyArg_ParseTuple(args, "sO|ii", &psToolbarName, &pObject,
                                      &bModify, &bRemovable))
    return NULL;                             // NULL triggers exception 
  if (!PyList_Check(pObject))
  {
    PyErr_SetString(PyExc_AssertionError, "Expected a list as second argument");
    return NULL;                                      // NULL triggers exception 
  }

  QStringList items;
  int nSize = PyList_Size(pObject);
  for (int i=0; i<nSize;++i)
  {
    PyObject* item = PyList_GetItem(pObject, i);
    if (!PyString_Check(item))
      continue;

    char* pItem = PyString_AsString(item);

    items.push_back(pItem);
  }

  try{
    Instance->d->_pcWidgetMgr->addCommandBar(psToolbarName, items);
    Instance->d->_pcWidgetMgr->getCommandBar(psToolbarName)->setCanModify(bModify!=0);
    Instance->d->_pcWidgetMgr->getCommandBar(psToolbarName)->setRemovable(bRemovable!=0);
  }catch(const Base::Exception& e) {
    PyErr_SetString(PyExc_AssertionError, e.what());		
    return NULL;
  }catch(...){
    PyErr_SetString(PyExc_RuntimeError, "unknown error");
    return NULL;
  }

  Py_INCREF(Py_None);
  return Py_None;
} 

PYFUNCIMP_S(ApplicationWindow,sCommandbarRemoveItems)
{
  PyObject* pObject;
  char* psToolbarName;
  if (!PyArg_ParseTuple(args, "sO", &psToolbarName, 
                                     &pObject))     // convert args: Python->C 
    return NULL;                             // NULL triggers exception 
  if (!PyList_Check(pObject))
  {
    PyErr_SetString(PyExc_AssertionError, "Expected a list as second argument");
    return NULL;                             // NULL triggers exception 
  }

  QStringList items;
  int nSize = PyList_Size(pObject);
  for (int i=0; i<nSize;++i)
  {
    PyObject* item = PyList_GetItem(pObject, i);
    if (!PyString_Check(item))
      continue;

    char* pItem = PyString_AsString(item);

    items.push_back(pItem);
  }

  Instance->d->_pcWidgetMgr->removeCommandBarItems( psToolbarName, items );

  Py_INCREF(Py_None);
  return Py_None;
} 

PYFUNCIMP_S(ApplicationWindow,sCommandbarDelete)
{
    char *psToolbarName;
    if (!PyArg_ParseTuple(args, "s", &psToolbarName))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 

  Instance->customWidgetManager()->removeCommandBar(psToolbarName);
  
  Py_INCREF(Py_None);
  return Py_None;
}


PYFUNCIMP_S(ApplicationWindow,sWorkbenchAdd)
{
  char*       psKey;
  PyObject*   pcObject;
  if (!PyArg_ParseTuple(args, "sO", &psKey,&pcObject))     // convert args: Python->C 
    return NULL;                    // NULL triggers exception 

  //Py_INCREF(pcObject);

  PyDict_SetItemString(Instance->d->_pcWorkbenchDictionary,psKey,pcObject);

  Instance->updateWorkbenchEntrys();

  Py_INCREF(Py_None);
  return Py_None;
} 

PYFUNCIMP_S(ApplicationWindow,sWorkbenchDelete)
{
  char*       psKey;
  if (!PyArg_ParseTuple(args, "s", &psKey))     // convert args: Python->C 
    return NULL;                    // NULL triggers exception 

  PyDict_DelItemString(Instance->d->_pcWorkbenchDictionary,psKey);

  Instance->updateWorkbenchEntrys();

  Py_INCREF(Py_None);
  return Py_None;
} 

PYFUNCIMP_S(ApplicationWindow,sWorkbenchActivate)
{
  char*       psKey;
  if (!PyArg_ParseTuple(args, "s", &psKey))     // convert args: Python->C 
    return NULL;                    // NULL triggers exception 

  Instance->activateWorkbench(psKey);

  Py_INCREF(Py_None);
    return Py_None;
}

PYFUNCIMP_S(ApplicationWindow,sWorkbenchGet)
{
    if (!PyArg_ParseTuple(args, ""))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 

  return Instance->d->_pcWorkbenchDictionary;
}

PYFUNCIMP_S(ApplicationWindow,sCommandAdd)
{
  char*       pName;
  PyObject*   pcCmdObj;
  if (!PyArg_ParseTuple(args, "sO", &pName,&pcCmdObj))     // convert args: Python->C 
    return NULL;                    // NULL triggers exception 

  //Py_INCREF(pcObject);

  ApplicationWindow::Instance->commandManager().addCommand(new PythonCommand(pName,pcCmdObj));

  Py_INCREF(Py_None);
  return Py_None;
} 

PYFUNCIMP_S(ApplicationWindow,sRunCommand)
{
  char*       pName;
  if (!PyArg_ParseTuple(args, "s", &pName))     // convert args: Python->C 
    return NULL;                    // NULL triggers exception 

  ApplicationWindow::Instance->commandManager().runCommandByName(pName);

  Py_INCREF(Py_None);
  return Py_None;
} 

//**************************************************************************
//**************************************************************************
// FCAppConsoleObserver
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef FC_DEBUG
bool GuiConsoleObserver::bMute = true;
#else
bool GuiConsoleObserver::bMute = false;
#endif

GuiConsoleObserver::GuiConsoleObserver(ApplicationWindow *pcAppWnd)
  :_pcAppWnd(pcAppWnd){}

/// get calles when a Warning is issued
void GuiConsoleObserver::Warning(const char *m)
{
  if(!bMute){
    QMessageBox::warning( _pcAppWnd, "Warning",m);
    _pcAppWnd->statusBar()->message( m, 2001 );
  }
}

/// get calles when a Message is issued
void GuiConsoleObserver::Message(const char * m)
{
  if(!bMute)
    _pcAppWnd->statusBar()->message( m, 2001 );
}

/// get calles when a Error is issued
void GuiConsoleObserver::Error  (const char *m)
{
  if(!bMute)
  {
    QMessageBox::critical( _pcAppWnd, "Exception happens",m);
    _pcAppWnd->statusBar()->message( m, 2001 );
  }
}

/// get calles when a Log Message is issued
void GuiConsoleObserver::Log    (const char *)
{
}

//**************************************************************************
//**************************************************************************
// FCAppConsoleObserver
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#include "moc_Application.cpp"
