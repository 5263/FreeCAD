/** \file MainGui.cpp
 *  \brief The main for the gui application
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

#ifdef _PreComp_
# undef _PreComp_
#endif

#ifdef FC_LINUX
#	include <unistd.h>
#endif

#if HAVE_CONFIG_H
#	include <config.h>
#endif // HAVE_CONFIG_H

#include <stdio.h>


// FreeCAD Base header
#include "../Base/Console.h"
#include "../Base/Interpreter.h"
#include "../Base/Parameter.h"
#include "../Base/Exception.h"
#include "../Base/EnvMacros.h"
#include "../Base/Factory.h"

// FreeCAD doc header
#include "../App/Application.h"


// QT extension
#ifdef _MSC_VER
#	pragma comment(lib,QTLIBNAME)
#	pragma comment(lib,"TKernel.lib")
#endif

#include "../Gui/Application.h"


void PrintInitHelp(void);




int main( int argc, char ** argv )
{

	// find the home path....
	std::string sHomePath;
#	ifdef FC_OS_WIN32
		sHomePath = FindHomePathWin32(0);
#	else
		sHomePath = FindHomePathUnix(argv[0]);
#	endif

		// try to figure out if using FreeCADLib
		std::string Temp = GetFreeCADLib(sHomePath.c_str());

		// sets all needed varables if a FreeCAD LibPack is found
		if(Temp != "")
		{
			EnvPrint("MeinGui Set Python ++++++++++++++++++++++++++++++++++++++++++++++");
			// sets the python environment variables if the FREECADLIB variable is defined
			SetPythonToFreeCADLib(Temp.c_str());
		}
	// Init phase ===========================================================
	// sets the default run mode for FC, starts with gui if not overridden in InitConfig...
	App::Application::SetRunMode("Gui");

	// parse the options 
	App::Application::InitConfig(argc,argv,sHomePath.c_str());

	App::Application::InitApplication();

  Gui::ApplicationWindow::initApplication();

	// dumps the configuration to the console
//	Application::DumpConfig();

	// Run phase ===========================================================

	if(App::GetApplication().Config()["RunMode"] == "Gui")
	{
	// run GUI
    Gui::ApplicationWindow::runApplication();

    Gui::ApplicationWindow::destruct();

	} else {

		App::Application::RunApplication();
	}


	// Destruction phase ===========================================================
	Base::Console().Log("FreeCAD terminating...\n\n");

	// cleans up 
	App::Application::Destruct();

	Base::Console().Log("FreeCAD completely terminated\n\n");

	return 0;
}

#ifdef _WIN32 
#include <windows.h> 

int __stdcall WinMain(HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, int nShowCmd) 
{ 
  int          argc; 
  int          retVal; 
  char**       argv; 
  unsigned int i; 
  int          j; 

  // parse a few of the command line arguments 
  // a space delimites an argument except when it is inside a quote 

  argc = 1; 
  int pos = 0; 
  for (i = 0; i < strlen(lpCmdLine); i++) { 
    while (lpCmdLine[i] == ' ' && i < strlen(lpCmdLine)) i++; 
    if (lpCmdLine[i] == '\"') { 
      i++; 
      while (lpCmdLine[i] != '\"' && i < strlen(lpCmdLine)) { 
        i++; 
        pos++; 
      } 
      argc++; 
      pos = 0; 
    } else { 
      while (lpCmdLine[i] != ' ' && i < strlen(lpCmdLine)) { 
        i++; 
        pos++; 
      } 
      argc++; 
      pos = 0; 
    } 
  } 
  argv = (char**)malloc(sizeof(char*)* (argc+1)); 

  argv[0] = (char*)malloc(1024); 
  ::GetModuleFileName(0, argv[0],1024); 

  for(j=1; j<argc; j++) { 
    argv[j] = (char*)malloc(strlen(lpCmdLine)+10); 
  } 
  argv[argc] = 0; 

  argc = 1; 
  pos = 0; 
  for (i = 0; i < strlen(lpCmdLine); i++) { 
    while (lpCmdLine[i] == ' ' && i < strlen(lpCmdLine)) i++; 
    if (lpCmdLine[i] == '\"') { 
      i++; 
      while (lpCmdLine[i] != '\"' && i < strlen(lpCmdLine)) { 
        argv[argc][pos] = lpCmdLine[i]; 
        i++; 
        pos++; 
      } 
      argv[argc][pos] = '\0'; 
      argc++; 
      pos = 0; 
    }else{ 
      while (lpCmdLine[i] != ' ' && i < strlen(lpCmdLine)) { 
        argv[argc][pos] = lpCmdLine[i]; 
        i++; 
        pos++; 
      } 
      argv[argc][pos] = '\0'; 
      argc++; 
      pos = 0; 
    } 
  } 
  argv[argc] = 0; 

  // Initialize the processes and start the application. 
  retVal = main(argc, argv); 

  // Delete arguments 
  for(j=0; j<argc; j++){ 
    free(argv[j]); 
  } 
  free(argv); 

  return retVal;; 
} 
#endif 