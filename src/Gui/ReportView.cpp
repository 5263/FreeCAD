/***************************************************************************
                          ReportView.cpp  -  description
                             -------------------
    begin                : 2003/08/16 10:47:44
    copyright            : (C) 2003 by Werner Mayer
    email                : werner.wm.mayer@gmx.de
 ***************************************************************************/

/** \file ReportView.cpp
 *  \brief The output window of FreeCAD
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
#	include <qscrollview.h>
#	include <qsyntaxhighlighter.h>
#	include <qtabwidget.h>
#	include <qtextedit.h>
#	include <qtimer.h>
#endif

#include "ReportView.h"
#include "Widgets.h"
#include "PythonEditor.h"

using namespace Gui::DockWnd;

/*
 *  Constructs a ReportView which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
ReportView::ReportView( QWidget* parent,  const char* name, WFlags fl )
    : FCDockWindow( parent, name, fl )
{
  if ( !name )
  	setName( "ReportOutput" );

  resize( 529, 162 );
  QGridLayout* tabLayout = new QGridLayout( this );
  tabLayout->setSpacing( 0 );
  tabLayout->setMargin( 0 );

  tab = new QTabWidget( this, "TabWidget" );
  tab->setProperty( "tabPosition", (int)QTabWidget::Bottom );
  tab->setProperty( "tabShape", (int)QTabWidget::Triangular );
  tabLayout->addWidget( tab, 0, 0 );


  mle = new ReportOutput( tab, "LogOutput" );
  tab->insertTab( mle, tr( "Output" ) );

	pyc = new PythonConsole(tab, "PythonConsole");
	tab->insertTab(pyc, "Python");
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ReportView::~ReportView()
{
    // no need to delete child widgets, Qt does it all for us
}

// ----------------------------------------------------------

ReportHighlighter::ReportHighlighter(QTextEdit* edit)
: QSyntaxHighlighter(edit), type(Message), lastPos(0), lastPar(0)
{
}

ReportHighlighter::~ReportHighlighter()
{
}

int ReportHighlighter::highlightParagraph ( const QString & text, int endStateOfLastPara )
{
	int curPar = currentParagraph();
	if (curPar > lastPar)
		lastPos = 0;

	if (type == Message)
	{
		setFormat(lastPos, text.length()-lastPos, Qt::black);
	}
	else if (type == Warning)
	{
		setFormat(lastPos, text.length()-lastPos, QColor(255, 170, 0));
	}
	else if (type == Error)
	{
		setFormat(lastPos, text.length()-lastPos, Qt::red);
	}
	else if (type == LogText)
	{
		setFormat(lastPos, text.length()-lastPos, Qt::black);
	}

	lastPos = text.length()-1;
	lastPar = curPar;
	return 0;
}

void ReportHighlighter::setParagraphType(ReportHighlighter::Paragraph t)
{
	type = t;
}

// ----------------------------------------------------------

ReportOutput::ReportOutput(QWidget* parent, const char* name)
 : QTextEdit(parent, name)
{
	reportHl = new ReportHighlighter(this);

	restoreFont();
	setReadOnly(true);
  clear();
  setHScrollBarMode(QScrollView::AlwaysOff);

  insert("FreeCAD output.\n");
  Base::Console().AttacheObserver(this);
}

ReportOutput::~ReportOutput()
{
	Base::Console().DetacheObserver(this);
	delete reportHl;
}

void ReportOutput::restoreFont()
{
  QFont _font(  font() );
  _font.setFamily( "Courier" );
#ifdef FC_OS_LINUX
  _font.setPointSize( 15 );
#else
  _font.setPointSize( 10 );
#endif
//  _font.setBold( TRUE );
  setFont( _font ); 
}

void ReportOutput::Warning(const char * s)
{
	reportHl->setParagraphType(ReportHighlighter::Warning);
  append(s);
}

void ReportOutput::Message(const char * s)
{
	reportHl->setParagraphType(ReportHighlighter::Message);
  append(s);
}

void ReportOutput::Error  (const char * s)
{
	reportHl->setParagraphType(ReportHighlighter::Error);
  append(s);
}

void ReportOutput::Log (const char * s)
{
  // ignore this
}

bool ReportOutput::event( QEvent* ev )
{
  bool ret = QWidget::event( ev ); 
  if ( ev->type() == QEvent::ApplicationFontChange ) 
  {
		restoreFont();
  }

  return ret;
}

QPopupMenu * ReportOutput::createPopupMenu ( const QPoint & pos )
{
	QPopupMenu* menu = QTextEdit::createPopupMenu(pos);
	menu->insertItem(tr("Clear"), this, SLOT(clear()));
	menu->insertSeparator();
	menu->insertItem(tr("Save As..."), this, SLOT(onSaveAs()));
	return menu;
}

void ReportOutput::onSaveAs()
{
  QString fn = FileDialog::getSaveFileName(QString::null,"Plain Text Files (*.txt *.log)", 
																						 this, QObject::tr("Save Report Output"));
  if (!fn.isEmpty())
  {
    int dot = fn.find('.');
    if (dot != -1)
    {
			QFile f(fn);
			if (f.open(IO_WriteOnly))
			{
				QTextStream t (&f);
				t << text();
				f.close();
			}
		}
	}
}

#include "moc_ReportView.cpp"
