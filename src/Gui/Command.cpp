/***************************************************************************
 *   Copyright (c) 2002 J�rgen Riegel <juergen.riegel@web.de>              *
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

#ifndef __Qt4All__
# include "Qt4All.h"
#endif

#include "Command.h"
#include "Action.h"
#include "Application.h"
#include "Document.h"
#include "Selection.h"
#include "HelpView.h"
#include "Macro.h"
#include "MainWindow.h"
#include "DlgUndoRedo.h"
#include "BitmapFactory.h"
#include "WhatsThis.h"
#include "WaitCursor.h"

#include <Base/Console.h>
#include <Base/Exception.h>
#include <Base/Interpreter.h>
#include <Base/Sequencer.h>

#include <App/Document.h>
#include <App/Feature.h>
#include <App/DocumentObject.h>


using Base::Interpreter;
using namespace Gui;
using namespace Gui::Dialog;
using namespace Gui::DockWnd;

/** \defgroup commands Command Framework
 * \section Overview
 * In GUI applications many commands can be invoked via a menu item, a toolbar button or an accelerator key. The answer of Qt to master this
 * challenge is the class \a QAction. A QAction object can be added to a popup menu or a toolbar and keep the state of the menu item and
 * the toolbar button synchronized.
 *
 * For example, if the user clicks the menu item of a toggle action then the toolbar button gets also pressed
 * and vice versa. For more details refer to your Qt documentation.
 *
 * \section Drawbacks
 * Since QAction inherits QObject and emits the \a triggered() signal or \a toggled() signal for toggle actions it is very convenient to connect
 * these signals e.g. with slots of your MainWindow class. But this means that for every action an appropriate slot of MainWindow is necessary
 * and leads to an inflated MainWindow class. Furthermore, it's simply impossible to provide plugins that may also need special slots -- without
 * changing the MainWindow class.
 *
 * \section wayout Way out
 * To solve these problems we have introduced the command framework to decouple QAction and MainWindow. The base classes of the framework are
 * \a Gui::CommandBase and \a Gui::Action that represent the link between Qt's QAction world and the FreeCAD's command  world. 
 *
 * The Action class holds a pointer to QAction and CommandBase and acts as a mediator and -- to save memory -- that gets created 
 * (@ref CommandBase::createAction()) not before it is added (@ref Command::addTo()) to a menu or toolbar.
 *
 * Now, the implementation of the slots of MainWindow can be done in the method \a activated() of subclasses of Command instead.
 *
 * For example, the implementation of the "Open file" command can be done as follows.
 * \code
 * class OpenCommand : public Command
 * {
 * public:
 *   OpenCommand() : Command("Std_Open")
 *   {
 *     // set up menu text, status tip, ...
 *     sMenuText     = "&Open";
 *     sToolTipText  = "Open a file";
 *     sWhatsThis    = "Open a file";
 *     sStatusTip    = "Open a file";
 *     sPixmap       = "Open"; // name of a registered pixmap
 *     iAccel        = Qt::CTRL+Qt::Key_O;
 *   }
 * protected:
 *   void activated(int)
 *   {
 *     QString filter ... // make a filter of all supported file formats
 *     QStringList FileList = QFileDialog::getOpenFileNames( filter,QString::null, getMainWindow() );
 *     for ( QStringList::Iterator it = FileList.begin(); it != FileList.end(); ++it ) {
 *       getGuiApplication()->open((*it).latin1());
 *     }
 *   }
 * };
 * \endcode
 * An instance of \a OpenCommand must be created and added to the \ref CommandManager to make the class known to FreeCAD.
 * To see how menus and toolbars can be built go to the @ref workbench.
 *
 * @see Gui::Command, Gui::CommandManager
 */

CommandBase::CommandBase( const char* sMenu, const char* sToolTip, const char* sWhat,
                          const char* sStatus, const char* sPixmap, int iAcc)
        : sMenuText(sMenu), sToolTipText(sToolTip), sWhatsThis(sWhat?sWhat:sToolTip),
        sStatusTip(sStatus?sStatus:sToolTip), sPixmap(sPixmap), iAccel(iAcc), _pcAction(0)
{
}

CommandBase::~CommandBase()
{
    //Note: The Action object becomes a children of MainWindow which gets destoyed _before_ the
    //command manager hence before any command object. So the action pointer is a dangling pointer
    //at this state.
}

Action* CommandBase::getAction() const
{
    return _pcAction;
}

Action * CommandBase::createAction()
{
    // does nothing
    return 0;
}

void CommandBase::languageChange()
{
    if ( _pcAction ) {
        _pcAction->setText       ( QObject::tr( sMenuText    ) );
        _pcAction->setToolTip    ( QObject::tr( sToolTipText ) );
        if ( sStatusTip )
            _pcAction->setStatusTip ( QObject::tr( sStatusTip   ) );
        else
            _pcAction->setStatusTip ( QObject::tr( sToolTipText ) );
        if ( sWhatsThis )
            _pcAction->setWhatsThis( QObject::tr( sWhatsThis   ) );
        else
            _pcAction->setWhatsThis( QObject::tr( sToolTipText ) );
    }
}

//===========================================================================
// Command
//===========================================================================

/* TRANSLATOR Gui::Command */

Command::Command(const char* name)
        : CommandBase(0), sName(name), sHelpUrl(0)
{
    sAppModule  = "FreeCAD";
    sGroup      = QT_TR_NOOP("Standard");
}

Command::~Command()
{
}

bool Command::isViewOfType(Base::Type t) const
{
    Gui::Document *d = getGuiApplication()->activeDocument();
    if (!d) return false;
    Gui::BaseView *v = d->getActiveView();
    if (!v) return false;
    if (v->getTypeId().isDerivedFrom(t))
        return true;
    else
        return false;
}

void Command::addTo(QWidget *pcWidget)
{
    if (!_pcAction)
        _pcAction = createAction();

    _pcAction->addTo(pcWidget);
}

Application *Command::getGuiApplication(void)
{
    return Application::Instance;
}

Gui::Document* Command::getActiveGuiDocument(void)
{
    return getGuiApplication()->activeDocument();
}

App::Document* Command::getDocument(const char* Name)
{
    if (Name)
        return App::GetApplication().getDocument(Name);
    else {
        Gui::Document * pcDoc = getGuiApplication()->activeDocument();
        if (pcDoc)
            return pcDoc->getDocument();
        else
            return 0l;
    }
}

App::AbstractFeature* Command::getFeature(const char* Name)
{
    App::DocumentObject *pObj = getObject(Name);
    if (pObj && pObj->getTypeId().isDerivedFrom(App::AbstractFeature::getClassTypeId()))
        return dynamic_cast<App::AbstractFeature*>(pObj);
    else
        return 0;
}

App::DocumentObject* Command::getObject(const char* Name)
{
    App::Document*pDoc = getDocument();
    if (pDoc)
        return pDoc->getObject(Name);
    else
        return 0;
}

void Command::invoke (int i)
{
    // Do not query _pcAction since it isn't created necessarily
#ifdef FC_LOGUSERACTION
    Base::Console().Log("CmdG: %s\n",sName);
#endif
    // set the application module type for the macro
    getGuiApplication()->macroManager()->setModule(sAppModule);
    try {
        // check if it really works NOW (could be a delay between click deactivation of the button)
        if (isActive())
            activated( i );
    }
    catch (Base::PyException &e) {
        e.ReportException();
        Base::Console().Error("Stack Trace: %s\n",e.getStackTrace().c_str());
    }
    catch (Base::AbortException&) {
    }
    catch (Base::Exception &e) {
        e.ReportException();
        // Pop-up a dialog for FreeCAD-specific exceptions
        QMessageBox::critical(Gui::getMainWindow(), QObject::tr("Exception"), QLatin1String(e.what()));
    }
    catch (std::exception &e) {
        std::string str;
        str += "C++ exception thrown (";
        str += e.what();
        str += ")";
        Base::Console().Error(str.c_str());
    }
    catch (const char* e) {
        Base::Console().Error("%s\n", e);
    }
#ifndef FC_DEBUG
    catch (...) {
        Base::Console().Error("Gui::Command::activated(%d): Unknown C++ exception in command thrown", i);
    }
#endif
}

void Command::testActive(void)
{
    if (!_pcAction) return;

    if ( _blockCmd ) {
        _pcAction->setEnabled ( false );
        return;
    }

    bool bActive = isActive();
    _pcAction->setEnabled ( bActive );
}

//--------------------------------------------------------------------------
// Helper methods
//--------------------------------------------------------------------------

bool Command::hasActiveDocument(void)
{
    return getActiveGuiDocument() != 0;
}
/// true when there is a document and a Feature with Name
bool Command::hasObject(const char* Name)
{
    return getDocument() != 0 && getDocument()->getObject(Name) != 0;
}

Gui::SelectionSingleton&  Command::getSelection(void)
{
    return Gui::Selection();
}

std::string Command::getUniqueObjectName(const char *BaseName)
{
    assert(hasActiveDocument());

    return getActiveGuiDocument()->getDocument()->getUniqueObjectName(BaseName);
}


//--------------------------------------------------------------------------
// UNDO REDO transaction handling
//--------------------------------------------------------------------------
/** Open a new Undo transaction on the active document
 *  This method opens a new UNDO transaction on the active document. This transaction
 *  will later apear in the UNDO REDO dialog with the name of the command. If the user
 *  recall the transaction everything changed on the document between OpenCommand() and
 *  CommitCommand will be undone (or redone). You can use an alternetive name for the
 *  operation default is the Command name.
 *  @see CommitCommand(),AbortCommand()
 */
void Command::openCommand(const char* sCmdName)
{
    // Using OpenCommand with no active document !
    assert(getGuiApplication()->activeDocument());

    if (sCmdName)
        getGuiApplication()->activeDocument()->openCommand(sCmdName);
    else
        getGuiApplication()->activeDocument()->openCommand(sName);
}

void Command::commitCommand(void)
{
    getGuiApplication()->activeDocument()->commitCommand();
}

void Command::abortCommand(void)
{
    getGuiApplication()->activeDocument()->abortCommand();
}

bool Command::_blockCmd = false;

void Command::blockCommand(bool block)
{
    Command::_blockCmd = block;
}

/// Run a App level Action
void Command::doCommand(DoCmd_Type eType,const char* sCmd,...)
{
    // temp buffer
    size_t format_len = std::strlen(sCmd)+4024;
    char* format = (char*) malloc(format_len);
    va_list namelessVars;
    va_start(namelessVars, sCmd);  // Get the "..." vars
    vsnprintf(format, format_len, sCmd, namelessVars);
    va_end(namelessVars);

    if (eType == Gui)
        getGuiApplication()->macroManager()->addLine(MacroManager::Gui,format);
    else
        getGuiApplication()->macroManager()->addLine(MacroManager::Base,format);

    try {
        Interpreter().runString(format);
    }
    catch (...) {
        // free memory to avoid a leak if an exception occurred
        free (format);
        throw;
    }

#ifdef FC_LOGUSERACTION
    Base::Console().Log("CmdC: %s\n",format);
#endif
    free (format);
}


const std::string Command::strToPython(const char* Str)
{
    return Base::InterpreterSingleton::strToPython(Str);
}

/// Updates the (active) document (propagate changes)
void Command::updateActive(void)
{
    WaitCursor wc;

    getGuiApplication()->activeDocument()->getDocument()->recompute();
    //getGuiApplication()->UpdateActive();
}

/// Updates the (all or listed) documents (propagate changes)
void Command::updateAll(std::list<Gui::Document*> cList)
{
    if (cList.size()>0) {
        for (std::list<Gui::Document*>::iterator It= cList.begin();It!=cList.end();It++)
            (*It)->onUpdate();
    }
    else {
        getGuiApplication()->onUpdate();
    }
}

//--------------------------------------------------------------------------
// Online help handling
//--------------------------------------------------------------------------

/// returns the begin of a online help page
const char * Command::beginCmdHelp(void)
{
    return  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n"
            "<html>\n"
            "<head>\n"
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\">\n"
            "<title>FreeCAD Main Index</title>\n"
            "</head>\n"
            "<body bgcolor=\"#ffffff\">\n\n";
}
/// returns the end of a online help page
const char * Command::endCmdHelp(void)
{
    return "</body></html>\n\n";
}

Action * Command::createAction(void)
{
    Action *pcAction;

    pcAction = new Action(this,getMainWindow());
    pcAction->setText(QObject::tr(sMenuText));
    pcAction->setToolTip(QObject::tr(sToolTipText));
    if ( sStatusTip )
        pcAction->setStatusTip(QObject::tr(sStatusTip));
    else
        pcAction->setStatusTip(QObject::tr(sToolTipText));
    if ( sWhatsThis )
        pcAction->setWhatsThis(QObject::tr(sWhatsThis));
    else
        pcAction->setWhatsThis(QObject::tr(sToolTipText));
    if (sPixmap)
        pcAction->setIcon(Gui::BitmapFactory().pixmap(sPixmap));
    pcAction->setShortcut(iAccel);

    return pcAction;
}

//===========================================================================
// MacroCommand
//===========================================================================

/* TRANSLATOR Gui::MacroCommand */

MacroCommand::MacroCommand(const char* name)
#if defined (_MSC_VER)
        : Command( _strdup(name) )
#else
        : Command( strdup(name) )
#endif
{
    sGroup = QT_TR_NOOP("Macros");
}

void MacroCommand::activated(int iMsg)
{
    std::string cMacroPath = App::GetApplication().GetParameterGroupByPath
                             ("User parameter:BaseApp/Preferences/Macro")->GetASCII("MacroPath",
                                     App::GetApplication().GetHomePath());

    QDir d(QString::fromUtf8(cMacroPath.c_str()));
    QFileInfo fi(d, QString::fromUtf8(sScriptName));
    Application::Instance->macroManager()->run(MacroManager::File, fi.filePath().toUtf8());
    // after macro run recalculate the document
    if ( Application::Instance->activeDocument() )
        Application::Instance->activeDocument()->getDocument()->recompute();
}

Action * MacroCommand::createAction(void)
{
    Action *pcAction;
    pcAction = new Action(this,getMainWindow());
    pcAction->setText(QString::fromUtf8(sMenuText));
    pcAction->setToolTip(QString::fromUtf8(sToolTipText));
    pcAction->setStatusTip(QString::fromUtf8(sStatusTip));
    pcAction->setWhatsThis(QString::fromUtf8(sWhatsThis));
    if ( sPixmap )
        pcAction->setIcon(Gui::BitmapFactory().pixmap(sPixmap));
    pcAction->setShortcut(iAccel);
    return pcAction;
}

void MacroCommand::setMenuText( const char* s )
{
#if defined (_MSC_VER)
    this->sMenuText = _strdup( s );
#else
    this->sMenuText = strdup( s );
#endif
}

void MacroCommand::setToolTipText( const char* s )
{
#if defined (_MSC_VER)
    this->sToolTipText = _strdup( s );
#else
    this->sToolTipText = strdup( s );
#endif
}

void MacroCommand::setStatusTip( const char* s )
{
#if defined (_MSC_VER)
    this->sStatusTip = _strdup( s );
#else
    this->sStatusTip = strdup( s );
#endif
}

void MacroCommand::setWhatsThis( const char* s )
{
#if defined (_MSC_VER)
    this->sWhatsThis = _strdup( s );
#else
    this->sWhatsThis = strdup( s );
#endif
}

void MacroCommand::setPixmap( const char* s )
{
#if defined (_MSC_VER)
    this->sPixmap = _strdup( s );
#else
    this->sPixmap = strdup( s );
#endif
}

void MacroCommand::setScriptName( const char* s )
{
#if defined (_MSC_VER)
    this->sScriptName = _strdup( s );
#else
    this->sScriptName = strdup( s );
#endif
}

void MacroCommand::setAccel(int i)
{
    iAccel = i;
}

void MacroCommand::load()
{
    ParameterGrp::handle hGrp = App::GetApplication().GetParameterGroupByPath("User parameter:BaseApp/Macro");

    if (hGrp->HasGroup("Macros")) {
        hGrp = hGrp->GetGroup("Macros");
        std::vector<Base::Reference<ParameterGrp> > macros = hGrp->GetGroups();
        for (std::vector<Base::Reference<ParameterGrp> >::iterator it = macros.begin(); it!=macros.end(); ++it ) {
            MacroCommand* macro = new MacroCommand((*it)->GetGroupName());
            macro->setScriptName  ( (*it)->GetASCII( "Script"     ).c_str() );
            macro->setMenuText    ( (*it)->GetASCII( "Menu"       ).c_str() );
            macro->setToolTipText ( (*it)->GetASCII( "Tooltip"    ).c_str() );
            macro->setWhatsThis   ( (*it)->GetASCII( "WhatsThis"  ).c_str() );
            macro->setStatusTip   ( (*it)->GetASCII( "Statustip"  ).c_str() );
            if ((*it)->GetASCII("Pixmap", "nix") != "nix")
                macro->setPixmap    ( (*it)->GetASCII( "Pixmap"     ).c_str() );
            macro->setAccel       ( (*it)->GetInt  ( "Accel",0    )         );
            Application::Instance->commandManager().addCommand( macro );
        }
    }
}

void MacroCommand::save()
{
    ParameterGrp::handle hGrp = App::GetApplication().GetParameterGroupByPath("User parameter:BaseApp/Macro")->GetGroup("Macros");
    hGrp->Clear();

    std::vector<Command*> macros = Application::Instance->commandManager().getGroupCommands("Macros");
    if ( macros.size() > 0 ) {
        for (std::vector<Command*>::iterator it = macros.begin(); it!=macros.end(); ++it ) {
            MacroCommand* macro = (MacroCommand*)(*it);
            ParameterGrp::handle hMacro = hGrp->GetGroup(macro->getName());
            hMacro->SetASCII( "Script",    macro->getScriptName () );
            hMacro->SetASCII( "Menu",      macro->getMenuText   () );
            hMacro->SetASCII( "Tooltip",   macro->getToolTipText() );
            hMacro->SetASCII( "WhatsThis", macro->getWhatsThis  () );
            hMacro->SetASCII( "Statustip", macro->getStatusTip  () );
            hMacro->SetASCII( "Pixmap",    macro->getPixmap     () );
            hMacro->SetInt  ( "Accel",     macro->getAccel      () );
        }
    }
}

//===========================================================================
// PythonCommand
//===========================================================================

PythonCommand::PythonCommand(const char* name,PyObject * pcPyCommand, const char* pActivationString)
        :Command(name),_pcPyCommand(pcPyCommand)
{
    if (pActivationString)
        Activation = pActivationString;

    sGroup = "Python";

    Py_INCREF(_pcPyCommand);

    // call the method "GetResources()" of the command object
    _pcPyResourceDict = Interpreter().runMethodObject(_pcPyCommand, "GetResources");
    // check if the "GetResources()" method returns a Dict object
    if (! PyDict_Check(_pcPyResourceDict) )
        throw Base::Exception("PythonCommand::PythonCommand(): Method GetResources() of the Python command object returns the wrong type (has to be Py Dictonary)");
}

const char* PythonCommand::getResource(const char* sName) const
{
    PyObject* pcTemp;

    // get the "MenuText" resource string
    pcTemp = PyDict_GetItemString(_pcPyResourceDict,sName);
    if (! pcTemp )
        return "";
    if (! PyString_Check(pcTemp) )
        throw Base::Exception("PythonCommand::getResource(): Method GetResources() of the Python command object returns a dictionary which holds not only strings");

    return PyString_AsString(pcTemp);
}

void PythonCommand::activated(int iMsg)
{
    if (Activation == "") {
        try {
            Interpreter().runMethodVoid(_pcPyCommand, "Activated");
        }
        catch ( const Base::PyException& e) {
            Base::Console().Error("Running the Python command '%s' failed:\n%s\n%s",
                                  sName, e.getStackTrace().c_str(), e.what());
        }
        catch ( const Base::Exception&) {
            Base::Console().Error("Running the Python command '%s' failed, try to resume",sName);
        }
    }
    else {
        doCommand(Doc,Activation.c_str());
    }
}

bool PythonCommand::isActive(void)
{
    try {
        Base::PyGILStateLocker lock;
        Py::Object cmd(_pcPyCommand);
        if (cmd.hasAttr("IsActive")) {
            Py::Callable call(cmd.getAttr("IsActive"));
            Py::Tuple args;
            Py::Object ret = call.apply(args);
            // if return type is not boolean or not true
            if (!PyBool_Check(ret.ptr()) || ret.ptr() != Py_True)
                return false;
        }
    }
    catch(Py::Exception& e) {
        Base::PyGILStateLocker lock;
        e.clear();
        return false;
    }

    return true;
}

void PythonCommand::languageChange()
{
    if ( _pcAction ) {
        _pcAction->setText       ( QObject::tr( getMenuText()    ) );
        _pcAction->setToolTip    ( QObject::tr( getToolTipText() ) );
        _pcAction->setStatusTip  ( QObject::tr( getStatusTip()   ) );
        _pcAction->setWhatsThis  ( QObject::tr( getWhatsThis()   ) );
    }
}

const char* PythonCommand::getHelpUrl(void)
{
    PyObject* pcTemp;
    pcTemp = Interpreter().runMethodObject(_pcPyCommand, "CmdHelpURL");
    if (! pcTemp )
        return "";
    if (! PyString_Check(pcTemp) )
        throw Base::Exception("PythonCommand::CmdHelpURL(): Method CmdHelpURL() of the Python command object returns no string");
    return PyString_AsString(pcTemp);
}

Action * PythonCommand::createAction(void)
{
    Action *pcAction;

    pcAction = new Action(this,getMainWindow());
//  pcAction->setText(sName);
    pcAction->setText(QString::fromUtf8(getResource("MenuText")));
    pcAction->setToolTip(QString::fromUtf8(getResource("ToolTip")));
    pcAction->setStatusTip(QString::fromUtf8(getResource("StatusTip")));
    pcAction->setWhatsThis(QString::fromUtf8(getResource("WhatsThis")));
    if (strcmp(getResource("Pixmap"),"") != 0)
        pcAction->setIcon(Gui::BitmapFactory().pixmap(getResource("Pixmap")));

    if ( pcAction->statusTip().isEmpty() )
        pcAction->setStatusTip(QString::fromUtf8(getResource("ToolTip")));
    if ( pcAction->whatsThis().isEmpty() )
        pcAction->setWhatsThis(QString::fromUtf8(getResource("ToolTip")));

    return pcAction;
}

const char* PythonCommand::getWhatsThis() const
{
    return getResource("WhatsThis");
}

const char* PythonCommand::getMenuText() const
{
    return getResource("MenuText");
}

const char* PythonCommand::getToolTipText() const
{
    return getResource("ToolTip");
}

const char* PythonCommand::getStatusTip() const
{
    return getResource("StatusTip");
}

const char* PythonCommand::getPixmap() const
{
    return getResource("Pixmap");
}

int PythonCommand::getAccel() const
{
    return 0;
}

//===========================================================================
// CommandManager
//===========================================================================

CommandManager::CommandManager()
{
}

CommandManager::~CommandManager()
{
    clearCommands();
}

void CommandManager::addCommand(Command* pCom)
{
    _sCommands[pCom->getName()] = pCom;//	pCom->Init();
}

void CommandManager::removeCommand(Command* pCom)
{
    std::map <std::string,Command*>::iterator It = _sCommands.find(pCom->getName());
    if (It != _sCommands.end()) {
        delete It->second;
        _sCommands.erase(It);
    }
}

void CommandManager::clearCommands()
{
    for ( std::map<std::string,Command*>::iterator it = _sCommands.begin(); it != _sCommands.end(); ++it )
        delete it->second;
    _sCommands.clear();
}

bool CommandManager::addTo(const char* Name,QWidget *pcWidget)
{
    if (_sCommands.find(Name) == _sCommands.end()) {
        // Print in release mode only a log message instead of an error message to avoid to annoy the user
#ifdef FC_DEBUG
        Base::Console().Error("CommandManager::addTo() try to add an unknown command (%s) to a widget!\n",Name);
#else
        Base::Console().Log("CommandManager::addTo() try to add an unknown command (%s) to a widget!\n",Name);
#endif
        return false;
    }
    else {
        Command* pCom = _sCommands[Name];
        pCom->addTo(pcWidget);
        return true;
    }
}

std::vector <Command*> CommandManager::getModuleCommands(const char *sModName) const
{
    std::vector <Command*> vCmds;

    for ( std::map<std::string, Command*>::const_iterator It= _sCommands.begin();It!=_sCommands.end();It++) {
        if ( strcmp(It->second->getAppModuleName(),sModName) == 0)
            vCmds.push_back(It->second);
    }

    return vCmds;
}

std::vector <Command*> CommandManager::getAllCommands(void) const
{
    std::vector <Command*> vCmds;

    for ( std::map<std::string, Command*>::const_iterator It= _sCommands.begin();It!=_sCommands.end();It++) {
        vCmds.push_back(It->second);
    }

    return vCmds;
}

std::vector <Command*> CommandManager::getGroupCommands(const char *sGrpName) const
{
    std::vector <Command*> vCmds;

    for ( std::map<std::string, Command*>::const_iterator It= _sCommands.begin();It!=_sCommands.end();It++) {
        if ( strcmp(It->second->getGroupName(),sGrpName) == 0)
            vCmds.push_back(It->second);
    }

    return vCmds;
}

Command* CommandManager::getCommandByName(const char* sName) const
{
    std::map<std::string,Command*>::const_iterator it = _sCommands.find( sName );
    return ( it != _sCommands.end() ) ? it->second : 0;
}

void CommandManager::runCommandByName (const char* sName) const
{
    Command* pCmd = getCommandByName(sName);

    if (pCmd)
        pCmd->invoke(0);
}

void CommandManager::testActive(void)
{
    for ( std::map<std::string, Command*>::iterator It= _sCommands.begin();It!=_sCommands.end();It++) {
        It->second->testActive();
    }
}

