/***************************************************************************
                          Widgets.h  -  description
                             -------------------
    begin                : 2002/12/20 10:47:44
    copyright            : (C) 2002 by Werner Mayer
    email                : werner.wm.mayer@gmx.de
 ***************************************************************************/

/** \file Widgets.h
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
 

#ifndef __FC_WIDGETS_H__
#define __FC_WIDGETS_H__
#include "Window.h"
#include <qprogressbar.h>
#include <qlabel.h>
#include <qiconview.h>
#include <qdragobject.h>
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qfiledialog.h>
#include <qvariant.h>
#include <qdialog.h>
#if QT_VERSION > 230
# include <qlistview.h>
# include <qlineedit.h>
#endif

class QHBoxLayout; 
class QTime;

/**
 * Using the Qt's open/save dialogs with own adjustments
 * In this implementation a waitcursor will additionally 
 * be set.
 * @see FCAutoWaitCursor
 */
class GuiExport FCFileDialog
{
  public:
    static QString getOpenFileName( const QString &initially = QString::null,
				                            const QString &filter = QString::null,
				                            QWidget *parent = 0, const char* name = 0 );
    static QString getOpenFileName( const QString &initially, const QString &filter,
				                            QWidget *parent, const char* name, const QString& caption );
    static QString getSaveFileName ( const QString & initially = QString::null, 
                                     const QString & filter = QString::null, QWidget * parent = 0, 
                                     const char * name = 0 );
    static QString getSaveFileName ( const QString & initially, const QString & filter, 
                                     QWidget * parent, const char * name, const QString & caption );
};

/**  
 * FreeCAD's progressbar for long operations
 * If you call @ref Start() several times without calling
 * @ref Stop() before the number of new steps will be added 
 * to the number of current steps, i.e. nevertheless the 
 * progress bar will run only once.
 */
class FCProgressBar : public QProgressBar
{
  public:
    /// construction
    FCProgressBar ( QWidget * parent=0, const char * name=0, WFlags f=0 );
    /** Starts the progress bar */
    void Start(QString txt, int steps, bool& flag);
    /** Does the next step */
    void Next();
    /** Stops the sequencer */
    void Stop ();

  private:
	  /** @name for internal use only */
    //@{
    /** Checks if the ESC button was pressed 
     *  If ESC @ref interrupt() is called.
     */
    bool isInterrupted();
    /**  
     * Throws an exception to stop the pending operation.
     */
    void interrupt();
    /** Draws the content of the progress bar */
    virtual void drawContents( QPainter *p );
    /** Reimplemented */
    bool setIndicator ( QString & indicator, int progress, int totalSteps );
    //@}
    bool bSeveralInstances;
    int iStep;
    int iTimeStep;
    int iTotalTime;
    int iFirstTotal;
    int iStartedProgresses;
    QTime   measureTime;
    QString remainingTime;
};

/**
 * List view class
 * This class also has a method to get the last
 * element of the tree. @see lastItem()
 */
class FCListView : public QListView
{
  Q_OBJECT

  public:
    FCListView ( QWidget * parent, const char * name, WFlags f );
    FCListView ( QWidget * parent = 0, const char * name = 0 );
    virtual ~FCListView (); 

    QListViewItem * lastItem () const;
    static QListViewItem * lastItem (QListView*);
};

/**
 *  Icon items used by the 'FCCmdView' and 'FCDlgCustomize' classes
 */
class FCCmdViewItem : public QIconViewItem
{
  public:
    FCCmdViewItem ( QIconView * parent, QString Action, QAction* pAct );
    virtual ~FCCmdViewItem ();

    QString text() const;
    QString GetAction();

  protected:
    QString sAction;
    QString description;
};

/**
 * Icon view class
 * This class allows to drag one or more items which
 * correspond to a FreeCAD command. The dragged items you
 * can drop onto a @ref FCToolBar object
 * @see FCCmdViewItem, FCCommand
 */
class FCCmdView : public QIconView
{
  Q_OBJECT

  public:
    FCCmdView ( QWidget * parent = 0, const char * name = 0, WFlags f = 0 );
    virtual ~FCCmdView ();

  protected:
    /** Allows to drag an item */
    QDragObject * dragObject ();

  protected slots:
    /// for internal use only
    void slotSelectionChanged(QIconViewItem * item);

  signals:
    void emitSelectionChanged(QString);
};

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;

/**
 *  Toolbar/command bar creation class
 */
class FCDlgCreateToolOrCmdBar : public QDialog
{ 
    Q_OBJECT

  public:
    FCDlgCreateToolOrCmdBar( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FCDlgCreateToolOrCmdBar();

    QGroupBox* GroupBox1;
    QLabel* TextLabel;
    QLineEdit* LineEditName;
    QCheckBox* CheckCreateCmdBar;
    QCheckBox* CheckCreateToolBar;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

  protected slots:
    void accept ();

  protected:
    QGridLayout* FCDlgCreateToolOrCmdBarLayout;
    QGridLayout* GroupBox1Layout;
    QHBoxLayout* Layout2;
};

/**
 * Line accelerator
 * A special implementation of QLineEdit useful
 * for pressing shortcuts.
 */
class FCAccelLineEdit : public QLineEdit
{
  public:
    FCAccelLineEdit ( QWidget * parent, const char * name=0 );

  protected:
     void keyPressEvent ( QKeyEvent * e);
};

/**
 *  A dialog having a checklist inside.
 */
class FCCheckListDlg : public QDialog
{ 
    Q_OBJECT

  public:
    FCCheckListDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FCCheckListDlg();

    /** set all items to be shown in the list */
    void setItems(const std::vector<std::string>& items);
    /** get all checked items */
    std::vector<int> getCheckedItems(); 
    void show ();
    void hide ();

  protected:
    std::vector<std::string> items;
    std::vector<int> checked;

    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QGroupBox* GroupBox1;
    QListView* ListView;
    QGridLayout* FCCheckListDlgLayout;
    QHBoxLayout* Layout2;
    QGridLayout* GroupBox1Layout;
};

/**
 *  Implementation of a color button
 */
class FCColorButton : public QButton
{
    Q_OBJECT

    Q_PROPERTY( QColor color READ color WRITE setColor )
 
 public:
    /// construction
    FCColorButton( QWidget* parent = 0, const char* name = 0 );
    FCColorButton( const QBrush& b, QWidget* parent = 0, const char* name = 0, WFlags f = 0 );
    /// destruction
    ~FCColorButton();

    /** set the color */
    void setColor( const QColor& );
    /** get the current color */
    QColor color() const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    
  public slots:
    /** opens the @ref QColorDialog dialog to select a new color */
    virtual void onChooseColor();

  signals:
    /** emits this signal when color is changed */
    void changed();

  protected:
    /// for internal use only
    void drawButton( QPainter* );
    /// for internal use only
    void drawButtonLabel( QPainter* );

  private:
    QColor col;
};

#endif // __FC_WIDGETS_H__
