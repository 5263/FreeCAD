/***************************************************************************
                          WidgetFactory.cpp  -  description
                             -------------------
    begin                : 2003/01/12 23:25:44
    copyright            : (C) 2002 by Werner Mayer
    email                : werner.wm.mayer@gmx.de
 ***************************************************************************/

/** \file WidgetFactory.cpp
 *  \brief Produce widgets
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

#include "WidgetFactory.h"
#include "PrefWidgets.h"
#include "Widgets.h"
#include "DlgPreferencesImp.h"
#include "DlgSettingsImp.h"
#include "DlgSettings3DViewImp.h"
#include "DlgGeneralImp.h"
#include "DlgEditorImp.h"
#include "DlgSettingsMacroImp.h"
#include "DlgOnlineHelpImp.h"

#include "../Base/Console.h"
#include "../Base/Exception.h"

#if QT_VERSION >= 300
#	include <qwidgetfactory.h>
# ifdef FC_OS_WIN32
#		pragma comment(lib,"qui.lib")
#		pragma warning( disable : 4098 )
#	endif
#endif


FCWidgetFactory* FCWidgetFactory::_pcSingleton = NULL;

FCWidgetFactory& FCWidgetFactory::Instance(void)
{
  if (_pcSingleton == NULL)
    _pcSingleton = new FCWidgetFactory;
  return *_pcSingleton;
}

void FCWidgetFactory::Destruct (void)
{
  if (_pcSingleton != NULL)
    delete _pcSingleton;
}

QWidget* FCWidgetFactory::ProduceWidget (const char* sName, QWidget* parent) const
{
	QWidget* w = (QWidget*)Produce(sName);

  // this widget class is not registered
  if (!w)
  {
#ifdef FC_DEBUG
    GetConsole().Warning("\"%s\" is not registered\n", sName);
#else
    GetConsole().Log("\"%s\" is not registered\n", sName);
#endif
    return NULL;
  }

  try
  {
    dynamic_cast<QWidget*>(w);
  }
  catch (...)
  {
#ifdef FC_DEBUG
    GetConsole().Error("%s does not inherit from \"QWidget\"\n", sName);
#else
    GetConsole().Log("%s does not inherit from \"QWidget\"\n", sName);
#endif
		delete w;
		return NULL;
  }

  // set the parent to the widget 
	if (parent)
	  w->reparent(parent, QPoint(0,0));

  return w;
}


QWidget* FCWidgetFactory::ProducePrefWidget(const char* sName, QWidget* parent, const char* sPref)
{
  QWidget* w = ProduceWidget(sName);
  // this widget class is not registered
  if (!w)
    return NULL; // no valid QWidget object

  // set the parent to the widget 
  w->reparent(parent, QPoint(0,0));

  try
  {
    dynamic_cast<FCWidgetPrefs*>(w)->setPrefName(sPref);
    dynamic_cast<FCWidgetPrefs*>(w)->restorePreferences();
  }
  catch (...)
  {
#ifdef FC_DEBUG
    GetConsole().Error("%s does not inherit from \"FCWidgetPrefs\"\n", w->className());
#endif
		delete w;
		return NULL;
  }

  return w;
}

// ----------------------------------------------------

#if QT_VERSION >= 300
class FCQtWidgetFactory : public QWidgetFactory
{
	public:
		FCQtWidgetFactory() : QWidgetFactory(){}
		~FCQtWidgetFactory(){}

		QWidget* createWidget( const QString & className, QWidget * parent, const char * name ) const
		{
			QString cname = QString("class %1").arg(className);
			return GetWidgetFactory().ProduceWidget(cname.latin1(), parent);
		}
};
#endif

// ----------------------------------------------------

template <class CLASS>
FCWidgetProducer<CLASS>::FCWidgetProducer ()
{
	FCWidgetFactory::Instance().AddProducer(typeid(CLASS).name(), this);
}

// ----------------------------------------------------

template <class CLASS>
FCPrefPageProducer<CLASS>::FCPrefPageProducer (const QString& caption) : mCaption(caption)
{
	FCWidgetFactory::Instance().AddProducer(mCaption.latin1(), this);
	FCDlgPreferencesImp::addPage(caption);
}

// ----------------------------------------------------

FCWidgetFactorySupplier* FCWidgetFactorySupplier::_pcSingleton = 0L;

FCWidgetFactorySupplier::FCWidgetFactorySupplier()
{
  // ADD YOUR PREFERENCFE PAGES HERE
  //
  //
	FCDlgPreferencesImp::addGroup( QObject::tr("FreeCAD") );
  new FCPrefPageProducer<FCDlgGeneral>       ( QObject::tr("General"     ) );
  new FCPrefPageProducer<FCDlgEditorSettings>( QObject::tr("Editor"      ) );
  new FCPrefPageProducer<FCDlgSettingsMacro> ( QObject::tr("Macros"      ) );
  new FCPrefPageProducer<FCOnlineHelp>       ( QObject::tr("Online help" ) );
	FCDlgPreferencesImp::addGroup( QObject::tr("Viewer") );
  new FCPrefPageProducer<FCDlgSettings>      ( QObject::tr("Help Viewer" ) );
  new FCPrefPageProducer<FCDlgSettings3DView>( QObject::tr("3D View"     ) );

	// ADD YOUR PREFERENCE WIDGETS HERE
	//
	//
	new FCWidgetProducer<FCPrefSpinBox>;
	new FCWidgetProducer<FCLineEdit>;
	new FCWidgetProducer<FCComboBox>;
	new FCWidgetProducer<FCListBox>;
	new FCWidgetProducer<FCCheckBox>;
	new FCWidgetProducer<FCRadioButton>;
	new FCWidgetProducer<FCSlider>;
	new FCWidgetProducer<FCCmdView>;
	new FCWidgetProducer<FCAccelLineEdit>;
	new FCWidgetProducer<FCColorButton>;

#if QT_VERSION >= 300
	QWidgetFactory::addWidgetFactory(new FCQtWidgetFactory);
#endif
}

FCWidgetFactorySupplier & FCWidgetFactorySupplier::Instance(void)
{
	// not initialized?
	if(!_pcSingleton)
	{
		_pcSingleton = new FCWidgetFactorySupplier;
	}

  return *_pcSingleton;
}

// ----------------------------------------------------

FCContainerDialog::FCContainerDialog( QWidget* templChild )
    : QDialog( 0L, 0L, true, 0 )

{
	setCaption( templChild->name() );
	setName( templChild->name() );

	setSizeGripEnabled( TRUE );
	MyDialogLayout = new QGridLayout( this, 1, 1, 11, 6, "MyDialogLayout"); 

	buttonOk = new QPushButton( this, "buttonOk" );
	buttonOk->setText( tr( "&OK" ) );
	buttonOk->setAutoDefault( TRUE );
	buttonOk->setDefault( TRUE );

	MyDialogLayout->addWidget( buttonOk, 1, 0 );
	QSpacerItem* spacer = new QSpacerItem( 210, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	MyDialogLayout->addItem( spacer, 1, 1 );

	buttonCancel = new QPushButton( this, "buttonCancel" );
	buttonCancel->setText( tr( "&Cancel" ) );
	buttonCancel->setAutoDefault( TRUE );

	MyDialogLayout->addWidget( buttonCancel, 1, 2 );

	templChild->reparent(this, QPoint());

	MyDialogLayout->addMultiCellWidget( templChild, 0, 0, 0, 2 );
	resize( QSize(307, 197).expandedTo(minimumSizeHint()) );

	// signals and slots connections
	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

FCContainerDialog::~FCContainerDialog()
{
}

// ----------------------------------------------------

FCPythonResource* FCPythonResource::_pcSingleton = NULL;

FCPythonResource& FCPythonResource::Instance(void)
{
  if (_pcSingleton == NULL)
    _pcSingleton = new FCPythonResource;
  return *_pcSingleton;
}

void FCPythonResource::Destruct (void)
{
  if (_pcSingleton != NULL)
    delete _pcSingleton;
}

FCPythonResource::FCPythonResource()
{
	// setting up Python binding 
	(void) Py_InitModule("FreeCADRes", FCPythonResource::Methods);
}

//**************************************************************************
// Python stuff

// FCPythonResource Methods						// Methods structure
PyMethodDef FCPythonResource::Methods[] = {
	{"GetInput",       (PyCFunction) FCPythonResource::sGetInput,        1},

  {NULL, NULL}		/* Sentinel */
};

PYFUNCIMP_S(FCPythonResource,sGetInput)
{
	char *psUIFile;
	if (!PyArg_ParseTuple(args, "s", &psUIFile))     // convert args: Python->C 
		return NULL;                             // NULL triggers exception 

#if QT_VERSION < 300
	GetConsole().Error("Qt version is too old!\n");
	return NULL;
#else
	QWidget* w=NULL;
	try{
		QString file = psUIFile;
		w = QWidgetFactory::create(file);
	}catch(...){
		PyErr_SetString(PyExc_RuntimeError, "Cannot create resource");
		return NULL;
	}

	try{
		QDialog* dlg = NULL;
		if (w->inherits("QDialog"))
		{
			dlg = (QDialog*)w;
		}
		else
		{
			dlg = new FCContainerDialog(w);
		}

		dlg->exec();

		QString input="";
		QObjectList *l = dlg->queryList( "QLineEdit" );
		QObjectListIt it( *l );
		QObject *obj;

		while ( (obj = it.current()) != 0 ) {
				// for each found object...
				++it;
				input = ((QLineEdit*)obj)->text();
		}
		delete l; // delete the list, not the objects
		delete dlg;

		PyObject *pItem;
		pItem = PyString_FromString(input.latin1());

		return pItem;
	} catch (...){
		return NULL;
	}
#endif
}
