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

// FreeCAD Base header
#include <Base/Parameter.h>
#include <Base/FileInfo.h>
#include <App/Application.h>

#include "MainWindow.h"
#include "Application.h"
#include "Action.h"
#include "View.h"

#include "Icons/developers.h"
#include "WidgetFactory.h"
#include "Command.h"
#include "Tree.h"
#include "PropertyView.h"
#include "BitmapFactory.h"
#include "Splashscreen.h"
#include "MenuManager.h"

#include "DlgTipOfTheDayImp.h"
#include "DlgUndoRedo.h"
#include "DlgOnlineHelpImp.h"
#include "ToolBox.h"
#include "HelpView.h"
#include "ReportView.h"
#include "Macro.h"
#include "ProgressBar.h"
#include "Window.h" 
#include "Workbench.h"
#include "WorkbenchManager.h"
#include "CommandBarManager.h"

#include "Language/Translator.h"
#include "GuiInitScript.h"


using namespace Gui;
using namespace Gui::DockWnd;
using namespace std;


MainWindow* MainWindow::instance = 0L;

namespace Gui {

// Pimpl class
struct MainWindowP
{
  QLabel* sizeLabel;
  QLabel* actionLabel;
  QTimer* activityTimer; 
  QWorkspace* workspace;
  QTabBar* tabs;
  QSignalMapper* windowMapper;
  QSplashScreen* splashscreen;
};

class MDITabbar : public QTabBar
{
public:
  MDITabbar( QWidget * parent = 0 ) : QTabBar(parent)
  {
    menu = new QMenu(this);
    // For Qt 4.2.x the tabs might be very wide
#if QT_VERSION >= 0x040200
    setDrawBase(false);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
#endif
  }

  ~MDITabbar()
  {
    delete menu;
  }

protected:
  void contextMenuEvent ( QContextMenuEvent * e )
  {
    menu->clear();
    CommandManager& cMgr = Application::Instance->commandManager();
    if ( tabRect(currentIndex()).contains(e->pos()) )
      cMgr.getCommandByName("Std_CloseActiveWindow")->addTo(menu);
    cMgr.getCommandByName("Std_CloseAllWindows")->addTo(menu);
    menu->insertSeparator();
    cMgr.getCommandByName("Std_CascadeWindows")->addTo(menu);
    cMgr.getCommandByName("Std_ArrangeIcons")->addTo(menu);
    cMgr.getCommandByName("Std_TileWindows")->addTo(menu);
    menu->insertSeparator();
    cMgr.getCommandByName("Std_Windows")->addTo(menu);
    menu->popup(e->globalPos());
  }

private:
  QMenu* menu;
};

} // namespace Gui


/* TRANSLATOR Gui::MainWindow */

MainWindow::MainWindow(QWidget * parent, Qt::WFlags f)
  : QMainWindow( parent, f/*WDestructiveClose*/ )
{
  d = new MainWindowP;
  d->splashscreen = 0;

  // global access 
  instance = this;

  // Create the layout containing the workspace and a tab bar
  QFrame* vbox = new QFrame(this);
  vbox->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setMargin(1);
  vbox->setLayout(layout);

  d->workspace = new QWorkspace();
  d->workspace->setScrollBarsEnabled( true );
  QPixmap backgnd(( const char** ) background );
  d->workspace->setBackground( backgnd );
  d->tabs = new MDITabbar();
  d->tabs->setShape( QTabBar:: RoundedSouth );

  layout->addWidget(d->workspace);
  layout->addWidget(d->tabs);
  setCentralWidget( vbox );

  // window title and icon of the main window
  setWindowTitle( App::Application::Config()["ExeName"].c_str() );
  setIcon(Gui::BitmapFactory().pixmap(App::Application::Config()["AppIcon"].c_str()));

  // labels and progressbar
  d->actionLabel = new QLabel(statusBar());
  d->actionLabel->setMinimumWidth(120);
  d->sizeLabel = new QLabel(tr("Dimension"), statusBar());
  d->sizeLabel->setMinimumWidth(120);
  statusBar()->addWidget(d->actionLabel,0 , true);
  statusBar()->addWidget(ProgressBar::instance(), 0, true);
  statusBar()->addWidget(d->sizeLabel, 0, true);

  // update gui timer
  d->activityTimer = new QTimer( this );
  connect( d->activityTimer, SIGNAL(timeout()),this, SLOT(updateActions()) );
  d->activityTimer->start( 300, TRUE );

  d->windowMapper = new QSignalMapper(this);

  // connection between workspace, window menu and tab bar
  connect( d->windowMapper, SIGNAL( mapped(QWidget *) ), d->workspace, SLOT( setActiveWindow( QWidget* ) ) );
  connect( d->workspace, SIGNAL( windowActivated ( QWidget * ) ), this, SLOT( onWindowActivated( QWidget* ) ) );
  connect( d->tabs, SIGNAL( selected( int) ), this, SLOT( onTabSelected(int) ) );

  //TODO Show all dockable windows over the workbench facility
  static const char* dockWindows[] = {
    QT_TRANSLATE_NOOP( "Gui::DockWindow", "Toolbox" ),
    QT_TRANSLATE_NOOP( "Gui::DockWindow", "Help view" ),
    QT_TRANSLATE_NOOP( "Gui::DockWindow", "Tree view" ),
    QT_TRANSLATE_NOOP( "Gui::DockWindow", "Property editor" ),
    QT_TRANSLATE_NOOP( "Gui::DockWindow", "Report View" )
  };

  // Cmd Button Group +++++++++++++++++++++++++++++++++++++++++++++++
  ToolBox* toolBox = new ToolBox(this);
  CommandBarManager::getInstance()->setToolBox( toolBox );
  DockWindowManager* pDockMgr = DockWindowManager::instance();
  pDockMgr->addDockWindow( dockWindows[0],toolBox, Qt::RightDockWidgetArea );

  // Help View ++++++++++++++++++++++++++++++++++++++++++++++++++++++
  QString home = Gui::Dialog::DlgOnlineHelpImp::getStartpage();
  HelpView* pcHelpView = new HelpView( home, this, "HelpViewer" );
  pDockMgr->addDockWindow(dockWindows[1], pcHelpView, Qt::RightDockWidgetArea );

  // Tree Bar  ++++++++++++++++++++++++++++++++++++++++++++++++++++++
  pcTree = new TreeView(0,this,"TreeView");
  pcTree->setMinimumWidth(210);
  pDockMgr->addDockWindow(dockWindows[2], pcTree, Qt::LeftDockWidgetArea );

  // PropertyView  ++++++++++++++++++++++++++++++++++++++++++++++++++++++
  PropertyView* pcPropView = new PropertyView(0,0,"PropertyView");
  pcPropView->setMinimumWidth(210);
  pDockMgr->addDockWindow(dockWindows[3], pcPropView, Qt::LeftDockWidgetArea );

  // Report View
  Gui::DockWnd::ReportView* pcOutput = new Gui::DockWnd::ReportView(this);
  pDockMgr->addDockWindow(dockWindows[4], pcOutput, Qt::BottomDockWidgetArea );


  // accept drops on the window, get handled in dropEvent, dragEnterEvent
  setAcceptDrops(true);
  statusBar()->message( tr("Ready"), 2001 );
}

MainWindow::~MainWindow()
{
  //TODO Redesign StdCmdRecentFiles
  // save recent file list
  // Note: the recent files are restored in StdCmdRecentFiles::createAction(), because we need
  //       a valid instance of StdCmdRecentFiles to do this
  StdCmdRecentFiles::save();

  delete d;
  instance = 0;
}

MainWindow* MainWindow::getInstance()
{
  // MainWindow has a public constructor
  return instance;
}

QMenu* MainWindow::createPopupMenu ()
{
  Command* cmd = Application::Instance->commandManager().getCommandByName("Std_DlgCustomize");
  QMenu* menu = QMainWindow::createPopupMenu();
  if (cmd) { 
    menu->addSeparator();
    cmd->addTo(menu);
  }

  return menu;
}

void MainWindow::arrangeIcons()
{
  d->workspace->arrangeIcons();
}

void MainWindow::tile()
{
  d->workspace->tile();
}

void MainWindow::cascade()
{
  d->workspace->cascade();
}

void MainWindow::closeActiveWindow ()
{
  d->workspace->closeActiveWindow();
}

void MainWindow::closeAllWindows ()
{
  d->workspace->closeAllWindows();
}

void MainWindow::activateNextWindow ()
{
  d->workspace->activateNextWindow();
}

void MainWindow::activatePreviousWindow ()
{
  d->workspace->activatePreviousWindow();
}

void MainWindow::whatsThis()
{
  QWhatsThis::enterWhatsThisMode();
}

bool MainWindow::eventFilter ( QObject* o, QEvent* e )
{
  if ( o != this ) {
    if ( e->type() == QEvent::WindowStateChange ) {
      // notify all mdi views when the active view receives a show normal, show minimized or show maximized event 
      MDIView * view = (MDIView*)o->qt_metacast("Gui::MDIView");
      if ( view )
        showActiveView( view );
    }
  }
#if 0 //TODO Check eventFilter
  if ( o != this ) {
    if ( e->type() == QEvent::ShowMaximized || e->type() == QEvent::ShowMinimized || e->type() == QEvent::ShowNormal || e->type() == QEvent::ShowFullScreen ) {
      MDIView * view = (MDIView*)o->qt_metacast("Gui::MDIView");

      // notify all mdi views when the active view receives a show normal, show minimized or show maximized event 
      if ( view )
        emit showActiveView( view );
    }
  }
#endif

  return QMainWindow::eventFilter( o, e );
}

void MainWindow::addWindow( MDIView* view )
{
  // make workspace parent of view
  d->workspace->addWindow(view);
  connect( view, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );
  connect( this, SIGNAL( showActiveView(MDIView*) ), view, SLOT( showActiveView(MDIView*) ) );

  // listen to the incoming events of the view
  view->installEventFilter(this);

  // show the very first window in maximized mode
  if ( d->workspace->windowList().isEmpty() )
    view->showMaximized();
  else
    view->show();

  // look if the window was already inserted
  for ( int i=0; i < d->tabs->count(); i++ ) {
    if ( d->tabs->tabData(i).value<QWidget*>() == view )
      return;
  }

  // being informed when the view is destroyed
  connect( view, SIGNAL( destroyed() ), this, SLOT( onWindowDestroyed() ) );

  // add a new tab to our tabbar
  int index=-1;
  if ( view->icon() )
    index = d->tabs->addTab(*view->icon(), view->caption());
  else
    index = d->tabs->addTab(view->caption());
  d->tabs->setTabToolTip( index, view->caption() );
  QVariant var; var.setValue<QWidget*>(view);
  d->tabs->setTabData( index, var);

  tabChanged( view );
  if ( d->tabs->count() == 1 )
    d->tabs->show(); // invoke show() for the first tab
  d->tabs->update();
  d->tabs->setCurrentIndex( index );
}

void MainWindow::removeWindow( Gui::MDIView* view )
{
  // free all connections
  disconnect( view, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );
  disconnect( this, SIGNAL( showActiveView(MDIView*) ), view, SLOT( showActiveView(MDIView*) ) );
  view->removeEventFilter(this);

  for ( int i = 0; i < d->tabs->count(); i++ ) {
    if ( d->tabs->tabData(i).value<QWidget*>() == view ) {
      d->tabs->removeTab( i );
      if ( d->tabs->count() == 0 )
        d->tabs->hide(); // no view open any more
      break;
    }
  }

  // this view is not under control of the main window any more
  disconnect( view, SIGNAL( destroyed() ), this, SLOT( onWindowDestroyed() ) );
}

void MainWindow::tabChanged( MDIView* view )
{
  for ( int i = 0; i < d->tabs->count(); i++ ) {
    if ( d->tabs->tabData(i).value<QWidget*>() == view ) {
      // remove file separators
      QString cap = view->caption();
      int pos = cap.findRev('/');
      cap = cap.mid( pos+1 );
      pos = cap.findRev('\\');
      cap = cap.mid( pos+1 );
      d->tabs->setTabText(i, cap );
      break;
    }
  }
}

MDIView* MainWindow::getWindowWithCaption( const QString& cap ) const
{
  for ( int i = 0; i < d->tabs->count(); i++ ) {
    MDIView* view = qobject_cast<MDIView*>(d->tabs->tabData(i).value<QWidget*>());
    if ( view->windowTitle() == cap ) {
      return view;
    }
  }

  return 0;
}

void MainWindow::onWindowDestroyed()
{
  QObject* view = (QObject*)sender();
  for ( int i = 0; i < d->tabs->count(); i++ )
  {
    if ( d->tabs->tabData(i).value<QWidget*>() == view )
    {
      d->tabs->removeTab( i );
      if ( d->tabs->count() == 0 )
        d->tabs->hide(); // no view open any more
      break;
    }
  }
}

void MainWindow::onWindowActivated( QWidget* w )
{
  MDIView* view = dynamic_cast<MDIView*>(w);

  // Even if windowActivated() signal is emitted mdi doesn't need to be a top-level window.
  // This happens e.g. if two windows are top-level and one of them gets docked again.
  // QWorkspace emits the signal then even though the other window is in front.
  // The consequence is that the docked window becomes the active window and not the undocked
  // window on top. This means that all accel events, menu and toolbar actions get redirected
  // to the (wrong) docked window.
  // But just testing whether the view is active and ignore it if not leads to other more serious problems -
  // at least under Linux. It seems to be a problem with the window manager.
  // Under Windows it seems to work though it's not really sure that it works reliably.
  // Result: So, we accept the first problem to be sure to avoid the second one.
  if ( !view /*|| !mdi->isActiveWindow()*/ ) 
    return; // either no MDIView or no valid object or no top-level window

  // set active the appropriate window (it needs not to be part of mdiIds, e.g. directly after creation)
  view->setActiveView();

  // set the appropriate tab to the new active window
  for ( int i = 0; i < d->tabs->count(); i++ )
  {
    if ( d->tabs->tabData(i).value<QWidget*>() == view )
    {
      d->tabs->blockSignals( true );
      d->tabs->setCurrentTab( i );
      d->tabs->blockSignals( false );
      break;
    }
  }
}

void MainWindow::onTabSelected( int i)
{
  QVariant var = d->tabs->tabData(i);
  if ( var.isValid() && var.canConvert<QWidget*>() ) {
    QWidget* view = var.value<QWidget*>();
    if ( view && !view->hasFocus() )
      view->setFocus();
  }
}

void MainWindow::onWindowsMenuAboutToShow()
{
  QList<QWidget*> windows = d->workspace->windowList( QWorkspace::CreationOrder );
  QWidget* active = d->workspace->activeWindow();

  // We search for the 'Std_WindowsMenu' command that provides the list of actions
  CommandManager& cMgr = Application::Instance->commandManager();
  Command* cmd = cMgr.getCommandByName("Std_WindowsMenu");
  QList<QAction*> actions = qobject_cast<ActionGroup*>(cmd->getAction())->actions();

  int numWindows = std::min<int>(actions.count()-1, windows.count());
  for ( int index = 0; index < numWindows; index++ ) {
    QWidget* child = windows.at(index);
    QAction* action = actions.at(index);
    QString text;
    if (index < 9)
      text = QString("&%1 %2").arg( index+1 ).arg(child->windowTitle());
    else
      text = QString("%1 %2").arg( index+1 ).arg(child->windowTitle());
    action->setText(text);
    action->setVisible(true);
    action->setChecked(child == active);
    connect(action, SIGNAL(triggered()), d->windowMapper, SLOT(map()));
    d->windowMapper->setMapping(action, child);
  }

  // if less windows than actions
  for (int index = numWindows; index < actions.count(); index++)
    actions[index]->setVisible(false);
  // show the separator
  if ( numWindows > 0 )
    actions.last()->setVisible(true);
}

QList<QWidget*> MainWindow::windows( QWorkspace::WindowOrder order ) const
{
  return d->workspace->windowList( order );
}

// set text to the pane
void MainWindow::setPaneText(int i, QString text)
{
  if (i==1)
    d->actionLabel->setText(text);
  else if (i==2)
    d->sizeLabel->setText(text);
}

//TODO Redo the active window stuff
MDIView* MainWindow::activeWindow(void) const
{/*
  // redirect all meesages to the view in fullscreen mode, if so
  MDIView* pView = 0;
  MDIView* pTmp = 0;
  for ( QMap<int, MDIView*>::Iterator it = d->mdiIds.begin(); it != d->mdiIds.end(); it++ )
  {
    if ( it.data()->isFullScreen() )
    {
      // store this in case we have a non-acitve view in fullscreen (e.g. opened dialog that is active)
      pTmp = it.data(); 
      if ( it.data()->isActiveWindow() )
        pView = it.data();
      break;
    }
  }
  // if no fullscreen window then look in workspace
  if ( !pView )
  {
    pView = reinterpret_cast <MDIView *> ( d->workspace->activeWindow() );
    if ( !pView && pTmp )
      pView = pTmp;
  }

  return pView;*/

  // the corresponding window to the current tab is active
  int id = d->tabs->currentIndex();
  if ( id < 0 )
    return 0;
  return qobject_cast<MDIView*>(d->tabs->tabData(id).value<QWidget*>());
}

void MainWindow::setActiveWindow( MDIView* view )
{
  d->workspace->setActiveWindow(view);
}

void MainWindow::closeEvent ( QCloseEvent * e )
{
  //TODO Reimplement the fullscreen mode stuff
  Application::Instance->tryClose( e );
  if( e->isAccepted() )
  {
    // Before closing the main window we must make sure that all views are in 'Normal' mode otherwise the 'lastWindowClosed()' signal
    // doesn't get emitted from QApplication later on. Just destroying these views doesn't work either.
    for ( int i = 0; i < d->tabs->count(); i++ )
    {
      MDIView* view = qobject_cast<MDIView*>(d->tabs->tabData(i).value<QWidget*>());
      if ( view->currentViewMode() != MDIView::Normal )
        view ->setCurrentViewMode(MDIView::Normal);
    }

    d->activityTimer->stop();
    QMainWindow::closeEvent( e );
  }
}

void MainWindow::appendRecentFile(const char* file)
{
  StdCmdRecentFiles* pCmd = dynamic_cast<StdCmdRecentFiles*>(Application::Instance->commandManager().getCommandByName("Std_RecentFiles"));
  if (pCmd)
  {
    pCmd->addRecentFile( file );
    pCmd->refreshRecentFileList();
  }
}

void MainWindow::updateActions()
{
  static QTime cLastCall;

  if ( cLastCall.elapsed() > 250 && isVisible() )
  {
    Application::Instance->commandManager().testActive();
    pcTree->testStatus();
    // remember last call
    cLastCall.start();
  }

  d->activityTimer->start( 300, true );	
}

void MainWindow::switchToTopLevelMode()
{
  QList<QDockWidget*> dw = this->findChildren<QDockWidget*>();
  for (QList<QDockWidget*>::Iterator it = dw.begin(); it != dw.end(); ++it) {
    (*it)->setParent(0, Qt::Window);
    (*it)->show();
  }
  QList<QWidget*> mdi = getMainWindow()->windows();
  for (QList<QWidget*>::Iterator it = mdi.begin(); it != mdi.end(); ++it) {
    (*it)->setParent(0, Qt::Window);
    (*it)->show();
  }
}

void MainWindow::switchToDockedMode()
{
  // TODO
  QMessageBox::critical(getMainWindow(), "Not implemented yet", "Not implemented yet");
}

void MainWindow::updateStyle(void)
{
  ParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp");
  hGrp = hGrp->GetGroup("Preferences")->GetGroup("General");

  QStyle* curStyle = QApplication::style();
  QString curStyleName = curStyle->objectName();
  QString newStyleName = hGrp->GetASCII( "WindowStyle", curStyleName.ascii() ).c_str();
  curStyleName = curStyleName.toLower();
  newStyleName = newStyleName.toLower();
  if ( curStyleName == newStyleName )
    return; // already set
  
  QStyle* newStyle = QStyleFactory::create( newStyleName );
  if ( newStyle != 0 )
  {
    QApplication::setStyle( newStyle );
  }
}

void MainWindow::loadWindowSettings()
{
  loadLayoutSettings();

  ParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("MainWindow");
  int w = hGrp->GetInt("Width", 1024);
  int h = hGrp->GetInt("Height", 768);
  int x = hGrp->GetInt("PosX", pos().x());
  int y = hGrp->GetInt("PosY", pos().y());
  bool max = hGrp->GetBool("Maximized", false);
  resize( w, h );
  move(x, y);
  if (max) showMaximized();

  updateStyle();
}

void MainWindow::saveWindowSettings()
{
  ParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("MainWindow");
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

  saveLayoutSettings();
}

void MainWindow::loadLayoutSettings()
{
  std::string path = App::GetApplication().Config()["UserHomePath"];
  std::string name = App::GetApplication().Config()["ExeName"];
  if(getenv("APPDATA"))
    path = std::string(getenv("APPDATA"));
  else
    name = std::string(".") + name;

  QByteArray state;
  QDir dir(path.c_str());
  if (dir.exists() && dir.cd(name.c_str())) {
    QString filename = dir.filePath("layout.bin");
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
      QDataStream str(&file);
      str >> state;
      file.close();
    }
  }

  QString hidden = App::Application::Config()["HiddenDockWindow"].c_str();
  QStringList hiddenDW = QStringList::split ( ';', hidden, false );
  DockWindowManager::instance()->hideDockWindows( hiddenDW );

  restoreState(state);
}

void MainWindow::saveLayoutSettings()
{
  std::string path = App::GetApplication().Config()["UserHomePath"];
  std::string name = App::GetApplication().Config()["ExeName"];
  if(getenv("APPDATA"))
    path = std::string(getenv("APPDATA"));
  else
    name = std::string(".") + name;

  QDir dir(path.c_str());

  if (dir.exists()) {
    if (!dir.exists(name.c_str()))
      dir.mkdir(name.c_str());
    if (dir.cd(name.c_str())) {
      QString filename = dir.filePath("layout.bin");
      QFile file(filename);
      if (file.open(QIODevice::WriteOnly)) {
        QByteArray state = saveState();
        QDataStream str(&file);
        str << state;
        file.close();
      }
    }
  }
}

void MainWindow::startSplasher(void)
{
  // startup splasher
  // when running in verbose mode no splasher
  if ( ! (App::Application::Config()["Verbose"] == "Strict") && (App::Application::Config()["RunMode"] == "Gui") )
  {
    ParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("Preferences")->GetGroup("General");
#ifdef FC_DEBUG
  bool splash = false;
#else
  bool splash = true;
#endif
    if (hGrp->GetBool("AllowSplasher", splash))
    {
      d->splashscreen = new SplashScreen( Gui::BitmapFactory().pixmap(App::Application::Config()["SplashPicture"].c_str()) );
      d->splashscreen->show();
    }else
      d->splashscreen = 0;


  }
}

void MainWindow::stopSplasher(void)
{
  if ( d->splashscreen )
  {
    d->splashscreen->finish( this );
    delete d->splashscreen;
    d->splashscreen = 0;
  }
}

void MainWindow::showTipOfTheDay( bool force )
{
  // tip of the day?
  ParameterGrp::handle
  hGrp = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("Preferences")->GetGroup("General");

#ifdef FC_DEBUG
  bool tip = false;
#else
  bool tip = true;
#endif

  tip = hGrp->GetBool("Tipoftheday", tip);
  if ( tip || force)
  {
    Gui::Dialog::DlgTipOfTheDayImp dlg(instance);
    dlg.exec();
  }
}

/**
 * Drops the event \a e and tries to open the files.
 */
void MainWindow::dropEvent ( QDropEvent* e )
{
  const QMimeData* data = e->mimeData();
  if ( data->hasUrls() ) {
    QList<QUrl> uri = data->urls();
    for ( QList<QUrl>::ConstIterator it = uri.begin(); it != uri.end(); ++it ) {
      QFileInfo info((*it).toLocalFile());
      if ( info.exists() && info.isFile() ) {
        // First check the complete extension
        if ( App::GetApplication().hasOpenType( info.extension().latin1() ) )
          Application::Instance->open(info.absFilePath().latin1());
        // Don't get the complete extension
        else if ( App::GetApplication().hasOpenType( info.extension(false).latin1() ) )
          Application::Instance->open(info.absFilePath().latin1());
      }
    }
  } else {
    QMainWindow::dropEvent(e);
  }
}

void MainWindow::dragEnterEvent ( QDragEnterEvent * e )
{
  // Here we must allow uri drafs and check them in dropEvent
  const QMimeData* data = e->mimeData();
  if ( data->hasUrls() )
    e->accept();
  else
    e->ignore();
}

void MainWindow::changeEvent(QEvent *e)
{
  if (e->type() == QEvent::LanguageChange) {
    d->sizeLabel->setText(tr("Dimension"));
    
    CommandManager& rclMan = Application::Instance->commandManager();
    vector<Command*> cmd = rclMan.getAllCommands();
    for ( vector<Command*>::iterator it = cmd.begin(); it != cmd.end(); ++it )
      (*it)->languageChange();

    //MenuManager::getInstance()->languageChange();
    // reload current workbench to translate root items of submenus
    Workbench* wb = WorkbenchManager::instance()->active();
    if (wb) wb->activate();
  } else {
    QMainWindow::changeEvent(e);
  }
}


#include "moc_MainWindow.cpp"
