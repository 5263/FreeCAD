TEMPLATE = lib
DEFINES += NO_KDE2
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  DEFINES += HAVE_CONFIG_H NO_INCLUDE_MOCFILES BaseExport= AppExport= GuiExport= #QT_NO_COMPAT
}
INTERFACES	= DlgActions.ui \
		DlgCmdbars.ui \
		DlgCommands.ui \
		DlgCustomize.ui \
		DlgDocTemplates.ui \
		DlgEditor.ui \
		DlgGeneral.ui \
		DlgMacroExecute.ui \
		DlgMacroRecord.ui \
		DlgOnlineHelp.ui \
		DlgParameter.ui \
		DlgPreferences.ui \
		DlgSettings3DView.ui \
		DlgSettings.ui \
		DlgSettingsMacro.ui \
		DlgToolbars.ui

HEADERS = 	Application.h \
		BitmapFactory.h \
		ButtonGroup.h \
		Command.h \
		CommandLine.h \
		DlgActionsImp.h \
		DlgCmdbarsImp.h \
		DlgCommandsImp.h \
		DlgCustomizeImp.h \
		DlgDocTemplatesImp.h \
		DlgEditorImp.h \
		DlgGeneralImp.h \
		DlgMacroExecuteImp.h \
		DlgMacroRecordImp.h \
		DlgOnlineHelpImp.h \
		DlgParameterImp.h \
		DlgPreferencesImp.h \
		DlgSettings3DViewImp.h \
		DlgSettingsImp.h \
		DlgSettingsMacroImp.h \
		DlgToolbarsImp.h \
		DlgUndoRedo.h \
		Document.h \
		GuiConsole.h \
		HtmlViewP.h \
		HtmlView.h \
		Macro.h \
		MouseModel.h \
		PlatQt.h \
		PreCompiled.h \
		PrefWidgets.h \
		Process.h \
		ProcessQt.h \
		PropertyPage.h \
		PropertyView.h \
		ReportView.h \
		SciEditor.h \
		ScintillaQt.h \
		Splashscreen.h \
		Tools.h \
		Tree.h \
		View3D.h \
		View3DInventor.h \
		View.h \
		WidgetFactory.h \
		Widgets.h \
		Window.h \
		qextmdi/dummykmainwindow.h \
		qextmdi/dummykpartsdockmainwindow.h \
		qextmdi/dummyktoolbar.h \
		qextmdi/exportdockclass.h \
		qextmdi/kdocktabctl.h \
		qextmdi/kdockwidget.h \
		qextmdi/kdockwidget_private.h \
		qextmdi/qextmdichildarea.h \
		qextmdi/qextmdichildfrm.h \
		qextmdi/qextmdichildfrmcaption.h \
		qextmdi/qextmdichildview.h \
		qextmdi/qextmdidefines.h \
		qextmdi/qextmdiiterator.h \
		qextmdi/qextmdilistiterator.h \
		qextmdi/qextmdimainfrm.h \
		qextmdi/qextmdinulliterator.h \
		qextmdi/qextmditaskbar.h \
		Language/LanguageFactory.h \
		scintilla/Accessor.h \
		scintilla/AutoComplete.h \
		scintilla/CallTip.h \
		scintilla/CellBuffer.h \
		scintilla/ContractionState.h \
		scintilla/DocumentAccessor.h \
		scintilla/Documents.h \
		scintilla/Editor.h \
		scintilla/ExternalLexer.h \
		scintilla/Indicator.h \
		scintilla/KeyMap.h \
		scintilla/KeyWords.h \
		scintilla/LineMarker.h \
		scintilla/Platform.h \
		scintilla/PropSet.h \
		scintilla/RESearch.h \
		scintilla/SciLexer.h \
		scintilla/Scintilla.h \
		scintilla/ScintillaBase.h \
		scintilla/ScintillaWidget.h \
		scintilla/SString.h \
		scintilla/Style.h \
		scintilla/StyleContext.h \
		scintilla/SVector.h \
		scintilla/UniConversion.h \
		scintilla/ViewStyle.h \
		scintilla/WindowAccessor.h \
		scintilla/XPM.h \
		Inventor/OCC/SoBrepShape.h \
		Inventor/Qt/devices/6DOFEvents.h \
		Inventor/Qt/devices/SoGuiDeviceP.h \
		Inventor/Qt/devices/SoGuiInputFocus.h \
		Inventor/Qt/devices/SoGuiInputFocusP.h \
		Inventor/Qt/devices/SoGuiKeyboardP.h \
		Inventor/Qt/devices/SoGuiMouseP.h \
		Inventor/Qt/devices/SoGuiSpaceballP.h \
		Inventor/Qt/devices/SoQtDevice.h \
		Inventor/Qt/devices/SoQtDeviceP.h \
		Inventor/Qt/devices/SoQtInputFocus.h \
		Inventor/Qt/devices/SoQtInputFocusP.h \
		Inventor/Qt/devices/SoQtKeyboard.h \
		Inventor/Qt/devices/SoQtMouse.h \
		Inventor/Qt/devices/SoQtSpaceball.h \
		Inventor/Qt/devices/SoQtSpaceballP.h \
		Inventor/Qt/viewers/SoGuiExaminerViewerP.h \
		Inventor/Qt/viewers/SoGuiFullViewerP.h \
		Inventor/Qt/viewers/SoGuiPlaneViewerP.h \
		Inventor/Qt/viewers/SoQtConstrainedViewer.h \
		Inventor/Qt/viewers/SoQtExaminerViewer.h \
		Inventor/Qt/viewers/SoQtExaminerViewerP.h \
		Inventor/Qt/viewers/SoQtFlyViewer.h \
		Inventor/Qt/viewers/SoQtFullViewer.h \
		Inventor/Qt/viewers/SoQtFullViewerP.h \
		Inventor/Qt/viewers/SoQtPlaneViewer.h \
		Inventor/Qt/viewers/SoQtPlaneViewerP.h \
		Inventor/Qt/viewers/SoQtViewer.h \
		Inventor/Qt/widgets/QtNativePopupMenu.h \
		Inventor/Qt/widgets/SoAnyThumbWheel.h \
		Inventor/Qt/widgets/SoQtGLArea.h \
		Inventor/Qt/widgets/SoQtPopupMenu.h \
		Inventor/Qt/widgets/SoQtThumbWheel.h \
		Inventor/Qt/SoAny.h \
		Inventor/Qt/SoGuiComponentP.h \
		Inventor/Qt/SoGuiGLWidgetP.h \
		Inventor/Qt/SoGuiP.h \
		Inventor/Qt/SoQt.h \
		Inventor/Qt/SoQtBasic.h \
		Inventor/Qt/SoQtComponent.h \
		Inventor/Qt/SoQtComponentP.h \
		Inventor/Qt/SoQtCursor.h \
		Inventor/Qt/soqtdefs.h \
		Inventor/Qt/SoQtGLWidget.h \
		Inventor/Qt/SoQtGLWidgetP.h \
		Inventor/Qt/SoQtInternal.h \
		Inventor/Qt/SoQtObject.h \
		Inventor/Qt/SoQtP.h \
		Inventor/Qt/SoQtRenderArea.h

SOURCES = 	Application.cpp \
		BitmapFactory.cpp \
		ButtonGroup.cpp \
		Command.cpp \
		CommandLine.cpp \
		CommandStd.cpp \
		CommandTest.cpp \
		CommandView.cpp \
		DlgActionsImp.cpp \
		DlgCmdbarsImp.cpp \
		DlgCommandsImp.cpp \
		DlgCustomizeImp.cpp \
		DlgDocTemplatesImp.cpp \
		DlgEditorImp.cpp \
		DlgGeneralImp.cpp \
		DlgMacroExecuteImp.cpp \
		DlgMacroRecordImp.cpp \
		DlgOnlineHelpImp.cpp \
		DlgParameterImp.cpp \
		DlgPreferencesImp.cpp \
		DlgSettings3DViewImp.cpp \
		DlgSettingsImp.cpp \
		DlgSettingsMacroImp.cpp \
		DlgToolbarsImp.cpp \
		DlgUndoRedo.cpp \
		Document.cpp \
		GuiConsole.cpp \
		HtmlView.cpp \
		Macro.cpp \
		MouseModel.cpp \
		PlatQt.cpp \
		PreCompiled.cpp \
		PrefWidgets.cpp \
		Process.cpp \
		PropertyPage.cpp \
		PropertyView.cpp \
		ReportView.cpp \
		resource.cpp \
		SciEditor.cpp \
		ScintillaQt.cpp \
		Splashscreen.cpp \
		Tools.cpp \
		Tree.cpp \
		View3D.cpp \
		View3DInventor.cpp \
		View.cpp \
		WidgetFactory.cpp \
		Widgets.cpp \
		Window.cpp \
		qextmdi/dummy_moc.cpp \
		qextmdi/kdocktabctl.cpp \
		qextmdi/kdockwidget.cpp \
		qextmdi/kdockwidget_private.cpp \
		qextmdi/qextmdichildarea.cpp \
		qextmdi/qextmdichildfrm.cpp \
		qextmdi/qextmdichildfrmcaption.cpp \
		qextmdi/qextmdichildview.cpp \
		qextmdi/qextmdimainfrm.cpp \
		qextmdi/qextmditaskbar.cpp \
		Language/LanguageFactory.cpp \
		scintilla/AutoComplete.cpp \
		scintilla/CallTip.cpp \
		scintilla/CellBuffer.cpp \
		scintilla/ContractionState.cpp \
		scintilla/DocumentAccessor.cpp \
		scintilla/Documents.cpp \
		scintilla/Editor.cpp \
		scintilla/ExternalLexer.cpp \
		scintilla/Indicator.cpp \
		scintilla/KeyMap.cpp \
		scintilla/KeyWords.cpp \
		scintilla/LexAda.cpp \
		scintilla/LexAsm.cpp \
		scintilla/LexAVE.cpp \
		scintilla/LexBaan.cpp \
		scintilla/LexBullant.cpp \
		scintilla/LexConf.cpp \
		scintilla/LexCPP.cpp \
		scintilla/LexCrontab.cpp \
		scintilla/LexCSS.cpp \
		scintilla/LexEiffel.cpp \
		scintilla/LexEScript.cpp \
		scintilla/LexFortran.cpp \
		scintilla/LexHTML.cpp \
		scintilla/LexLisp.cpp \
		scintilla/LexLout.cpp \
		scintilla/LexLua.cpp \
		scintilla/LexMatlab.cpp \
		scintilla/LexOthers.cpp \
		scintilla/LexPascal.cpp \
		scintilla/LexPerl.cpp \
		scintilla/LexPOV.cpp \
		scintilla/LexPython.cpp \
		scintilla/LexRuby.cpp \
		scintilla/LexSQL.cpp \
		scintilla/LexVB.cpp \
		scintilla/LineMarker.cpp \
		scintilla/PropSet.cpp \
		scintilla/RESearch.cpp \
		scintilla/ScintillaBase.cpp \
		scintilla/Style.cpp \
		scintilla/StyleContext.cpp \
		scintilla/UniConversion.cpp \
		scintilla/ViewStyle.cpp \
		scintilla/WindowAccessor.cpp \
		scintilla/XPM.cpp \
		Inventor/OCC/SoBrepShape.cpp \
		Inventor/Qt/devices/6DOFEvents.cpp \
		Inventor/Qt/devices/SoGuiDevice.cpp \
		Inventor/Qt/devices/SoQtDevice.cpp \
		Inventor/Qt/devices/SoQtInputFocus.cpp \
		Inventor/Qt/devices/SoQtInputFocus1.cpp \
		Inventor/Qt/devices/SoQtKeyboard.cpp \
		Inventor/Qt/devices/SoQtKeyboard1.cpp \
		Inventor/Qt/devices/SoQtMouse.cpp \
		Inventor/Qt/devices/SoQtMouse1.cpp \
		Inventor/Qt/devices/SoQtSpaceball.cpp \
		Inventor/Qt/devices/SoQtSpaceball1.cpp \
		Inventor/Qt/viewers/ExaminerViewer.cpp \
		Inventor/Qt/viewers/FullViewer.cpp \
		Inventor/Qt/viewers/PlaneViewer.cpp \
		Inventor/Qt/viewers/SoQtConstrainedViewer.cpp \
		Inventor/Qt/viewers/SoQtExaminerViewer.cpp \
		Inventor/Qt/viewers/SoQtFlyViewer.cpp \
		Inventor/Qt/viewers/SoQtFullViewer.cpp \
		Inventor/Qt/viewers/SoQtPlaneViewer.cpp \
		Inventor/Qt/viewers/SoQtViewer.cpp \
		Inventor/Qt/widgets/QtNativePopupMenu.cpp \
		Inventor/Qt/widgets/SoAnyThumbWheel.cpp \
		Inventor/Qt/widgets/SoQtGLArea.cpp \
		Inventor/Qt/widgets/SoQtPopupMenu.cpp \
		Inventor/Qt/widgets/SoQtThumbWheel.cpp \
		Inventor/Qt/SoAny.cpp \
		Inventor/Qt/SoGuiGLWidgetCommon.cpp \
		Inventor/Qt/SoQt.cpp \
		Inventor/Qt/SoQtCommon.cpp \
		Inventor/Qt/SoQtComponent.cpp \
		Inventor/Qt/SoQtComponentCommon.cpp \
		Inventor/Qt/SoQtCursor.cpp \
		Inventor/Qt/SoQtGLWidget.cpp \
		Inventor/Qt/SoQtObject.cpp \
		Inventor/Qt/SoQtRenderArea.cpp

TRANSLATIONS	= Language/FreeCAD_fr.ts \
		  Language/FreeCAD_de.ts


CONFIG = warn_off qt thread 

unix {
LIBS = -L/usr/local/lib  -L/opt/OCC/Cas5.1/ros/Linux/lib -L../../bin\
		-lqui\
		-lTKernel \
		-lTKMath \
		-lTKService \
		-lTKGeomAlgo \
		-lTKGeomBase \
		-lTKG2d \
		-lTKG3d \
		-lTKBRep \
		-lTKTopAlgo \
		-lTKPrim \
		-lTKV2d \
		-lTKV3d \
		-lTKCAF \
		-lTKCDF \
		-lCoin \
		-lFreeCADBase \
		-lFreeCADApp
}

unix {
INCLUDEPATH += 	/usr/local/include/python2.3 \
				/opt/OCC/Cas5.1/ros/inc \
				scintilla qextmdi Inventor Inventor/Qt $$UI_DIR $$MOC_DIR ./..
}

DESTDIR = ../../bin
