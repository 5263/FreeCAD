/***************************************************************************
                          Widgets.cpp  -  description
                             -------------------
    begin                : 2002/12/20 10:47:44
    copyright            : (C) 2002 by Werner Mayer
    email                : werner.wm.mayer@gmx.de
 ***************************************************************************/

/** \file Widgets.cpp
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



#include "PreCompiled.h"

#ifndef _PreComp_
#	include <qstring.h>
#	include <qurl.h>
#	include <ctype.h>
#endif

#include "Widgets.h"
#include "Application.h"
#include "PrefWidgets.h"
#include "../Base/Exception.h"

#include <qlayout.h>
#include <qdatetime.h>

#if QT_VERSION > 230
# include <qstyle.h>
#endif


QString FCFileDialog::getOpenFileName( const QString & startWith, const QString& filter,
                          				     QWidget *parent, const char* name )
{
    return getOpenFileName( startWith, filter, parent, name, QString::null  );
}

QString FCFileDialog::getOpenFileName( const QString & startWith, const QString& filter, QWidget *parent, 
                                       const char* name, const QString& caption )
{
  QString file = QFileDialog::getOpenFileName( startWith, filter, parent, name, caption );

  // set wait cursor because on windows OS this method blocks the QApplication object
  if (!file.isEmpty())
    FCAutoWaitCursor::Instance().SetWaitCursor();
  return file;
}

QString FCFileDialog::getSaveFileName( const QString & startWith, const QString& filter,
                          				     QWidget *parent, const char* name )
{
    return getSaveFileName( startWith, filter, parent, name, QString::null  );
}

QString FCFileDialog::getSaveFileName( const QString & startWith, const QString& filter, QWidget *parent, 
                                       const char* name, const QString& caption )
{
  QString file = QFileDialog::getSaveFileName( startWith, filter, parent, name, caption );

  // set wait cursor because on windows OS this method blocks the QApplication object
  if (!file.isEmpty())
    FCAutoWaitCursor::Instance().SetWaitCursor();
  return file;
}

QString FCFileDialog::getSaveFileName ( const QString & initially, const QString & filter, QWidget * parent, 
                                        const QString & caption )
{
  FCFileDialog fd(AnyFile, initially, filter, parent, "Save Dialog", true);
  fd.setCaption(caption);
  fd.exec();
  return fd.selectedFileName();
}

FCFileDialog::FCFileDialog (Mode mode, QWidget* parent, const char* name, bool modal)
: QFileDialog(parent, name, modal)
{
}

FCFileDialog::FCFileDialog (Mode mode, const QString& dirName, const QString& filter, 
                            QWidget* parent, const char* name, bool modal)
 : QFileDialog(dirName, filter, parent, name, modal)
{
  setMode(mode);
}

FCFileDialog::~FCFileDialog()
{
}

void FCFileDialog::accept()
{
  QString fn = selectedFileName();

  if (QFile(fn).exists() && mode() == AnyFile)
  {
    char szBuf[200];
    sprintf(szBuf, "'%s' already exists.\nReplace existing file?", fn.latin1());
    if (QMessageBox::information(this, "Existing file", szBuf, "Yes", "No", QString::null, 1) == 0)
      QFileDialog::accept();
  }
  else
    QFileDialog::accept();
}

QString FCFileDialog::selectedFileName()
{
  QString fn = selectedFile();

  // if empty do nothing
  if (fn.isEmpty())
    return fn;

  // search for extension
  int pos = fn.findRev('.');
  if (pos == -1)
  {
    // try to figure out the selected filter
    QString filt = selectedFilter();
    int dot = filt.find('*');
    int blank = filt.find(' ', dot);
    int brack = filt.find(')', dot);
    if (dot != -1 && blank != -1)
    {
      QString sub = filt.mid(dot+1, blank-dot-1);
      fn = fn + sub;
    }
    else if (dot != -1 && brack != -1)
    {
      QString sub = filt.mid(dot+1, brack-dot-1);
      fn = fn + sub;
    }
  }

  return fn;
}

///////////////////////////////////////////////////////////////////////////////////
class FCProgressBarPrivate
{
  public:
    bool bSeveralInstances;
    int iStep;
    int iTimeStep;
    int iTotalTime;
    int iFirstTotal;
    int iStartedProgresses;
    QTime   measureTime;
    QString remainingTime;
};

FCProgressBar::FCProgressBar ( QWidget * parent, const char * name, WFlags f )
: QProgressBar (parent, name, f)
{
  d = new FCProgressBarPrivate;

  setFixedWidth(120);
  // this style is very nice ;-)
  setIndicatorFollowsStyle(false);
  // update the string after timestep steps
  d->iTimeStep = 1;
  // so far there are no sequencer started
  d->iStartedProgresses = 0;
  d->bSeveralInstances = false;
}

FCProgressBar::~FCProgressBar ()
{
  delete d;
}

// NOTE: for each call of this method you must call the
//       corresponding stop method
void FCProgressBar::Start(QString txt, int steps/*, bool& flag*/)
{
  // increment the size called this method
  d->iStartedProgresses++;

  // if you call this method in a for loop
  // the steps size is regarded only once 
//  if (!flag || steps == 0)
//    return;
//
//  flag = false;

  // several sequencer started
  if (d->iStartedProgresses > 1)
  {
    d->bSeveralInstances = true;
    steps *= totalSteps();
    setTotalSteps(steps);
    d->iTimeStep = totalSteps() / d->iFirstTotal; 
    d->measureTime.restart();
  }
  else
  {
    d->iTotalTime = 0;
    d->iStep = 0;
    d->remainingTime = "";
    d->iFirstTotal = steps;
    setTotalSteps(steps);
    d->measureTime.start();
  }

  // print message to the satusbar
  if (d->iStartedProgresses == 1)
    ApplicationWindow::Instance->statusBar()->message(txt);
}

void FCProgressBar::Next()
{
  if (!isInterrupted())
    setProgress(d->iStep++);
}

void FCProgressBar::Stop ()
{
  d->iStartedProgresses--;
  if (d->iStartedProgresses == 0)
  {
    Reset();
    d->bSeveralInstances = false;
  }
}

void FCProgressBar::Reset()
{
  reset();
  setTotalSteps(0);
  setProgress(-1);
}

bool FCProgressBar::isInterrupted()
{
#ifdef  FC_OS_WIN32
  MSG  tMSG;

  // Escape button pressed ?
  while (PeekMessage(&tMSG, NULL, WM_KEYDOWN, WM_KEYDOWN, PM_REMOVE) == TRUE)
  {
    TranslateMessage(&tMSG);
    if (tMSG.message == WM_KEYDOWN)
    {
      if (tMSG.wParam == VK_ESCAPE)
      {
        interrupt();
        return true;
      }
    }
  }
#endif

  return false;
}

void FCProgressBar::interrupt()
{
  //resets
  Reset();
  d->bSeveralInstances = false;
  d->iStartedProgresses = 0;

  FCGuiConsoleObserver::bMute = true;
  FCException exc("Aborting...");
  FCGuiConsoleObserver::bMute = false;
  throw exc;
}

void FCProgressBar::drawContents( QPainter *p )
{
  const int unit_width  = 9;
  const int unit_height = 12;
  const int total_steps = totalSteps();
  const int progress_val = progress();
  QString	progress_str;

  setIndicator ( progress_str, progress_val, total_steps );

  const QRect bar = contentsRect();

	if (total_steps) 
  {
	  int u = bar.width();
	  int pw;

    if ( u > 0 && progress_val >= INT_MAX / u && total_steps >= u )
  		pw = (u * (progress_val / u)) / (total_steps / u);
	  else
	  	pw = bar.width() * progress_val / total_steps;

    p->setPen( colorGroup().highlightedText() );
    p->setClipRect( bar.x(), bar.y(), pw, bar.height() );
    p->fillRect( bar, colorGroup().brush( QColorGroup::Highlight ) );
    p->drawText( bar, AlignCenter, progress_str );

    p->setClipRect( bar.x()+pw, bar.y(), bar.width()-pw, bar.height() );
	}

	if ( progress_val != total_steps )
    p->fillRect( bar, colorGroup().brush( QColorGroup::Background ) );

#if QT_VERSION <= 230
	p->setPen( style()==MotifStyle? colorGroup().foreground() : colorGroup().text() );
#else
  p->setPen( style().isA("QMotifStyle")? colorGroup().foreground() : colorGroup().text());
#endif
	p->drawText( bar, AlignCenter, progress_str );
}

bool FCProgressBar::setIndicator ( QString & indicator, int progress, int totalSteps )
{
	if (QProgressBar::setIndicator(indicator, progress, totalSteps) == false)
	{
		if (progress != -1)
		{
			// recalc remaining time every iTimeStep steps
			if (progress % d->iTimeStep == 0 && progress > 0)
			{
				// difference in ms
				int diff = d->measureTime.restart();
				d->iTotalTime += diff;
				// remaining time in s
				diff *= (totalSteps - progress) / d->iTimeStep;
				diff /= 1000;

#ifdef FC_DEBUG
				GetConsole().Log("Elapsed time: %ds, (%d of %d)\n", diff, progress, totalSteps);
#endif
//#				ifdef FC_DEBUG
//				printf("Elapsed time: %ds, (%d of %d)\n", diff, progress, totalSteps);
//#				endif

//				if (bSeveralInstances == false)
				{
					// get time format
					int second = diff %  60; diff /= 60;
					int minute = diff %  60; diff /= 60;
					int hour   = diff %  60;
					QString h,m,s;
					if (hour < 10)   
						h = tr("0%1").arg(hour);
					else
						h = tr("%1").arg(hour);
					if (minute < 10) 
						m = tr("0%1").arg(minute);
					else
						m = tr("%1").arg(minute);
					if (second < 10) 
						s = tr("0%1").arg(second);
					else
						s = tr("%1").arg(second);

					// nice formating for output
					if(hour == 0)
						if(minute == 0)
							d->remainingTime = tr("(%1s)").arg(s);
						else
							d->remainingTime = tr("(%1' %2s)").arg(m).arg(s);
					else
						d->remainingTime = tr("(%1h %2min)").arg(h).arg(m);
				}
			}

			char szBuf[200];
			sprintf(szBuf, "%d %% %s", (100 * progress) / totalSteps, d->remainingTime.latin1());
			indicator = szBuf;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////

FCListView::FCListView ( QWidget * parent, const char * name, WFlags f )
{
}

FCListView::FCListView ( QWidget * parent, const char * name )
{
}

FCListView::~FCListView ()
{
}

QListViewItem* FCListView::lastItem () const
{
  QListViewItem* item = firstChild();
  if ( item ) 
  {
  	while ( item->nextSibling() || item->firstChild() ) 
    {
	    if ( item->nextSibling() )
    		item = item->nextSibling();
	    else
		    item = item->firstChild();
  	}
  }

  return item;
}

QListViewItem* FCListView::lastItem (QListView* listview)
{
  QListViewItem* item = listview->firstChild();
  if ( item ) 
  {
  	while ( item->nextSibling() || item->firstChild() ) 
    {
	    if ( item->nextSibling() )
    		item = item->nextSibling();
	    else
		    item = item->firstChild();
  	}
  }

  return item;
}

///////////////////////////////////////////////////////////////////////////////////

class FCCmdViewItemPrivate
{
  public:
    QString sAction;
    QString description;
};

FCCmdViewItem::FCCmdViewItem ( QIconView * parent, QString Action, QAction* pAct)
: QIconViewItem(parent, pAct->menuText(), pAct->iconSet().pixmap())
{
  d = new FCCmdViewItemPrivate;
  d->sAction = Action;
  d->description = pAct->toolTip();
}

FCCmdViewItem::~FCCmdViewItem ()
{
  delete d;
}

QString FCCmdViewItem::text() const
{
  return d->description;
}

QString FCCmdViewItem::GetAction()
{
  return d->sAction;
}

///////////////////////////////////////////////////////////////////////////////////

FCCmdView::FCCmdView ( QWidget * parent, const char * name, WFlags f )
: QIconView(parent, name, f)
{
  // settings for the view showing the icons
  setResizeMode(Adjust);
  setItemsMovable(false);
  setWordWrapIconText(false);
  setGridX(50);
  setGridY(50);

  setSelectionMode(Extended);

  // clicking on a icon a signal with its description will be emitted
  connect(this, SIGNAL ( currentChanged ( QIconViewItem * ) ), this, SLOT ( slotSelectionChanged(QIconViewItem * ) ) );
}

FCCmdView::~FCCmdView ()
{
}

void FCCmdView::slotSelectionChanged(QIconViewItem * item)
{
  emit emitSelectionChanged(item->text());
}

QDragObject * FCCmdView::dragObject ()
{
  FCActionDrag::actions.clear();
  if ( !currentItem() )
      return 0;

  bool bFirst = true;
  FCActionDrag *ad=NULL;
  QPoint orig = viewportToContents( viewport()->mapFromGlobal( QCursor::pos() ) );
  for ( QIconViewItem *item = firstItem(); item; item = item->nextItem() ) 
  {
    if ( item->isSelected() ) 
    {
      if (typeid(*item) == typeid(FCCmdViewItem))
      {
        ad = new FCActionDrag( ((FCCmdViewItem*)item)->GetAction(), this );
        if (bFirst)
        {
          bFirst = false;
          ad->setPixmap( *currentItem()->pixmap(), QPoint( currentItem()->pixmapRect().width() / 2, 
                                                           currentItem()->pixmapRect().height() / 2 ) );
        }
      }
    }
  }

  return ad;
}

/* 
 *  Constructs a FCDlgCreateToolOrCmdBar which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FCDlgCreateToolOrCmdBar::FCDlgCreateToolOrCmdBar( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "FCDlgCreateToolOrCmdBar" );
    resize( 242, 179 ); 
    setProperty( "caption", tr( "Create Toolbar and/or Command bar" ) );
    setProperty( "sizeGripEnabled", QVariant( TRUE, 0 ) );
    FCDlgCreateToolOrCmdBarLayout = new QGridLayout( this ); 
    FCDlgCreateToolOrCmdBarLayout->setSpacing( 6 );
    FCDlgCreateToolOrCmdBarLayout->setMargin( 11 );

    GroupBox1 = new QGroupBox( this, "GroupBox1" );
    GroupBox1->setProperty( "title", tr( "Toolbar/Command bar" ) );
    GroupBox1->setColumnLayout(0, Qt::Vertical );
    GroupBox1->layout()->setSpacing( 0 );
    GroupBox1->layout()->setMargin( 0 );
    GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
    GroupBox1Layout->setAlignment( Qt::AlignTop );
    GroupBox1Layout->setSpacing( 6 );
    GroupBox1Layout->setMargin( 11 );

    TextLabel = new QLabel( GroupBox1, "TextLabel" );
    TextLabel->setProperty( "text", tr( "Name:" ) );

    GroupBox1Layout->addWidget( TextLabel, 0, 0 );

    LineEditName = new QLineEdit( GroupBox1, "LineEditName" );

    GroupBox1Layout->addWidget( LineEditName, 1, 0 );

    CheckCreateCmdBar = new QCheckBox( GroupBox1, "CheckCreateCmdBar" );
    CheckCreateCmdBar->setProperty( "text", tr( "Create command bar" ) );
    CheckCreateCmdBar->setProperty( "checked", QVariant( TRUE, 0 ) );

    GroupBox1Layout->addWidget( CheckCreateCmdBar, 3, 0 );

    CheckCreateToolBar = new QCheckBox( GroupBox1, "CheckCreateToolBar" );
    CheckCreateToolBar->setProperty( "text", tr( "Create toolbar" ) );
    CheckCreateToolBar->setProperty( "checked", QVariant( TRUE, 0 ) );

    GroupBox1Layout->addWidget( CheckCreateToolBar, 2, 0 );

    FCDlgCreateToolOrCmdBarLayout->addWidget( GroupBox1, 0, 0 );

    Layout2 = new QHBoxLayout; 
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setProperty( "text", tr( "&OK" ) );
    buttonOk->setProperty( "autoDefault", QVariant( TRUE, 0 ) );
    buttonOk->setProperty( "default", QVariant( TRUE, 0 ) );
    Layout2->addWidget( buttonOk );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout2->addItem( spacer );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setProperty( "text", tr( "&Cancel" ) );
    buttonCancel->setProperty( "autoDefault", QVariant( TRUE, 0 ) );
    Layout2->addWidget( buttonCancel );

    FCDlgCreateToolOrCmdBarLayout->addLayout( Layout2, 2, 0 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    FCDlgCreateToolOrCmdBarLayout->addItem( spacer_2, 1, 0 );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

    // tab order
    setTabOrder( LineEditName, CheckCreateToolBar );
    setTabOrder( CheckCreateToolBar, CheckCreateCmdBar );
    setTabOrder( CheckCreateCmdBar, buttonOk );
    setTabOrder( buttonOk, buttonCancel );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
FCDlgCreateToolOrCmdBar::~FCDlgCreateToolOrCmdBar()
{
    // no need to delete child widgets, Qt does it all for us
}

void FCDlgCreateToolOrCmdBar::accept () 
{
  QString txt = LineEditName->text();
  if (CheckCreateToolBar->isChecked())
    ApplicationWindow::Instance->GetCustomWidgetManager()->getToolBar(txt.latin1());
  if (CheckCreateCmdBar->isChecked())
    ApplicationWindow::Instance->GetCustomWidgetManager()->getCmdBar(txt.latin1());
  QDialog::accept();
}

///////////////////////////////////////////////////////////////////////////////////

FCAccelLineEdit::FCAccelLineEdit ( QWidget * parent, const char * name )
: QLineEdit(parent, name)
{
}

void FCAccelLineEdit::keyPressEvent ( QKeyEvent * e)
{
  QString txt;
  clear();

  if (e->ascii() == 0)
    return; // only meta key pressed

  switch(e->state())
	{
		case ControlButton:
      txt += QAccel::keyToString(CTRL+e->key());
      setText(txt);
			break;
		case ControlButton+AltButton:
      txt += QAccel::keyToString(CTRL+ALT+e->key());
      setText(txt);
			break;
		case ControlButton+ShiftButton:
      txt += QAccel::keyToString(CTRL+SHIFT+e->key());
      setText(txt);
			break;
		case ControlButton+AltButton+ShiftButton:
      txt += QAccel::keyToString(CTRL+ALT+SHIFT+e->key());
      setText(txt);
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////////

/* 
 *  Constructs a FCCheckListDlg which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FCCheckListDlg::FCCheckListDlg( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "FCCheckListDlg" );
    resize( 402, 270 ); 
    setProperty( "caption", tr( "MyDialog" ) );
    setProperty( "sizeGripEnabled", QVariant( TRUE, 0 ) );
    FCCheckListDlgLayout = new QGridLayout( this ); 
    FCCheckListDlgLayout->setSpacing( 6 );
    FCCheckListDlgLayout->setMargin( 11 );

    Layout2 = new QHBoxLayout; 
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setProperty( "text", tr( "&OK" ) );
    buttonOk->setProperty( "autoDefault", QVariant( TRUE, 0 ) );
    buttonOk->setProperty( "default", QVariant( TRUE, 0 ) );
    Layout2->addWidget( buttonOk );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout2->addItem( spacer );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setProperty( "text", tr( "&Cancel" ) );
    buttonCancel->setProperty( "autoDefault", QVariant( TRUE, 0 ) );
    Layout2->addWidget( buttonCancel );

    FCCheckListDlgLayout->addLayout( Layout2, 1, 0 );

    GroupBox1 = new QGroupBox( this, "GroupBox1" );
    GroupBox1->setColumnLayout(0, Qt::Vertical );
    GroupBox1->layout()->setSpacing( 0 );
    GroupBox1->layout()->setMargin( 0 );
    GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
    GroupBox1Layout->setAlignment( Qt::AlignTop );
    GroupBox1Layout->setSpacing( 6 );
    GroupBox1Layout->setMargin( 11 );

    ListView = new QListView( GroupBox1, "ListView" );
    ListView->addColumn( "Items" );
    ListView->setRootIsDecorated( TRUE );

    GroupBox1Layout->addWidget( ListView, 0, 0 );

    FCCheckListDlgLayout->addWidget( GroupBox1, 0, 0 );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
FCCheckListDlg::~FCCheckListDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

void FCCheckListDlg::setItems(const std::vector<std::string>& items)
{
  this->items = items;
}

std::vector<int> FCCheckListDlg::getCheckedItems()
{
  return checked;
}

void FCCheckListDlg::show ()
{
  QListViewItem *item = 0;
  for (std::vector<std::string>::iterator it = items.begin(); it!=items.end(); ++it)
  {
    (void)new QCheckListItem( ListView, it->c_str(), QCheckListItem::CheckBox );
  }

  QDialog::show();
}

void FCCheckListDlg::hide ()
{
  int pos = 0;
  QListViewItemIterator it = ListView->firstChild();

  for ( ; it.current(); it++, pos++ ) 
  {
    if ( ((QCheckListItem*)it.current())->isOn() ) 
    {
      checked.push_back(pos);
	  }
  }

  QDialog::hide();
}

///////////////////////////////////////////////////////////////////////////////////

FCColorButton::FCColorButton(QWidget* parent, const char* name)
  : QButton( parent, name )
{
  setMinimumSize( minimumSizeHint() );
  connect( this, SIGNAL(clicked()), SLOT(onChooseColor()));
}

FCColorButton::FCColorButton( const QBrush& b, QWidget* parent, const char* name, WFlags f )
  : QButton( parent, name, f )
{
  col = b.color();
  setMinimumSize( minimumSizeHint() );
}

FCColorButton::~FCColorButton()
{
}

void FCColorButton::setColor( const QColor& c )
{
  col = c;
  update();
}

QColor FCColorButton::color() const
{
  return col;
}

QSize FCColorButton::sizeHint() const
{
  return QSize( 50, 25 );
}

QSize FCColorButton::minimumSizeHint() const
{
  return QSize( 50, 25 );
}

void FCColorButton::drawButton( QPainter *paint )
{
#if QT_VERSION <= 230
  style().drawBevelButton( paint, 0, 0, width(), height(), colorGroup(), isDown() );
  drawButtonLabel( paint );
  if ( hasFocus() ) 
  {
   	style().drawFocusRect( paint, style().bevelButtonRect( 0, 0, width(), height()),
		colorGroup(), &colorGroup().button() );
  }
#else
  style().drawPrimitive( QStyle::PE_ButtonBevel, paint, QRect(0, 0, width(), height()), colorGroup());
  drawButtonLabel( paint );
  if ( hasFocus() ) 
  {
    style().drawPrimitive( QStyle::PE_FocusRect, paint, style().subRect( QStyle::SR_PushButtonContents, this),
		colorGroup() );
  }
#endif
}

void FCColorButton::drawButtonLabel( QPainter *paint )
{
  QColor pen = isEnabled() ? hasFocus() ? palette().active().buttonText() : palette().inactive().buttonText()
		                                    : palette().disabled().buttonText();
  paint->setPen( pen );
	paint->setBrush( QBrush( col ) );

  paint->drawRect( width()/4, height()/4, width()/2, height()/2 );
}

void FCColorButton::onChooseColor()
{
  QColor c = QColorDialog::getColor( palette().active().background(), this );
  if ( c.isValid() ) 
  {
    setColor( c );
    emit changed();
  }
}

///////////////////////////////////////////////////////////////////////////////////

class FCSpinBoxPrivate
{
  public:
    FCSpinBoxPrivate();
    bool pressed;
    int nY, nStep;
};

FCSpinBoxPrivate::FCSpinBoxPrivate()
{
  pressed = false;
}

// -----------------------------------------------------------------------------------

FCSpinBox::FCSpinBox ( QWidget* parent, const char* name )
 : QSpinBox (parent, name)
{
  setMouseTracking(true);
  d = new FCSpinBoxPrivate;
}

FCSpinBox::FCSpinBox ( int minValue, int maxValue, int step, QWidget* parent, const char* name )
 : QSpinBox(minValue, maxValue, step, parent, name)
{
  setMouseTracking(true);
  d = new FCSpinBoxPrivate;
}

FCSpinBox::~FCSpinBox()
{
  delete d;
  d = 0L;
}

void FCSpinBox::mouseMoveEvent ( QMouseEvent* e )
{
  if (QWidget::mouseGrabber() == NULL && !rect().contains(e->pos()) && d->pressed )
    grabMouse(QCursor(IbeamCursor));

  if (QWidget::mouseGrabber() == this)
  {
    // get "speed" of mouse move
    int mult = d->nY - e->y();

    int nValue = value() + mult * d->nStep;
    if (nValue <= maxValue())
      setValue ( nValue );
    else
      setValue ( maxValue() );

    d->nY = e->y();
  }
  else
    QSpinBox::mouseMoveEvent(e);
}

void FCSpinBox::mousePressEvent   ( QMouseEvent* e )
{
  d->pressed = true;

  int nMax = maxValue();
  int nMin = minValue();

  if (nMax == INT_MAX || nMin == -INT_MAX)
  {
    d->nStep = 100;
  }
  else
  {
    int nRange = nMax - nMin;
    int nHeight = QApplication::desktop()->height();
    if (nRange > nHeight)
      d->nStep = int(nRange / nHeight);
    else
      d->nStep = 1;
  }

  d->nY = e->y();
}

void FCSpinBox::mouseReleaseEvent ( QMouseEvent* e )
{
  if (QWidget::mouseGrabber() == this)
    releaseMouse();
  d->pressed = false;
}

void FCSpinBox::focusOutEvent ( QFocusEvent* e )
{
  if (QWidget::mouseGrabber() == this)
    releaseMouse();
}

void FCSpinBox::wheelEvent ( QWheelEvent* e )
{
  if (isEnabled()) 
    QSpinBox::wheelEvent(e);
}

bool FCSpinBox::eventFilter ( QObject* o, QEvent* e )
{
  if ( o != editor() )
  	return false;

  // get the editor's mouse events
  switch (e->type())
  {
    // redirect the events to spin box (itself)
    case QEvent::MouseButtonPress:
      mousePressEvent ((QMouseEvent*)e);
      break;

    case QEvent::MouseButtonRelease:
      mouseReleaseEvent ((QMouseEvent*)e);
      break;

    case QEvent::MouseMove:
      mouseMoveEvent ((QMouseEvent*)e);
      break;

    case QEvent::FocusOut:
      focusOutEvent ((QFocusEvent*)e);
      break;
  }

  return QSpinBox::eventFilter(o, e);
}

// -------------------------------------------------------------

class FCFloatSpinBoxPrivate
{
  public:
    FCFloatSpinBoxPrivate(){}
    ~FCFloatSpinBoxPrivate()
    {
      delete m_pValidator; 
    }

    QDoubleValidator* m_pValidator;
    int               m_iDecimals;
    double            m_fDivisor;
    double            m_fEpsilon;
};

FCFloatSpinBox::FCFloatSpinBox ( QWidget * parent, const char * name )
: FCSpinBox(parent, name)
{
  d = new FCFloatSpinBoxPrivate;

  d->m_pValidator = new QDoubleValidator((double)minValue(), (double)maxValue(), 2, this, name );
  setValidator(d->m_pValidator);
  
  setDecimals(0);
}

FCFloatSpinBox::FCFloatSpinBox ( int minValue, int maxValue, int step, QWidget* parent, const char* name )
: FCSpinBox(minValue, maxValue, step, parent, name)
{
  d = new FCFloatSpinBoxPrivate;

  d->m_pValidator = new QDoubleValidator((double)minValue, (double)maxValue, 2, this, name );
  setValidator(d->m_pValidator);

  setDecimals(0);
}

FCFloatSpinBox::~FCFloatSpinBox() 
{
  delete d;
}

int FCFloatSpinBox::decimals() const
{
  return d->m_iDecimals;
}

void FCFloatSpinBox::setDecimals(int i)
{
  d->m_iDecimals = i;
  d->m_fDivisor = int(pow(10.0, double(d->m_iDecimals)));
  d->m_fEpsilon = 1.0 / pow(10.0, double(d->m_iDecimals+1));

  if (maxValue() <  INT_MAX)
    FCSpinBox::setMaxValue(int(maxValue() * d->m_fDivisor));
  if (minValue() > -INT_MAX)
    FCSpinBox::setMinValue(int(minValue() * d->m_fDivisor));
}

double FCFloatSpinBox::valueFloat() const
{ 
  return FCSpinBox::value() / double(d->m_fDivisor); 
}

void FCFloatSpinBox::setMinValueFloat(double value)
{ 
  FCSpinBox::setMinValue(int(d->m_fDivisor * value)); 
}

void FCFloatSpinBox::setMaxValueFloat(double value)
{ 
  FCSpinBox::setMaxValue(int(d->m_fDivisor * value)); 
}

double FCFloatSpinBox::minValueFloat () const
{
  return FCSpinBox::minValue() / double(d->m_fDivisor); 
}

double FCFloatSpinBox::maxValueFloat () const
{
  return FCSpinBox::maxValue() / double(d->m_fDivisor); 
}

QString FCFloatSpinBox::mapValueToText(int value)
{ 
  return QString::number(double(value) / d->m_fDivisor, 'f', d->m_iDecimals); 
}

int FCFloatSpinBox::mapTextToValue(bool*)
{ 
  double fEps = value() > 0.0 ? d->m_fEpsilon : - d->m_fEpsilon;
  return int(text().toDouble() * d->m_fDivisor + fEps); 
}

void FCFloatSpinBox::valueChange()
{
  FCSpinBox::valueChange();
  emit valueFloatChanged( FCSpinBox::value() / double(d->m_fDivisor) );
}

void FCFloatSpinBox::setValueFloat(double value)
{ 
  double fEps = value > 0.0 ? d->m_fEpsilon : - d->m_fEpsilon;
  FCSpinBox::setValue(int(d->m_fDivisor * value + fEps)); 
}

void FCFloatSpinBox::stepChange () 
{
  FCSpinBox::stepChange();
}

// --------------------------------------------------------

FCAnimation* FCAnimation::_pcSingleton = 0L;

void FCAnimation::Destruct(void)
{
	// not initialized or double destruct!
  assert(_pcSingleton);
	delete _pcSingleton;
}

FCAnimation* FCAnimation::Instance(void)
{
	// not initialized?
	if(!_pcSingleton)
	{
		_pcSingleton = new FCAnimation(NULL, "Animation");
	}

  return _pcSingleton;
}

FCAnimation::FCAnimation(QWidget * parent, const char * name, WFlags f)
  : QLabel(parent, name, f)
{
  setFixedWidth(50);
  hide();
}

FCAnimation::~FCAnimation()
{
}

void FCAnimation::startAnimation()
{
#ifndef QGIF_H
  setText("Download");
#else
  setMovie(QMovie("trolltech.gif"));
#endif
#if QT_VERSION > 300
  setMovie(QMovie("trolltech.gif"));
#endif
}

void FCAnimation::stopAnimation()
{
  setText("");
}

#include "moc_Widgets.cpp"
