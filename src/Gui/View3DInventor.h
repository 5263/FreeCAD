/** \file View3DInventor.h
 *  \brief 3D View Window
 *  \author $Author$
 *  \version $Revision$
 *  \date    $Date$
 *  @see FCView.cpp
 */

/***************************************************************************
 *   (c) J�rgen Riegel (juergen.riegel@web.de) 2003                        *   
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License (LGPL)   *
 *   as published by the Free Software Foundation; either version 2 of     *
 *   the License, or (at your option) any later version.                   *
 *   for detail see the LICENCE text file.                                 *
 *                                                                         *
 *   FreeCAD is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        * 
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with FreeCAD; if not, write to the Free Software        * 
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
 *   USA                                                                   *
 *                                                                         *
 *   Juergen Riegel 2003                                                   *
 ***************************************************************************/

#ifndef __VIEW3DINVENTOR__
#define __VIEW3DINVENTOR__

#include "View.h"

class QMouseEvent;
class FCMouseModel;
class FCGuiDocument;
class FCGuiDocument;
class View3D;
class FCTree;
class QSplitter;
class QWidget;		
class QPushButton;	
class QVBoxLayout;	
class QHBoxLayout;	
class QWidgetStack;
class QTabBar;
class SoQtExaminerViewer;

class	SoSeparator ;
class	SoShapeHints;
class	SoMaterial	;


/** The 3D View Window
 *  It consist out of the 3DView and the tree
 */
class FCView3DInventor: public FCView
{
	Q_OBJECT

public:
	FCView3DInventor( FCGuiDocument* pcDocument, QWidget* parent, const char* name, int wflags=WDestructiveClose );
	~FCView3DInventor();

	/// Mesage handler
	virtual bool OnMsg(const char* pMsg);
	virtual const char *GetName(void);

	virtual void resizeEvent ( QResizeEvent * e);

	virtual void Update(void);

	void UpdatePrefs(void);

	void SetViewerDefaults(void);

	void SetShape(void);


//signals:
	//void message(const QString&, int );
	
public slots:
//	void closeEvent(QCloseEvent* e);        
	void onWindowActivated ();
	void setCursor(const QCursor&);
	void dump();

protected:
	// inventor nodes:
	SoSeparator * pcSepUserSpecial;
	SoSeparator * pcSepRoot;
	SoSeparator * pcSepShapeRoot;
	SoSeparator * pcSepAxis;
	SoShapeHints* pcShapeHint; 
	SoMaterial	* pcShapeMaterial;
private:
	SoQtExaminerViewer * _viewer;
	QWidget*		_pcWidget;
	QVBox*			_pcFrame;    
	QWidgetStack*	_pcWidgetStack;
	QTabBar*		_pcTabBar;

};





#endif  //__VIEW3DINVENTOR__

