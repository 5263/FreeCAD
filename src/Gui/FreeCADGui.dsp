# Microsoft Developer Studio Project File - Name="FreeCADGui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FreeCADGui - Win32 Debug
!MESSAGE Dies ist kein g�ltiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und f�hren Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "FreeCADGui.mak".
!MESSAGE 
!MESSAGE Sie k�nnen beim Ausf�hren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "FreeCADGui.mak" CFG="FreeCADGui - Win32 Debug"
!MESSAGE 
!MESSAGE F�r die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "FreeCADGui - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FreeCADGui - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "FreeCADGui___Win32_Release"
# PROP BASE Intermediate_Dir "FreeCADGui___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FREECADGUI_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /O2 /I "." /I "inventor/Qt" /D "NDEBUG" /D "_USRDLL" /D "FCGui" /D "SCI_LEXER" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../bin/FreeCADGui.dll"

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "FreeCADGui___Win32_Debug"
# PROP BASE Intermediate_Dir "FreeCADGui___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FREECADGUI_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GR /GX /ZI /Od /I "." /I "inventor/Qt" /D "_USRDLL" /D "FCGui" /D "FC_DEBUG" /D "SCI_LEXER" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../bin/FreeCADGuiD.dll"

!ENDIF 

# Begin Target

# Name "FreeCADGui - Win32 Release"
# Name "FreeCADGui - Win32 Debug"
# Begin Group "Dialog"

# PROP Default_Filter ""
# Begin Group "Settings"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgEditor.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgEditor.ui
InputName=DlgEditor

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgEditor.ui
InputName=DlgEditor

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgEditorImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgEditorImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgEditorImp.h
InputName=DlgEditorImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgEditorImp.h
InputName=DlgEditorImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgGeneral.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgGeneral.ui
InputName=DlgGeneral

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgGeneral.ui
InputName=DlgGeneral

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgGeneralImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgGeneralImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgGeneralImp.h
InputName=DlgGeneralImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgGeneralImp.h
InputName=DlgGeneralImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgPreferences.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgPreferences.ui
InputName=DlgPreferences

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgPreferences.ui
InputName=DlgPreferences

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgPreferencesImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgPreferencesImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgPreferencesImp.h
InputName=DlgPreferencesImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgPreferencesImp.h
InputName=DlgPreferencesImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgSettings.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgSettings.ui
InputName=DlgSettings

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgSettings.ui
InputName=DlgSettings

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgSettings3DView.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgSettings3DView.ui
InputName=DlgSettings3DView

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgSettings3DView.ui
InputName=DlgSettings3DView

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgSettings3DViewImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgSettings3DViewImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgSettings3DViewImp.h
InputName=DlgSettings3DViewImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgSettings3DViewImp.h
InputName=DlgSettings3DViewImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgSettingsImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgSettingsImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgSettingsImp.h
InputName=DlgSettingsImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgSettingsImp.h
InputName=DlgSettingsImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgSettingsMacro.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgSettingsMacro.ui
InputName=DlgSettingsMacro

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgSettingsMacro.ui
InputName=DlgSettingsMacro

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgSettingsMacroImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgSettingsMacroImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgSettingsMacroImp.h
InputName=DlgSettingsMacroImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgSettingsMacroImp.h
InputName=DlgSettingsMacroImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Customize"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgActions.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgActions.ui
InputName=DlgActions

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgActions.ui
InputName=DlgActions

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgActionsImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgActionsImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgActionsImp.h
InputName=DlgActionsImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgActionsImp.h
InputName=DlgActionsImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgCmdbars.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgCmdbars.ui
InputName=DlgCmdbars

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgCmdbars.ui
InputName=DlgCmdbars

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgCmdbarsImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgCmdbarsImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgCmdbarsImp.h
InputName=DlgCmdbarsImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgCmdbarsImp.h
InputName=DlgCmdbarsImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgCommands.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgCommands.ui
InputName=DlgCommands

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgCommands.ui
InputName=DlgCommands

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgCommandsImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgCommandsImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgCommandsImp.h
InputName=DlgCommandsImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgCommandsImp.h
InputName=DlgCommandsImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgCustomizeImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgCustomizeImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgCustomizeImp.h
InputName=DlgCustomizeImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgCustomizeImp.h
InputName=DlgCustomizeImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgToolbars.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgToolbars.ui
InputName=DlgToolbars

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgToolbars.ui
InputName=DlgToolbars

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgToolbarsImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgToolbarsImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgToolbarsImp.h
InputName=DlgToolbarsImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgToolbarsImp.h
InputName=DlgToolbarsImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\DlgDocTemplates.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgDocTemplates.ui
InputName=DlgDocTemplates

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgDocTemplates.ui
InputName=DlgDocTemplates

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgDocTemplatesImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgDocTemplatesImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgDocTemplatesImp.h
InputName=DlgDocTemplatesImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgDocTemplatesImp.h
InputName=DlgDocTemplatesImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgMacroExecute.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgMacroExecute.ui
InputName=DlgMacroExecute

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgMacroExecute.ui
InputName=DlgMacroExecute

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgMacroExecuteImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgMacroExecuteImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgMacroExecuteImp.h
InputName=DlgMacroExecuteImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgMacroExecuteImp.h
InputName=DlgMacroExecuteImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgMacroRecord.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgMacroRecord.ui
InputName=DlgMacroRecord

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgMacroRecord.ui
InputName=DlgMacroRecord

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgMacroRecordImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgMacroRecordImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgMacroRecordImp.h
InputName=DlgMacroRecordImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgMacroRecordImp.h
InputName=DlgMacroRecordImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgParameter.ui

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgParameter.ui
InputName=DlgParameter

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DlgParameter.ui
InputName=DlgParameter

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgParameterImp.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgParameterImp.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgParameterImp.h
InputName=DlgParameterImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgParameterImp.h
InputName=DlgParameterImp

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgUndoRedo.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\DlgUndoRedo.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgUndoRedo.h
InputName=DlgUndoRedo

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\DlgUndoRedo.h
InputName=DlgUndoRedo

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Command"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Command.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\Command.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Command.h
InputName=Command

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Command.h
InputName=Command

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CommandStd.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\CommandTest.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\CommandView.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# End Group
# Begin Group "Dock windows"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ButtonGroup.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\ButtonGroup.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\ButtonGroup.h
InputName=ButtonGroup

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\ButtonGroup.h
InputName=ButtonGroup

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HtmlView.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\HtmlView.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\HtmlView.h
InputName=HtmlView

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\HtmlView.h
InputName=HtmlView

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Scintilla"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\scintilla\AutoComplete.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\AutoComplete.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\CallTip.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\CallTip.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\CellBuffer.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\CellBuffer.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\ContractionState.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\ContractionState.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\DocumentAccessor.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\DocumentAccessor.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\Documents.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\Documents.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\Editor.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\Editor.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\ExternalLexer.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\ExternalLexer.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\Indicator.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\Indicator.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\KeyMap.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\KeyMap.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\KeyWords.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexAda.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexAsm.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexAVE.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexBaan.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexBullant.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexConf.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexCPP.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexCrontab.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexCSS.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexEiffel.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexEScript.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexFortran.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexHTML.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexLisp.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexLout.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexLua.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexMatlab.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexOthers.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexPascal.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexPerl.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexPOV.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexPython.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexRuby.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexSQL.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LexVB.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LineMarker.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\LineMarker.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\PropSet.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\RESearch.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\RESearch.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\ScintillaBase.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\ScintillaBase.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\Style.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\Style.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\StyleContext.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\StyleContext.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\SVector.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\UniConversion.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\UniConversion.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\ViewStyle.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\ViewStyle.h
# End Source File
# Begin Source File

SOURCE=.\scintilla\WindowAccessor.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\XPM.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\scintilla\XPM.h
# End Source File
# End Group
# Begin Group "qextmdi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\qextmdi\dummy_moc.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\qextmdi\dummykmainwindow.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\qextmdi
InputPath=.\qextmdi\dummykmainwindow.h
InputName=dummykmainwindow

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\qextmdi
InputPath=.\qextmdi\dummykmainwindow.h
InputName=dummykmainwindow

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qextmdi\dummykpartsdockmainwindow.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\qextmdi
InputPath=.\qextmdi\dummykpartsdockmainwindow.h
InputName=dummykpartsdockmainwindow

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\qextmdi
InputPath=.\qextmdi\dummykpartsdockmainwindow.h
InputName=dummykpartsdockmainwindow

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qextmdi\dummyktoolbar.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\qextmdi
InputPath=.\qextmdi\dummyktoolbar.h
InputName=dummyktoolbar

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\qextmdi
InputPath=.\qextmdi\dummyktoolbar.h
InputName=dummyktoolbar

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qextmdi\exportdockclass.h
# End Source File
# Begin Source File

SOURCE=.\qextmdi\kdocktabctl.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\qextmdi\kdocktabctl.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\kdocktabctl.h
InputName=kdocktabctl

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\kdocktabctl.h
InputName=kdocktabctl

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qextmdi\kdockwidget.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\qextmdi\kdockwidget.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\kdockwidget.h
InputName=kdockwidget

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\kdockwidget.h
InputName=kdockwidget

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qextmdi\kdockwidget_private.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\qextmdi\kdockwidget_private.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\kdockwidget_private.h
InputName=kdockwidget_private

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\kdockwidget_private.h
InputName=kdockwidget_private

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdichildarea.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdichildarea.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\qextmdichildarea.h
InputName=qextmdichildarea

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\qextmdichildarea.h
InputName=qextmdichildarea

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdichildfrm.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdichildfrm.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\qextmdichildfrm.h
InputName=qextmdichildfrm

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\qextmdichildfrm.h
InputName=qextmdichildfrm

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdichildfrmcaption.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdichildfrmcaption.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\qextmdichildfrmcaption.h
InputName=qextmdichildfrmcaption

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\qextmdichildfrmcaption.h
InputName=qextmdichildfrmcaption

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdichildview.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdichildview.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\qextmdichildview.h
InputName=qextmdichildview

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\qextmdichildview.h
InputName=qextmdichildview

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdidefines.h
# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdiiterator.h
# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdilistiterator.h
# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdimainfrm.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdimainfrm.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\qextmdimainfrm.h
InputName=qextmdimainfrm

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc 
	python ..\Tools\post_moc.py $(InputDir)\$(InputName).moc 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\qextmdimainfrm.h
InputName=qextmdimainfrm

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc 
	python ..\Tools\post_moc.py $(InputDir)\$(InputName).moc 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmdinulliterator.h
# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmditaskbar.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\qextmdi\qextmditaskbar.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\qextmditaskbar.h
InputName=qextmditaskbar

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Mocing $(InputPath)
InputDir=.\qextmdi
InputPath=.\qextmdi\qextmditaskbar.h
InputName=qextmditaskbar

"$(InputDir)\$(InputName).moc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\$(InputName).moc

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Inventor"

# PROP Default_Filter ""
# Begin Group "Devices"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inventor\Qt\devices\6DOFEvents.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\6DOFEvents.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoGuiDevice.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoGuiDeviceP.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoGuiInputFocus.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoGuiKeyboardP.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoGuiMouseP.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoGuiSpaceballP.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtDevice.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtDevice.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtDeviceP.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtInputFocus.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtInputFocus.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtInputFocus1.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtKeyboard.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtKeyboard.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtKeyboard1.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtMouse.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtMouse.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtMouse1.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtSpaceball.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtSpaceball.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtSpaceball1.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\devices\SoQtSpaceballP.h
# End Source File
# End Group
# Begin Group "Viewers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\ExaminerViewer.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\FullViewer.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\PlaneViewer.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoGuiExaminerViewerP.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoGuiFullViewerP.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoGuiPlaneViewerP.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtConstrainedViewer.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtConstrainedViewer.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtExaminerViewer.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtExaminerViewer.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtExaminerViewerP.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\viewers
InputPath=.\Inventor\Qt\viewers\SoQtExaminerViewerP.h
InputName=SoQtExaminerViewerP

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\viewers
InputPath=.\Inventor\Qt\viewers\SoQtExaminerViewerP.h
InputName=SoQtExaminerViewerP

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtFlyViewer.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtFlyViewer.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtFullViewer.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtFullViewer.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtFullViewerP.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\viewers
InputPath=.\Inventor\Qt\viewers\SoQtFullViewerP.h
InputName=SoQtFullViewerP

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\viewers
InputPath=.\Inventor\Qt\viewers\SoQtFullViewerP.h
InputName=SoQtFullViewerP

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtPlaneViewer.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtPlaneViewer.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtPlaneViewerP.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\viewers
InputPath=.\Inventor\Qt\viewers\SoQtPlaneViewerP.h
InputName=SoQtPlaneViewerP

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\viewers
InputPath=.\Inventor\Qt\viewers\SoQtPlaneViewerP.h
InputName=SoQtPlaneViewerP

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtViewer.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\viewers\SoQtViewer.h
# End Source File
# End Group
# Begin Group "Widgets"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inventor\Qt\widgets\QtNativePopupMenu.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\widgets\QtNativePopupMenu.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\widgets
InputPath=.\Inventor\Qt\widgets\QtNativePopupMenu.h
InputName=QtNativePopupMenu

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\widgets
InputPath=.\Inventor\Qt\widgets\QtNativePopupMenu.h
InputName=QtNativePopupMenu

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\widgets\SoAnyThumbWheel.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\widgets\SoAnyThumbWheel.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\widgets\SoQtGLArea.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\widgets\SoQtGLArea.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\widgets
InputPath=.\Inventor\Qt\widgets\SoQtGLArea.h
InputName=SoQtGLArea

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\widgets
InputPath=.\Inventor\Qt\widgets\SoQtGLArea.h
InputName=SoQtGLArea

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\widgets\SoQtPopupMenu.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\widgets\SoQtPopupMenu.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\widgets
InputPath=.\Inventor\Qt\widgets\SoQtPopupMenu.h
InputName=SoQtPopupMenu

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\widgets
InputPath=.\Inventor\Qt\widgets\SoQtPopupMenu.h
InputName=SoQtPopupMenu

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\widgets\SoQtThumbWheel.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\widgets\SoQtThumbWheel.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\widgets
InputPath=.\Inventor\Qt\widgets\SoQtThumbWheel.h
InputName=SoQtThumbWheel

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt\widgets
InputPath=.\Inventor\Qt\widgets\SoQtThumbWheel.h
InputName=SoQtThumbWheel

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "OCC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inventor\OCC\SoBrepShape.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\Inventor\OCC\SoBrepShape.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Inventor\Qt\SoAny.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoAny.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoGuiComponentP.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoGuiGLWidgetCommon.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoGuiP.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQt.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQt.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtCommon.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtComponent.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtComponent.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtComponentCommon.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtComponentP.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt
InputPath=.\Inventor\Qt\SoQtComponentP.h
InputName=SoQtComponentP

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt
InputPath=.\Inventor\Qt\SoQtComponentP.h
InputName=SoQtComponentP

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtCursor.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtCursor.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtGLWidget.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtGLWidget.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtGLWidgetP.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt
InputPath=.\Inventor\Qt\SoQtGLWidgetP.h
InputName=SoQtGLWidgetP

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt
InputPath=.\Inventor\Qt\SoQtGLWidgetP.h
InputName=SoQtGLWidgetP

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtObject.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtObject.h
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtP.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt
InputPath=.\Inventor\Qt\SoQtP.h
InputName=SoQtP

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Inventor\Qt
InputPath=.\Inventor\Qt\SoQtP.h
InputName=SoQtP

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtRenderArea.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Inventor\Qt\SoQtRenderArea.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Application.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\Application.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Application.h
InputName=Application

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Application.h
InputName=Application

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CommandLine.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\CommandLine.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\CommandLine.h
InputName=CommandLine

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\CommandLine.h
InputName=CommandLine

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DllMain.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Document.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\Document.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Document.h
InputName=Document

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Document.h
InputName=Document

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GuiConsole.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\GuiConsole.h
# End Source File
# Begin Source File

SOURCE=.\Libs.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Macro.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\Macro.h
# End Source File
# Begin Source File

SOURCE=.\MouseModel.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\MouseModel.h
# End Source File
# Begin Source File

SOURCE=.\PlatQt.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\PlatQt.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\PlatQt.h
InputName=PlatQt

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\PlatQt.h
InputName=PlatQt

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PreCompiled.cpp
# ADD CPP /Yc"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\PreCompiled.h
# End Source File
# Begin Source File

SOURCE=.\PrefWidgets.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\PrefWidgets.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\PrefWidgets.h
InputName=PrefWidgets

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\PrefWidgets.h
InputName=PrefWidgets

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PropertyView.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\PropertyView.h
# End Source File
# Begin Source File

SOURCE=.\ScintillaQt.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\ScintillaQt.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\ScintillaQt.h
InputName=ScintillaQt

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\ScintillaQt.h
InputName=ScintillaQt

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Splashscreen.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\Splashscreen.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Splashscreen.h
InputName=Splashscreen

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Splashscreen.h
InputName=Splashscreen

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Themes.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\Themes.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Themes.h
InputName=Themes

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Themes.h
InputName=Themes

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tools.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\Tools.h
# End Source File
# Begin Source File

SOURCE=.\Tree.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\Tree.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Tree.h
InputName=Tree

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Tree.h
InputName=Tree

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\View.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\View.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\View.h
InputName=View

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\View.h
InputName=View

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\View3D.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\View3D.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\View3D.h
InputName=View3D

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\View3D.h
InputName=View3D

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WidgetFactory.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\WidgetFactory.h
# End Source File
# Begin Source File

SOURCE=.\Widgets.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\Widgets.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Widgets.h
InputName=Widgets

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Widgets.h
InputName=Widgets

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Window.cpp
# ADD CPP /Yu"PreCompiled.h"
# End Source File
# Begin Source File

SOURCE=.\Window.h

!IF  "$(CFG)" == "FreeCADGui - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Window.h
InputName=Window

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeCADGui - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\Window.h
InputName=Window

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
