/***************************************************************************
                          PrefWidgets.h  -  description
                             -------------------
    begin                : 2002/12/20 10:47:44
    copyright            : (C) 2002 by Werner Mayer
    email                : werner.wm.mayer@gmx.de
 ***************************************************************************/

/** \file PrefWidgets.h
 *  \brief A set of FreeCAD widgets
 *  \author Werner Mayer
 *  \version 0.1
 *  \date    2003/01/06
 */


/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *   for detail see the LICENCE text file.                                 *
 *   Werner Mayer 2002                                                     *
 *                                                                         *
 ***************************************************************************/
 

#ifndef __FC_PREF_WIDGETS_H__
#define __FC_PREF_WIDGETS_H__

#include "../App/Application.h"
#include "../Base/Parameter.h"
#include "Command.h"

#include "qextmdi/qextmdimainfrm.h"
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qlistbox.h>
#include <qstring.h>
#include <qslider.h>

// forward declarations
class FCWidgetPrefsHandler;
class QDoubleValidator;
class FCWidgetFactorySupplier;
class QAction;
class FCStackBar;
class FCDockWindow;

/** The widget preference class
 *  If you want to extend a QWidget class to save/restore data
 *  you just have to derive from this class and implement
 *  the both methods @ref restorePreferences() and
 *  @ref savePreferences()
 *  @see FCWidgetPrefsHandler
 *  @see FCWidgetFactorySupplier
 */
class FCWidgetPrefs : public FCParameterGrp::ObserverType
{
  public:
    /// changes the preference name
    virtual void setEntryName(QString name);
    /// get the widget's name in preferences
    QString getEntryName() const;
    /// changes the preference path
    virtual void setParamGrpPath(QString name);
    /// get the widget's preferences path
    QString getParamGrpPath() const;
    /// return the handler
    FCWidgetPrefsHandler* getHandler();
    /// install a new handler
    void installHandler(FCWidgetPrefsHandler*);
    /// get the handle to the parameter group
    FCParameterGrp::handle getParamGrp();
    /// observers method
    virtual void OnChange(FCSubject<const char*> &rCaller, const char * sReason);
    /// get the handle to the root parameter group
    static FCParameterGrp::handle getRootParamGrp();

  protected:
    /// restore the preferences
    virtual void restorePreferences() = 0;
    /// save the preferences
    virtual void savePreferences()    = 0;
    /// constructor
    FCWidgetPrefs(const char * name = 0, bool bInstall=true);
    /// destructor
    virtual ~FCWidgetPrefs();

    /// changes the preference name
    void setPrefName(QString pref);
    FCParameterGrp::handle hPrefGrp;

  private:
    FCWidgetPrefsHandler* pHandler;
    QString m_sPrefName;
    QString m_sPrefGrp;

    // friends
    friend class FCWidgetFactorySupplier;
    friend class FCWidgetPrefsHandler;
};

/** The preference handler class
 *  This class allows you to connect the slots
 *  @ref Save() and @ref Restore() 
 *  to signals of your classes.
 *  @see FCWidgetPrefs
 */
class FCWidgetPrefsHandler : public QObject
{
  Q_OBJECT

  protected:
    FCWidgetPrefsHandler(FCWidgetPrefs* p);

  public slots:
    /// save
    virtual void onSave();
    /// restore
    virtual void onRestore();

  signals:
    void saved();
    void restored();

  protected:
    FCWidgetPrefs* pPref;

    //friends
    friend class FCWidgetPrefs;
		friend class FCWidgetPrefsManager;
};

class FCWidgetPrefsManager
{
  public:
    std::vector<FCWidgetPrefsHandler*> getHandlers()
    {
      return m_aHandlers;
    }

		void append(FCWidgetPrefsHandler* handler)
		{
			if (handler)
			{
#ifdef FC_DEBUG
				if (handler->pPref->getParamGrp().IsNull())
					throw;
#endif
				handler->onRestore();
				m_aHandlers.push_back(handler);
			}
		}

  protected:
    std::vector<FCWidgetPrefsHandler*> m_aHandlers;
};

/** The FCEditSpinBox class
 */
class FCEditSpinBox : public QSpinBox, public FCWidgetPrefs
{
    Q_OBJECT

    Q_PROPERTY( int    accuracy   READ getAccuracy      WRITE setAccuracy      )
    Q_PROPERTY( double valueFloat READ getValueFloat    WRITE setValueFloat    )
    Q_PROPERTY( double valueMax   READ getMaxValueFloat WRITE setMaxValueFloat )
    Q_PROPERTY( double valueMin   READ getMinValueFloat WRITE setMinValueFloat )
    Q_PROPERTY( QString prefEntry READ getEntryName     WRITE setEntryName     )
    Q_PROPERTY( QString prefPath  READ getParamGrpPath  WRITE setParamGrpPath  )

  public:
    FCEditSpinBox ( QWidget * parent = 0, const char * name = 0 );
    virtual ~FCEditSpinBox();

    // PROPERTIES
    // getters
    int    getAccuracy      () const;
    double getValueFloat    () const;
    double getMinValueFloat () const;
    double getMaxValueFloat () const;
    QString getEntryName    () const;
    QString getParamGrpPath () const;
    // setters
    void  setAccuracy      (int    accur);
    void  setValueFloat    (double value);
    void  setMinValueFloat (double value);
    void  setMaxValueFloat (double value);
    void  setEntryName     (QString name);
    void  setParamGrpPath  (QString name);

  protected:
    // restore from/save to parameters
    void restorePreferences();
    void savePreferences();
    virtual QString mapValueToText(int value);
    virtual int mapTextToValue(bool* ok);
    virtual void valueChange();
    void stepChange();

  signals:
    void valueFloatChanged(double value);

  private:
    QDoubleValidator* m_pValidator;
    int               m_iAccuracy;
    double            m_fDivisor;
    double            m_fEpsilon;
};

/**
 *
 */
class FCLineEdit : public QLineEdit, public FCWidgetPrefs
{
    Q_OBJECT

    Q_PROPERTY( QString prefEntry READ getEntryName     WRITE setEntryName     )
    Q_PROPERTY( QString prefPath  READ getParamGrpPath  WRITE setParamGrpPath  )

  public:
    FCLineEdit ( QWidget * parent = 0, const char * name = 0 );
    virtual ~FCLineEdit();

    // PROPERTIES
    // getters
    QString getEntryName    () const;
    QString getParamGrpPath () const;
    // setters
    void  setEntryName     (QString name);
    void  setParamGrpPath  (QString name);

  protected:
    // restore from/save to parameters
    void restorePreferences();
    void savePreferences();
};

/**
 *
 */
class FCComboBox : public QComboBox, public FCWidgetPrefs
{
    Q_OBJECT

    Q_PROPERTY( QString prefEntry READ getEntryName     WRITE setEntryName     )
    Q_PROPERTY( QString prefPath  READ getParamGrpPath  WRITE setParamGrpPath  )

  public:
    FCComboBox ( QWidget * parent = 0, const char * name = 0 );
    virtual ~FCComboBox();

    // PROPERTIES
    // getters
    QString getEntryName    () const;
    QString getParamGrpPath () const;
    // setters
    void  setEntryName     (QString name);
    void  setParamGrpPath  (QString name);

  protected:
    // restore from/save to parameters
    void restorePreferences();
    void savePreferences();
};

/**
 *
 */
class FCListBox : public QListBox, public FCWidgetPrefs
{
    Q_OBJECT

    Q_PROPERTY( QString prefEntry READ getEntryName     WRITE setEntryName     )
    Q_PROPERTY( QString prefPath  READ getParamGrpPath  WRITE setParamGrpPath  )

  public:
    FCListBox ( QWidget * parent = 0, const char * name = 0, WFlags f = 0 );
    virtual ~FCListBox();

    // PROPERTIES
    // getters
    QString getEntryName    () const;
    QString getParamGrpPath () const;
    // setters
    void  setEntryName     (QString name);
    void  setParamGrpPath  (QString name);

  protected:
    // restore from/save to parameters
    void restorePreferences();
    void savePreferences();
};

/**
 *
 */
class FCCheckBox : public QCheckBox, public FCWidgetPrefs
{
    Q_OBJECT

    Q_PROPERTY( QString prefEntry READ getEntryName     WRITE setEntryName     )
    Q_PROPERTY( QString prefPath  READ getParamGrpPath  WRITE setParamGrpPath  )

  public:
    FCCheckBox ( QWidget * parent = 0, const char * name = 0 );
    virtual ~FCCheckBox();

    // PROPERTIES
    // getters
    QString getEntryName    () const;
    QString getParamGrpPath () const;
    // setters
    void  setEntryName     (QString name);
    void  setParamGrpPath  (QString name);

  protected:
    // restore from/save to parameters
    void restorePreferences();
    void savePreferences();
};

/**
 *
 */
class FCRadioButton : public QRadioButton, public FCWidgetPrefs
{
    Q_OBJECT

    Q_PROPERTY( QString prefEntry READ getEntryName     WRITE setEntryName     )
    Q_PROPERTY( QString prefPath  READ getParamGrpPath  WRITE setParamGrpPath  )

  public:
    FCRadioButton ( QWidget * parent = 0, const char * name = 0 );
    virtual ~FCRadioButton();

    // PROPERTIES
    // getters
    QString getEntryName    () const;
    QString getParamGrpPath () const;
    // setters
    void  setEntryName     (QString name);
    void  setParamGrpPath  (QString name);

  protected:
    // restore from/save to parameters
    void restorePreferences();
    void savePreferences();
};

/**
 *
 */
class FCSlider : public QSlider, public FCWidgetPrefs
{
    Q_OBJECT

    Q_PROPERTY( QString prefEntry READ getEntryName     WRITE setEntryName     )
    Q_PROPERTY( QString prefPath  READ getParamGrpPath  WRITE setParamGrpPath  )

  public:
    FCSlider ( QWidget * parent = 0, const char * name = 0 );
    virtual ~FCSlider();

    // PROPERTIES
    // getters
    QString getEntryName    () const;
    QString getParamGrpPath () const;
    // setters
    void  setEntryName     (QString name);
    void  setParamGrpPath  (QString name);

  protected:
    // restore from/save to parameters
    void restorePreferences();
    void savePreferences();
};

/**
 *  Class to drag a 'QAction' object
 */
class FCActionDrag : public QStoredDrag
{
  public:
    FCActionDrag ( QString action, QWidget * dragSource = 0, const char * name = 0 );
    virtual ~FCActionDrag ();

    static bool canDecode ( const QMimeSource * e );
    static bool decode ( const QMimeSource * e, QString&  action );

  public:
    static std::vector<QString> actions;
};

class FCCustomWidget : public FCWidgetPrefs
{
  public:
    bool hasCustomItems();
    std::vector<std::string> getItems();
    void setItems(const std::vector<std::string>& items);
    void addItem(const std::string& item);
    void loadXML();
    void saveXML();
    virtual void update(FCCommandManager& rclMgr) = 0;
    QString getWorkbench();
    virtual ~FCCustomWidget();

  protected:
    FCCustomWidget(const char* grp, const char * name);
    virtual void restorePreferences();
    virtual void savePreferences();
    void init(const char* grp, const char* name);

    std::vector<std::string> _clItems;
    QString                  _clWorkbench;
};

/**
 *  Toolbar class that knows 'drag and drop'
 */
class FCToolBar : public QToolBar, public FCCustomWidget
{
  Q_OBJECT

  public:
    FCToolBar ( const QString & label, QMainWindow *, QWidget *, bool newLine = FALSE, const char * name = 0, WFlags f = 0, const char* type = "Toolbars" );
    FCToolBar ( QMainWindow * parent = 0, const char * name = 0, const char* type = "Toolbars" );
    virtual ~FCToolBar();
    virtual void clearAll();
    void update(FCCommandManager& rclMgr);

  public:
    static bool isAllowed(QWidget* w);

  protected:
    void dropEvent ( QDropEvent * );
    void dragEnterEvent ( QDragEnterEvent * );
    void dragLeaveEvent ( QDragLeaveEvent * );
    void dragMoveEvent ( QDragMoveEvent * );
    virtual void restorePreferences();
    virtual void savePreferences();
    bool bSaveColor;
};

class FCPopupMenu : public QPopupMenu, public FCCustomWidget
{
  Q_OBJECT

  public:
    FCPopupMenu ( QWidget * parent=0, const char * name=0, const char* menu = 0 );
    virtual ~FCPopupMenu();
    void update(FCCommandManager& rclMgr);
    virtual void OnChange(FCSubject<const char*> &rCaller, const char * sReason);

  protected:
    void dropEvent ( QDropEvent * );
    void dragEnterEvent ( QDragEnterEvent * );
    void dragLeaveEvent ( QDragLeaveEvent * );
    void dragMoveEvent ( QDragMoveEvent * );
    void mouseMoveEvent ( QMouseEvent * );
    virtual void restorePreferences();
    virtual void savePreferences();
    QString parent;
    bool    bAllowDrag;
};

class FCCustomWidgetManager
{
  public:
    FCCustomWidgetManager(FCCommandManager& rclMgr, FCStackBar* pCmdBar);
    ~FCCustomWidgetManager();

    bool init(const char* workbench);
    bool update(const char* workbench);
    bool update();

    // toolbars
    FCToolBar* getToolBar(const char* name);
    std::vector<FCToolBar*> getToolBars();
    void delToolBar(const char* name);
    int countToolBars();

    // command bars
    FCToolBar* getCmdBar(const char* name);
    std::vector<FCToolBar*> getCmdBars();
    void delCmdBar(const char* name);
    int countCmdBars();

    // menus
    FCPopupMenu* getPopupMenu(const char* name, const char* parent = 0);
    std::vector<FCPopupMenu*> getPopupMenus();
    void delPopupMenu(const char* name);
    int countPopupMenus();

    // dockable windows
    FCDockWindow* getDockWindow(const char* name);
    std::vector<FCDockWindow*> getDockWindows();
    void delDockWindow(const char* name);
		void addDockWindow(const char* name,FCDockWindow *pcDocWindow, const char* sCompanion = NULL,
                       KDockWidget::DockPosition pos = KDockWidget::DockRight, int percent = 50);

    void addPopupMenu (const std::string& type, const std::vector<std::string>& defIt, const char* parent = 0);
    void addPopupMenu (const std::string& type, const char* parent = 0);
    void addToolBar   (const std::string& type, const std::vector<std::string>& defIt);
    void addToolBar   (const std::string& type);
    void addCmdBar    (const std::string& type, const std::vector<std::string>& defIt);
    void addCmdBar    (const std::string& type);
    void addItem      (const std::string& item);

  protected:
    std::map <std::string,FCPopupMenu*> _clPopupMenus;
    std::map <std::string,FCToolBar*>   _clToolbars;
    std::map <std::string,FCToolBar*>   _clCmdbars;
	  std::map <std::string,FCDockWindow*>    _clDocWindows;
    std::vector<std::string>            _clDefaultItems;
    FCCommandManager&                   _clCmdMgr;
  	FCStackBar*                         _pclStackBar;
};

#endif // __FC_PREF_WIDGETS_H__
