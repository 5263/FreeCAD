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


#ifndef APPLICATION_H
#define APPLICATION_H


#ifndef _PreComp_
# include <string>
# include <vector>
# include <qmainwindow.h>
# include <qworkspace.h>
#endif

#define  putpix()

#include "../Base/Console.h"
#include "../App/Application.h"


class FCGuiDocument;
class QComboBox;
class FCWindow;
class QToolBar;
class FCBaseView;
class QPopupMenu;
class QToolBar;
class FCViewBar;
class MDIView;
class QSplashScreen;

namespace Gui{
class CustomWidgetManager;
class CommandManager;
class MacroManager;
namespace DockWnd {
class HelpView;
} //namespace DockWnd
} //namespace Gui

/** The Applcation main class
 *  This is the central class of the GUI 
 */
class GuiExport ApplicationWindow: public QMainWindow, public App::ApplicationObserver
{
    Q_OBJECT
 
	
public:
	/// construction
    ApplicationWindow();
	/// destruction
    ~ApplicationWindow();

	void CreateStandardOperations();

	// Observer
  virtual void OnDocNew(App::Document* pcDoc);
  virtual void OnDocDelete(App::Document* pcDoc);

  void addWindow( MDIView* view );
  QWidgetList windows( QWorkspace::WindowOrder order = QWorkspace::CreationOrder ) const;

	/// message when a GuiDocument is about to vanish
	void OnLastWindowClosed(FCGuiDocument* pcDoc);

	/// some kind of singelton
	static ApplicationWindow* Instance;

	/** @name methodes for View handling */
	//@{
	/// send Messages to the active view
	bool SendMsgToActiveView(const char* pMsg);
	/// send Messages test to the active view
	bool SendHasMsgToActiveView(const char* pMsg);
	/// returns the active view or NULL
	MDIView* GetActiveView(void);
	/// Geter for the Active View
	FCGuiDocument* GetActiveDocument(void);
	/// Attach a view (get called by the FCView constructor)
	void AttachView(FCBaseView* pcView);
	/// Detach a view (get called by the FCView destructor)
	void DetachView(FCBaseView* pcView);
	/// get calld if a view gets activated, this manage the whole activation scheme
	void ViewActivated(MDIView* pcView);
	/// call update to all docuemnts an all views (costly!)
	void Update(void);
	/// call update to all views of the active document
	void UpdateActive(void);
	/// call update to the pixmaps' size
	void UpdatePixmapsSize(void);
	/// call update to style
	void UpdateStyle(void);
	//@}

	/// Set the active document
	void SetActiveDocument(FCGuiDocument* pcDocument);

	/// true when the application shuting down
	bool IsClosing(void);

	/// Reference to the command manager
  Gui::CommandManager &GetCommandManager(void);
	
	/// Returns the widget manager
  Gui::CustomWidgetManager* GetCustomWidgetManager(void);

	/** @name status bar handling */
	//@{	
	/// set text to the pane
	void setPaneText(int i, QString text);
	//@}

	/** @name workbench handling */
	//@{	
	/// Activate a named workbench
	void ActivateWorkbench(const char* name);
	/// update the combo box when there are changes in the workbenches
	void UpdateWorkbenchEntrys(void);
	/// returns the name of the active workbench
	QString GetActiveWorkbench(void);
	std::vector<std::string> GetWorkbenches(void);
	//@}

  /// MRU: recent files
  void AppendRecentFile(const char* file);

	/// Get macro manager
  Gui::MacroManager *GetMacroMngr(void);


public:
	/** @name Init, Destruct an Access methodes */
	//@{
	static void InitApplication(void);
	static void RunApplication(void);
	static void StartSplasher(void);
	static void StopSplasher(void);
	static void ShowTipOfTheDay(bool force=false);
	static void Destruct(void);

	//@}

private:
	static 	QApplication* _pcQApp ;
	static 	QSplashScreen *_splash;


public:
	//---------------------------------------------------------------------
	// python exports goes here +++++++++++++++++++++++++++++++++++++++++++	
	//---------------------------------------------------------------------

	// static python wrapper of the exported functions
	PYFUNCDEF_S(sMenuAppendItems); // append items
	PYFUNCDEF_S(sMenuRemoveItems); // remove items
	PYFUNCDEF_S(sMenuDelete);      // delete the whole menu

	PYFUNCDEF_S(sToolbarAppendItems);
	PYFUNCDEF_S(sToolbarRemoveItems);
	PYFUNCDEF_S(sToolbarDelete);

	PYFUNCDEF_S(sCommandbarAppendItems);
	PYFUNCDEF_S(sCommandbarRemoveItems);
	PYFUNCDEF_S(sCommandbarDelete);

	PYFUNCDEF_S(sWorkbenchAdd);
	PYFUNCDEF_S(sWorkbenchDelete);
	PYFUNCDEF_S(sWorkbenchActivate);
	PYFUNCDEF_S(sWorkbenchGet);

	PYFUNCDEF_S(sSendActiveView);

	PYFUNCDEF_S(sUpdateGui);
	PYFUNCDEF_S(sCreateDialog);

	PYFUNCDEF_S(sRunCommand);
	PYFUNCDEF_S(sCommandAdd);

	static PyMethodDef    Methods[]; 
 

signals:
	void sendQuit();
  void timeEvent();

public:
  void Polish();
  bool isCustomizable () const;
  void customize ();
  void tileHorizontal();
  void tile();
  void cascade();

protected: // Protected methods
	virtual void closeEvent ( QCloseEvent * e );
	/// waiting cursor stuff 
	void timerEvent( QTimerEvent * e){emit timeEvent();}

  // windows stuff
  void LoadWindowSettings();
  void SaveWindowSettings();
  void LoadDockWndSettings();
  void SaveDockWndSettings();

public slots:
	/// this slot get frequently activatet and test the commands if they are still active
	void UpdateCmdActivity();
	/** @name methodes for the UNDO REDO handling 
	 *  this methodes are usaly used by the GUI document! Its not intended
	 *  to use them directly. If the GUI is not up, there is usaly no UNDO / REDO 
	 *  nececary.
	 */
	//@{
	void OnUndo();
	void OnRedo();
	//@}
protected slots:
  virtual void languageChange();

private slots:
  void OnShowView();
  void onWindowActivated( QWidget* );
  void onWindowsMenuAboutToShow();
  void onWindowsMenuActivated( int id );
  void onWindowDestroyed();
  void onToggleStatusBar();
  void onTabSelected( int i);

private:
  struct ApplicationWindowP* d;
};



/** The console observer for the Application window
 *  This class distribute the Messages issued on the console
 *  to the status bar. 
 *  @see FCConsole
 *  @see FCConsoleObserver
 */
 class GuiConsoleObserver: public Base::ConsoleObserver
{
public:
	GuiConsoleObserver(ApplicationWindow *pcAppWnd);
		
	/// get calles when a Warning is issued
	virtual void Warning(const char *m);
	/// get calles when a Message is issued
	virtual void Message(const char * m);
	/// get calles when a Error is issued
	virtual void Error  (const char *m);
	/// get calles when a Log Message is issued
	virtual void Log    (const char *);

	/// Mutes the Observer, no Dialog Box will appear
	static bool bMute;
protected:
	ApplicationWindow* _pcAppWnd;
};

#endif
