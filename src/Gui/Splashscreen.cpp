/***************************************************************************
                          Splashscreen.cpp  -  description
                             -------------------
    begin                : 2002/08/19 21:11:52
    copyright            : (C) 2002 by Werner Mayer
    email                : werner.wm.mayer@gmx.de
 ***************************************************************************/

/** \file Splashscreen.cpp
 *  \brief The splash screen module
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

#include "../Version.h"
#ifndef _PreComp_
#	include <qapplication.h>
#	include <qmessagebox.h>
#	include <qiconview.h>
#	include <qfiledialog.h>
#	include <qimage.h>
#	include <qlabel.h>
#	include <qlayout.h>
#	include <qpixmap.h>
#	include <qprogressbar.h>
#	include <qpushbutton.h>
#	include <qsplashscreen.h>
#	include <qstylefactory.h>
#	include <qtextbrowser.h>
#	include <qtextview.h>
#	include <qthread.h>
#	include <qtooltip.h>
#	include <qvariant.h>
#	include <qwhatsthis.h>
#endif

#include "Splashscreen.h"
#include "Icons/developers.h"
#include "../Base/Console.h"


FCSplashWidget::FCSplashWidget( QWidget* parent,  const char* name, WFlags f)
  : QLabel( parent, "splash", f)
{
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));
  bRun = true;

  // set the developers of FC
  _aclDevelopers["Juergen_Riegel"] = std::make_pair<std::string, QPixmap>("J�rgen Riegel", QPixmap(Juergen_Riegel));
  _aclDevelopers["Werner_Mayer"]   = std::make_pair<std::string, QPixmap>("Werner Mayer" , QPixmap(Juergen_Riegel));

  // set the text for all subclasses
  SplasherText = QString(
    "<!DOCTYPE HTML PUBLIC ""-//W3C//DTD HTML 4.0 Transitional//EN"">"
    "<html>"
    "<body bgcolor=""#ffffff"">"
    "<p>"
    "<table cellpadding=2 cellspacing=1 border=0  width=100% bgcolor=#E5E5E5 >"
    "<tr>"
    "<th bgcolor=#E5E5E5 width=33%>"
    "<ul>"
    "<a href=""FreeCAD"">FreeCAD (c) 2001 J�rgen Riegel</a>\n"
    "</ul>"
    "</th>"
    "</tr>"
    "<tr>"
    "<td>"
    "<p>Version&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>%1.%2 </b></p>"
    "<p>Build number&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;                             <b>%3</b></p>"
    "<p>Build date&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <b>%4</b></p>"
    "</td>"
    "</tr>"
    "<tr>"
    "<td>"
    "Main developers of FreeCAD are:"
    "<ul>").arg(FCVersionMajor).arg(FCVersionMinor).arg(FCVersionBuild).arg(FCVersionDisDa);
  for (std::map<std::string, std::pair<std::string, QPixmap> >::iterator it = _aclDevelopers.begin(); it != _aclDevelopers.end(); ++it)
    SplasherText += QString(	"<li> <a href=""%1"">%2</a>\n").
                    arg(QString(it->first.c_str())). // first argument
                    arg(QString(it->second.first.c_str()));// second argument
  SplasherText += 
    "</ul>"
    "</td>"
    "</tr>"
    "</table>"
    "</body></html>";
}

FCSplashWidget::~FCSplashWidget()
{
}

void FCSplashWidget::aboutToQuit()
{
#ifdef FC_DEBUG
  //printf("Terminating thread %s...\n", this->getName().latin1());
#endif
  // terminate savely this thread
  if (running())
  {
    bRun = false;
    wait(1000);
  }
}

void FCSplashWidget::hideEvent ( QHideEvent * e )
{
#ifdef FC_DEBUG
  //printf("%s goes sleeping...\n", this->getName().latin1());
#endif
  QWidget::hideEvent ( e );
}

/////////////////////////////////////////////////////////////////////////////////

class FCSplashObserver : public FCConsoleObserver
{
  public:
    FCSplashObserver(QSplashScreen* splasher=0, const char* name=0)
			: splash(splasher)
		{
		  GetConsole().AttacheObserver(this);
		}
    
		~FCSplashObserver()
		{
		  GetConsole().DetacheObserver(this);
		}

    void Warning(const char * s)
		{
			Log(s);
		}
	  void Message(const char * s)
		{
			Log(s);
		}
    void Error  (const char * s)
		{
			Log(s);
		}
	  void Log (const char * s)
		{
			QString msg(s);
			// remove leading blanks and carriage returns
			while (msg[0] == '\n' || msg[0] == ' ')
			{
				msg = msg.mid(1);
			}
			msg = QString("\n %1").arg(msg);
			splash->message( msg, Qt::AlignTop|Qt::AlignLeft, Qt::black );
			qApp->processEvents();
			QWaitCondition().wait(50);
		}

		private:
			QSplashScreen* splash;
};

FCSplashScreen::FCSplashScreen(  const QPixmap & pixmap , WFlags f )
  : QSplashScreen( pixmap, f), progBar(0L)
{
	// write the messages to splasher
	messages = new FCSplashObserver(this);

	// append also a progressbar for visual feedback
  progBar = new QProgressBar( this, "SplasherProgress" );
  progBar->setProperty( "progress", 0 );
  progBar->setStyle(QStyleFactory::create("motif"));
	progBar->setFixedSize(width()-6, 15);

	// make the splasher a bit higher
	resize(width(), height()+progBar->height());
	progBar->move(3, height()-(progBar->height()));
}

FCSplashScreen::~FCSplashScreen()
{
	delete messages;
	delete progBar;
}

void FCSplashScreen::drawContents ( QPainter * painter )
{
	if (progBar)
	  progBar->setProgress(progBar->progress() + 6);
	QSplashScreen::drawContents(painter);
}

/////////////////////////////////////////////////////////////////////////////////

FCSplashBrowser::FCSplashBrowser(QWidget * parent, const char * name)
: QTextBrowser(parent, name)
{
}

void FCSplashBrowser::setSource ( const QString & name )
{
  emit linkClicked(name);
}

/////////////////////////////////////////////////////////////////////////////////

FCSplashAbout* FCSplashAbout::_pclSingleton = NULL;
void FCSplashAbout::Destruct(void)
{
  // not initialized or double destruct!
  assert(_pclSingleton);
	delete _pclSingleton;
}

FCSplashAbout* FCSplashAbout::Instance(void)
{
  // not initialized?
  if(!_pclSingleton)
  {
    _pclSingleton = new FCSplashAbout(qApp->mainWidget());
    _pclSingleton->start();
  }

  _pclSingleton->show();
  if (!_pclSingleton->running())
  {
    _pclSingleton->bRun = true;
    _pclSingleton->start();
  }

  return _pclSingleton;
}

FCSplashAbout::FCSplashAbout( QWidget* parent,  const char* name)
  : FCSplashWidget( parent, name, QLabel::WDestructiveClose | 
                                  QLabel::WStyle_Customize  | 
                                  QLabel::WStyle_NoBorder   | 
                                  QLabel::WType_Modal       )
{
  int iHeight = 389; int iWidth = 496;
  QPixmap image0( ( const char** ) image0_data );
  if ( !name )
  	setName( "SplasherDialog" );
  setProperty( "enabled", QVariant( TRUE, 0 ) );
  resize( iHeight, iWidth ); 
  setProperty( "caption",  "FreeCAD Startup" );
  SplasherDialogLayout = new QVBoxLayout( this ); 
  SplasherDialogLayout->setSpacing( 6 );
  SplasherDialogLayout->setMargin( 11 );

  PixmapLabel = new QLabel( this, "PixmapLabel" );
  PixmapLabel->setProperty( "sizePolicy", QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, PixmapLabel->sizePolicy().hasHeightForWidth() ) );
  PixmapLabel->setProperty( "pixmap", image0 );
  PixmapLabel->setProperty( "scaledContents", QVariant( TRUE, 0 ) );
  SplasherDialogLayout->addWidget( PixmapLabel );

  SplasherTextView = new FCSplashBrowser( this, "SplasherText" );
  SplasherTextView->setText (SplasherText);
  SplasherTextView->setHScrollBarMode(QScrollView::AlwaysOff);
  SplasherTextView->setVScrollBarMode(QScrollView::AlwaysOff);
  connect(SplasherTextView, SIGNAL(linkClicked(const QString&)), this, SLOT(linkClicked(const QString&)));
  SplasherDialogLayout->addWidget( SplasherTextView );

  RemainingTime = new QLabel(this, "Time");
  RemainingTime->setText("");
  SplasherDialogLayout->addWidget(RemainingTime);

  ButtonOK = new QPushButton(this, "OK");
  ButtonOK->setFixedWidth(60);
  ButtonOK->setText("Ok");
  ButtonOK->setAutoDefault(true);
  connect(ButtonOK, SIGNAL(clicked()), this, SLOT(clicked()));
  SplasherDialogLayout->addWidget(ButtonOK);

  setFrameStyle( QFrame::WinPanel | QFrame::Raised );
	setPixmap( QPixmap() );
	adjustSize();
	QRect r = QApplication::desktop()->geometry();
  QRect s = rect();
  s = QRect(0, 0, iHeight, iWidth);
	move( r.center() - s.center() );
	show();
	repaint( FALSE );
}

FCSplashAbout::~FCSplashAbout()
{
  bRun = false;
}

void FCSplashAbout::run()
{
  int sleep = 300;
  int max = 100;
  int cnt = 0;
  while (bRun && cnt++ < max)
  {
    msleep(sleep);
    SplasherTextView->scrollBy(0,1);

    if ((max-cnt)*sleep <= 10000)
    {
      RemainingTime->setText(tr("Closing this dialog in %1 seconds...").arg((max-cnt)*sleep/1000));
    }
  }

  RemainingTime->setText("");
  hide();
}

QString FCSplashAbout::getName()
{
  return "AboutSplasher";
}

void FCSplashAbout::keyPressEvent ( QKeyEvent * e )
{
  bRun = false;
  QWidget::keyPressEvent ( e );
}

void FCSplashAbout::clicked()
{
  bRun = false;
}

void FCSplashAbout::hideEvent ( QHideEvent * e)
{
  QWidget::hideEvent ( e );
  setFCPixmap(QPixmap ( ( const char** ) image0_data ));
}

void FCSplashAbout::linkClicked(const QString& txt)
{
  if (_aclDevelopers.find(txt.latin1()) != _aclDevelopers.end())
  {
    QPixmap pixmap = _aclDevelopers[txt.latin1()].second;
    PixmapLabel->setProperty( "pixmap", pixmap);
  }
  else if (txt == QString("FreeCAD"))
  {
    PixmapLabel->setProperty( "pixmap", QPixmap ((const char**)image0_data));
  }
}

void FCSplashAbout::setFCPixmap(const QPixmap& image0)
{
  PixmapLabel->setProperty( "pixmap", image0 );
}

#include "moc_Splashscreen.cpp"
