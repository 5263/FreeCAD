/** \file main.cpp
 *  \brief Main!
 *  \author $Author$
 *  \version $Revision$
 *  \date    $Date$
 */

/***************************************************************************
 *   (c) J�rgen Riegel (juergen.riegel@web.de) 2002                        *   
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License (GPL)            *
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
 *   Juergen Riegel 2002                                                   *
 ***************************************************************************/

#include "../Config.h"


#include <stdio.h>

#ifndef __linux
#	include <direct.h>
#	include <windows.h>
#endif


// FreeCAD Base header
#include "../Base/Export.h"
#include "../Base/Console.h"
#include "../Base/Interpreter.h"
#include "../Base/Parameter.h"
#include "../Base/Exception.h"

// FreeCAD doc header
#include "../App/Application.h"


#include "Standard_Failure.hxx"
#include <xercesc/util/XMLException.hpp>
#include "iostream"

// FreeCAD Gui header

#ifdef  _FC_GUI_ENABLED_
#  include <qapplication.h>
#  include "../Gui/Application.h"
#	include "../Gui/GuiConsole.h"
#  ifdef WNT
#    pragma comment(lib,"qt-mt230nc.lib")
#  endif 
#endif



const char sBanner[] = \
"  #####                 ####  ###   ####  \n" \
"  #                    #      # #   #   # \n" \
"  #     ##  #### ####  #     #   #  #   # \n" \
"  ####  # # #  # #  #  #     #####  #   # \n" \
"  #     #   #### ####  #    #     # #   # \n" \
"  #     #   #    #     #    #     # #   #  ##  ##  ##\n" \
"  #     #   #### ####   ### #     # ####   ##  ##  ##\n\n" ;


#ifndef __linux
// scriptings
#include "InitScript.h"
#include "TestScript.h"
#include "TestEnvScript.h"
#include "InstallScript.h"
#else
// this might be a cleaner approach? (Besides path to scripts)
//const char FreeCADInit[]="execfile('./Main/FreeCADInit.py')";
//const char FreeCADTest[]="execfile('./Main/FreeCADTest.py')";
//const char FreeCADTestEnv[]="execfile('./Main/FreeCADTestEnv.py')";
#endif

#include <string>

void PrintInitHelp(void);

// globals
FCParameterManager *pcGlobalParameter;

// run control
#ifdef _FC_GUI_ENABLED_
	int RunMode = 0;
#else
	int RunMode = 1;
#endif

FCstring sFileName;
const char*     sScriptName;

// forwards
void Init(int argc, char ** argv );
void ParsOptions(int argc, char ** argv);
void CheckEnv(void);




int main( int argc, char ** argv ) {

	// Init phase ===========================================================
	try{
		// first check the environment variables
		CheckEnv();

		// �nitialization (phase 1)
		Init(argc,argv);

		// the FreeCAD Application

		GetApplication();


	}
	// catch all OCC exceptions
	catch(Standard_Failure e)
	{
		Handle(Standard_Failure) E = Standard_Failure::Caught();
		cout << "An Open CasCade exception was caught:"<< endl << E << endl;
		PrintInitHelp();
		exit(10);
	}
	// catch all FC exceptions
	catch(FCException e)
	{
		GetConsole().Error("Application init failed:");
		e.ReportException();
		PrintInitHelp();
		exit(20);
	}
	// catch XML exceptions
    catch (XMLException& e)
    {
		GetConsole().Error("Application init failed:");
		GetConsole().Error(StrX(e.getMessage()).c_str());
		PrintInitHelp();
		exit(30);
    }

	// catch all the (nasty) rest
	catch(...)
	{
		GetConsole().Error("Application init failed, because of a really nesty (unknown) error...");
		PrintInitHelp();
		exit(40);
	}


	// Run phase ===========================================================

	int ret;
	try{
		switch(RunMode)
		{
		case 0:{
		// run GUI
#			ifdef _FC_GUI_ENABLED_
				// A new QApplication
				GetConsole().Log("Creating GUI Application...\n");
				QApplication* pcQApp = new QApplication ( argc, argv );

				ApplicationWindow * mw = new ApplicationWindow();
				pcQApp->setMainWidget(mw);
				ApplicationWindow::Instance->setCaption( "FreeCAD" );

				GetConsole().Log("Showing GUI Application...\n");
				mw->show();
				pcQApp->connect( pcQApp, SIGNAL(lastWindowClosed()), pcQApp, SLOT(quit()) );
				// run the Application event loop
				GetConsole().Log("Running event loop...\n");
				ret = pcQApp->exec();
				GetConsole().Log("event loop left\n");
				delete pcQApp;

#			else
				GetConsole().Error("GUI mode not possible. This is a FreeCAD compiled without GUI. Use FreeCAD -c\n");
#			endif
				break;
			}
		case 1:
			// Run the comandline interface
			ret = GetInterpreter().RunCommandLine("Console mode");
			break;
		case 2:
			// run a script
			GetConsole().Log("Running script: %s\n",sFileName.c_str());
			GetInterpreter().LaunchFile(sFileName.c_str());
			break;
		case 3:
			// run internal script
			GetConsole().Log("Running internal script:\n");
			GetInterpreter().Launch(sScriptName);
			break;
		default:
			assert(0);
		}
	}
	catch(Standard_Failure e)
	{
		GetConsole().Error("Running the application failed, OCC exception caught:\n");
		Handle(Standard_Failure) E = Standard_Failure::Caught();
		cout << "An exception was caught " << E << endl;
		exit(4);
	}
	catch(FCException e)
	{
		GetConsole().Error("Running the application failed:\n");
		e.ReportException();
		exit(5); 
	}
	catch(...)
	{
		GetConsole().Error("Running the application failed, because of a really nesty (unknown) error...\n\n");
		exit(6);
	}

	// Destruction phase ===========================================================
	GetConsole().Log("FreeCAD terminating...\n\n");

	pcGlobalParameter->SaveDocument("AppParam.FCParam");
	delete pcGlobalParameter;

	GetConsole().Log("FreeCAD completely terminated\n\n");

	return 0;
}



#ifdef FREECADMAINPY
BOOL APIENTRY DllMain( HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved){return TRUE;}

extern "C" {
#ifdef _DEBUG
void __declspec(dllexport) initFreeCADDCmdPy() {
#else
void __declspec(dllexport) initFreeCADCmdPy() {
#endif

	GetApplication();
	return;
}
}
#endif

//************************************************************************
// Init()
// Initialize all the stuff and running the init script:
// - Launching the FCInterpreter (starting python)
// - Launching the FCConsole
// - Call the Init script ("(FreeCADDir)/scripts/FreeCADInit.py")
//************************************************************************
void Init(int argc, char ** argv )
{
	// init python
	GetInterpreter();
	// std console (Also init the python bindings)
	//GetConsole().AttacheObserver(new FCGUIConsole());
	GetConsole().AttacheObserver(new FCCmdConsoleObserver());
	// file logging fcility
#	ifdef _DEBUG
		GetConsole().AttacheObserver(new FCLoggingConsoleObserver("FreeCAD.log"));
#	endif

	// Banner
	GetConsole().Message("FreeCAD (c) 2001 Juergen Riegel\n\n%s",sBanner);

	pcGlobalParameter = new FCParameterManager();

	//pcGlobalParameter->CreateDocument();
	if(pcGlobalParameter->LoadOrCreateDocument("AppParam.FCParam"))
	{
		GetConsole().Warning("   Parameter not existing, write initial one\n");
		GetConsole().Message("   This Warning means normaly FreeCAD running the first time or the\n"
		                     "   configuration was deleted or moved. You have to reinstall FreeCAD\n"
		                     "   by typing FreeCAD -i. This build up the standard configuration and\n"
		                     "   install all present modules. \n");

	}
/*
	pcGlobalParameter->GetGroup("BaseApp");
	FCHandle<FCParametrGrp> h = pcGlobalParameter->GetGroup("BaseApp");
	h = h->GetGroup("Windows");

	h->SetBool("Works",true);
	bool bTest = h->GetBool("Works");

	h->SetInt("Works",1000);
	long lTest = h->GetInt("Works");

	h->SetFloat("Works",123.23);
	double test = h->GetFloat("Works");

	h->SetASCII("Works","hello");
	char cBuf[256];
	h->GetASCII("Works",cBuf,255);
	FCstring sTrest = h->GetASCII("Works");
	
	
	pcGlobalParameter->SaveDocument("AppParam.FCParam");
	//pcGlobalParameter->CreateDocument();



	exit (0);

*/
	ParsOptions(argc,argv);

	
	// Start the python interpreter
	FCInterpreter &rcInterperter = GetInterpreter();
	rcInterperter.SetComLineArgs(argc,argv);
	rcInterperter.Launch("print 'Python started'\n");
	
	// starting the init script
	rcInterperter.Launch(FreeCADInit);

	// creating the application 
	FCApplication::_pcSingelton = new FCApplication(pcGlobalParameter);

}

//**************************************************************************
// checking the environment


const char sEnvErrorText1[] = \
"It seems some of the variables needed by FreeCAD are not set\n"\
"or wrong set. This regards the Open CasCade variables:\n"\
"CSF_GRAPHICSHR=C:\\CasRoot\\Windows_NT\\dll\\opengl.dll\n"\
"CSF_MDTVFONTDIRECTORY=C:\\CasRoot\\src\\FontMFT\\\n"\
"CSF_MDTVTEXTURESDIRECTORY=C:\\CasRoot\\src\\Textures\\\n"\
"CSF_UNITSDEFINITION=C:\\CasRoot\\src\\UnitsAPI\\Units.dat\n"\
"CSF_UNITSLEXICON=C:\\CasRoot\\src\\UnitsAPI\\Lexi_Expr.dat\n"\
"Please reinstall OpenCasCade!\n\n";

const char sEnvErrorText2[] = \
"It seems some of the variables needed by FreeCAD are not set\n"\
"or wrong set. This regards the Open CasCade variables:\n"\
"XXX=C:\\CasRoot\\Windows_NT\\dll\\opengl.dll\n"\
"Please reinstall XXX!\n\n";


void CheckEnv(void)
{
	int bFailure = 0;
	// set the resource env variables
	char  szString [256] ;
	char  szDirectory [256] ;

	getcwd (szDirectory,sizeof szDirectory);
	if (szDirectory[strlen(szDirectory)-1] != '\\') {
		strcat(szDirectory,"\\");
	}
	
	sprintf(szString,"CSF_StandardDefaults=%s",szDirectory);
	putenv (szString);
//	cout<<szString<<endl;

	sprintf(szString,"CSF_PluginDefaults=%s",szDirectory);
	putenv (szString);
//	cout<<szString<<endl;
        
        
#define TEST_ENVVAR_EXISTS(envvar,type) \
	if (!getenv(envvar)){ \
          cerr<<"Environment variable "<<envvar<<" is not set!"<<endl; \
          bFailure|=type;\
        }  
        TEST_ENVVAR_EXISTS("CSF_MdtvFontDirectory",1)
        TEST_ENVVAR_EXISTS("CSF_MdtvTexturesDirectory",1)
        TEST_ENVVAR_EXISTS("CSF_UnitsDefinition",1)
        TEST_ENVVAR_EXISTS("CSF_UnitsLexicon",1)

        if (bFailure&1) {    
         	cerr<<"Environment Error(s)"<<endl<<sEnvErrorText1;
			exit(1);
        }
        if (bFailure&2) {    
         	cerr<<"Environment Error(s)"<<endl<<sEnvErrorText2;
			exit(1);
        }

#undef TEST_ENVVAR_EXISTS         
         
/*	
	if(  getenv("CASROOT") )
	{
		RSstring CasRoot = getenv("CASROOT");
		TRACE("Environment CASROOT defined, using Cascade from %s\n",CasRoot.c_str());
		_putenv(("CSF_GRAPHICSHR="+CasRoot+"\\Windows_NT\\dll\\opengl.dll").c_str());
		_putenv(("CSF_MDTVFONTDIRECTORY="+CasRoot+"\\src\\FontMFT\\").c_str());
		_putenv(("CSF_MDTVTEXTURESDIRECTORY="+CasRoot+"\\src\\Textures\\").c_str());
		_putenv(("CSF_UNITSDEFINITION="+CasRoot+"\\src\\UnitsAPI\\Units.dat").c_str());
		_putenv(("CSF_UNITSLEXICON="+CasRoot+"\\src\\UnitsAPI\\Lexi_Expr.dat").c_str());
	}else
	{
		TRACE("Environment CASROOT NOT defined, using Cascade from intallation\n");
		_putenv("CSF_GRAPHICSHR=.\\opengl.dll");
		TRACE("CSF_GRAPHICSHR=%s\n",getenv("CSF_GRAPHICSHR"));
		_putenv("CSF_MDTVFONTDIRECTORY=.\\data\\");
		_putenv("CSF_MDTVTEXTURESDIRECTORY=.\\data\\");
		_putenv("CSF_UNITSDEFINITION=.\\data\\Units.dat");
		_putenv("CSF_UNITSLEXICON=.\\data\\Lexi_Expr.dat");
	}
*/

}

const char Usage[] = \
" [Options] files..."\
"Options:\n"\
"  -h             Display this information "\
"  -c             Runs FreeCAD in console mode (no windows)\n"\
"  -cf file-name  Runs FreeCAD in server mode with script file-name\n"\
"  -te            Runs FreeCAD to test environment\n"\
"  -t0            Runs FreeCAD self test function\n"\
"  -i             Install a new Module (e.g.ModuleName_0.1.FCModule) and the rest\n"\
"\n consult also the HTML documentation\n"\
"";


void ParsOptions(int argc, char ** argv)
{
	// scan command line arguments for user input. 
	for (int i = 1; i < argc; i++) 
	{ 
		if (*argv[i] == '-' ) 
		{ 
			switch (argv[i][1]) 
			{ 
			// Console modes 
			case 'c': 
			case 'C':  
				switch (argv[i][2])  
				{   
					// Console with file
					case 'f':  
					case 'F':  
						RunMode = 2;
						if(argc <= i+1)
						{
							GetConsole().Error("Expecting a file\n");  
							GetConsole().Error("\nUsage: %s %s",argv[0],Usage);  
						}
						sFileName = argv[i+1];
						i++;
						break;   
					case '\0':  
						RunMode = 1;
						break;   
					default:  
						GetConsole().Error("Invalid Input %s\n",argv[i]);  
						GetConsole().Error("\nUsage: %s %s",argv[0],Usage);  
						throw FCException("Comandline error(s)");  
				};  
				break;  
			case 't': 
			case 'T':  
				switch (argv[i][2])  
				{   
					// run the test environment script
					case 'e':  
					case 'E':  
						RunMode = 3;
						sScriptName = FreeCADTestEnv;
						break;   
					case '0':  
						// test script level 0
						RunMode = 3;
						sScriptName = FreeCADTest;
						break;   
					default:  
						//default testing level 0
						RunMode = 3;
						sScriptName = FreeCADTest;
						break;   
				};  
				break;  
			case 'i': 
			case 'I':  
				RunMode = 3;
				sScriptName = FreeCADInstall;
				break;  
			case '?': 
			case 'h': 
			case 'H': 
				GetConsole().Message("\nUsage: %s %s",argv[0],Usage);
				throw FCException("Comandline break");  
				break;  
			default: 
				GetConsole().Error("Invalid Option: %s\n",argv[i]); 
				GetConsole().Error("\nUsage: %s %s",argv[0],Usage); 
				throw FCException("Comandline error(s)");  
			} 
		} 
		else  
		{ 
			GetConsole().Error("Illegal command line argument #%d, %s\n",i,argv[i]); 
			GetConsole().Error("\nUsage: %s %s",argv[0],Usage); 
			throw FCException("Comandline error(s)");  
		} 
	}  
}  


void PrintInitHelp(void)
{
	cerr << endl << endl
		 << "  An initializing error was caught. This means mainly" << endl
		 << "  FreeCAD is not installed properly. Type \"FreeCAD -i\""<< endl
		 << "  to reinstall FreeCAD." << endl << endl
		 << "  Good luck ;-)" << endl << endl;
}


