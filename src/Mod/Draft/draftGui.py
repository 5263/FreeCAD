
#***************************************************************************
#*   FreeCAD Draft Workbench                                               *
#*   Author: Yorik van Havre - http://yorik.orgfree.com                    *
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License (GPL)            *
#*   as published by the Free Software Foundation; either version 2 of     *
#*   the License, or (at your option) any later version.                   *
#*   for detail see the LICENCE text file.                                 *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU Library General Public License for more details.                  *
#*                                                                         *
#*   You should have received a copy of the GNU Library General Public     *
#*   License along with this program; if not, write to the Free Software   *
#*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
#*   USA                                                                   *
#*                                                                         *
#***************************************************************************

'''
This is the GUI part of the Draft module.
Report to Draft.py for info
'''

from PyQt4 import QtCore,QtGui
import FreeCAD, os

#---------------------------------------------------------------------------
# Customized widgets
#---------------------------------------------------------------------------

class DraftDockWidget(QtGui.QDockWidget):
	"custom QDockWidget that emits a resized() signal when resized"
	def __init__(self,parent = None):
		QtGui.QDockWidget.__init__(self,parent)
	def resizeEvent(self,event):
		self.emit(QtCore.SIGNAL("resized()"))

class DraftLineEdit(QtGui.QLineEdit):
	"custom QLineEdit widget that has the power to catch Escape keypress"
	def __init__(self, parent = None):
		QtGui.QLineEdit.__init__(self, parent)
	def keyPressEvent(self, event):
		if event.key() == QtCore.Qt.Key_Escape:
			self.emit(QtCore.SIGNAL("escaped()"))
		elif (event.key() == QtCore.Qt.Key_Z) and QtCore.Qt.ControlModifier:
			self.emit(QtCore.SIGNAL("undo()"))
		else:
			QtGui.QLineEdit.keyPressEvent(self, event)

class toolBar:
	"main draft Toolbar"
	def __init__(self):
		class Ui_draftToolbar(object):

#---------------------------------------------------------------------------
# General UI setup
#---------------------------------------------------------------------------

			def setupUi(self, draftToolbar):
				icondir = self.findicons()
				draftToolbar.setObjectName("draftToolbar")
				draftToolbar.resize(QtCore.QSize(QtCore.QRect(0,0,800,32).size()).expandedTo(draftToolbar.minimumSizeHint()))
				self.draftToolbar = draftToolbar

				self.cmdlabel = QtGui.QLabel(draftToolbar)
				self.cmdlabel.setGeometry(QtCore.QRect(110,4,111,18))
				self.cmdlabel.setObjectName("cmdlabel")
				boldtxt = QtGui.QFont()
				boldtxt.setWeight(75)
				boldtxt.setBold(True)
				self.cmdlabel.setFont(boldtxt)

				self.labelx = QtGui.QLabel(draftToolbar)
				self.labelx.setGeometry(QtCore.QRect(150,4,60,18))
				self.labelx.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
				self.labelx.setObjectName("labelx")
				self.labelx.setText("X")
				self.labelx.hide()

				self.xValue = DraftLineEdit(draftToolbar)
				self.xValue.setGeometry(QtCore.QRect(220,4,70,18))
				self.xValue.setObjectName("xValue")
				self.xValue.setText("0.00")
				self.xValue.hide()

				self.labely = QtGui.QLabel(draftToolbar)
				self.labely.setGeometry(QtCore.QRect(300,4,30,18))
				self.labely.setObjectName("labely")
				self.labely.setText("Y")
				self.labely.hide()

				self.yValue = DraftLineEdit(draftToolbar)
				self.yValue.setGeometry(QtCore.QRect(320,4,70,18))
				self.yValue.setObjectName("yValue")
				self.yValue.setText("0.00")
				self.yValue.hide()

				self.labelz = QtGui.QLabel(draftToolbar)
				self.labelz.setGeometry(QtCore.QRect(400,4,30,18))
				self.labelz.setObjectName("labelz")
				self.labelz.setText("Z")
				self.labelz.hide()

				self.zValue = DraftLineEdit(draftToolbar)
				self.zValue.setGeometry(QtCore.QRect(420,4,70,18))
				self.zValue.setObjectName("zValue")
				self.zValue.setText("0.00")
				self.zValue.setEnabled(False)
				self.zValue.hide()

				self.lockButton = QtGui.QPushButton(draftToolbar)
				self.lockButton.setGeometry(QtCore.QRect(500,3,20,20))
				self.lockButton.setIcon(QtGui.QIcon(icondir + "close.png"))
				self.lockButton.setObjectName("lockButton")
				self.lockButton.setCheckable(True)
				self.lockButton.setChecked(True)
				self.lockButton.hide()

				self.isRelative = QtGui.QCheckBox(draftToolbar)
				self.isRelative.setGeometry(QtCore.QRect(530,6,91,18))
				self.isRelative.setChecked(True)
				self.isRelative.setObjectName("isRelative")
				self.isRelative.hide()

				self.undoButton = QtGui.QPushButton(draftToolbar)
				self.undoButton.setGeometry(QtCore.QRect(610,3,60,20))
				self.undoButton.setIcon(QtGui.QIcon(icondir + "undo.png"))
				self.undoButton.setObjectName("undoButton")
				self.undoButton.hide()

				self.finishButton = QtGui.QPushButton(draftToolbar)
				self.finishButton.setGeometry(QtCore.QRect(670,3,80,20))
				self.finishButton.setIcon(QtGui.QIcon(icondir + "finish.png"))
				self.finishButton.setObjectName("finishButton")
				self.finishButton.hide()

				self.closeButton = QtGui.QPushButton(draftToolbar)
				self.closeButton.setGeometry(QtCore.QRect(750,3,60,20))
				self.closeButton.setIcon(QtGui.QIcon(icondir + "close.png"))
				self.closeButton.setObjectName("closeButton")
				self.closeButton.hide()

				self.labelRadius = QtGui.QLabel(draftToolbar)
				self.labelRadius.setGeometry(QtCore.QRect(200,4,75,18))
				self.labelRadius.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
				self.labelRadius.setObjectName("labelRadius")
				self.labelRadius.hide()

				self.radiusValue = DraftLineEdit(draftToolbar)
				self.radiusValue.setGeometry(QtCore.QRect(280,4,70,18))
				self.radiusValue.setObjectName("radiusValue")
				self.radiusValue.setText("0.00")
				self.radiusValue.hide()

				self.labelText = QtGui.QLabel(draftToolbar)
				self.labelText.setGeometry(QtCore.QRect(200,4,75,18))
				self.labelText.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
				self.labelText.setObjectName("labelText")
				self.labelText.hide()

				self.textValue = DraftLineEdit(draftToolbar)
				self.textValue.setGeometry(QtCore.QRect(280,4,480,18))
				self.textValue.setObjectName("textValue")
				self.textValue.hide()
				
				self.colorButton = QtGui.QPushButton(draftToolbar)
				self.colorButton.setGeometry(QtCore.QRect(790,2,30,22))
				self.colorButton.setObjectName("colorButton")
				self.color = QtGui.QColor(0,0,0)
				self.colorPix = QtGui.QPixmap(16,16)
				self.colorPix.fill(self.color)
				self.colorButton.setIcon(QtGui.QIcon(self.colorPix))

				self.widthButton = QtGui.QSpinBox(draftToolbar)
				self.widthButton.setGeometry(QtCore.QRect(830,2,50,22))
				self.widthButton.setObjectName("widthButton")
				self.widthButton.setValue(2)

				self.isCopy = QtGui.QCheckBox(draftToolbar)
				self.isCopy.setGeometry(QtCore.QRect(600,6,91,18))
				self.isCopy.setChecked(False)
				self.isCopy.setObjectName("isCopy")
				self.isCopy.hide()

				self.sourceCmd=None

				self.retranslateUi(draftToolbar)

				QtCore.QObject.connect(self.xValue,QtCore.SIGNAL("returnPressed()"),self.checkx)
				QtCore.QObject.connect(self.yValue,QtCore.SIGNAL("returnPressed()"),self.checky)

				QtCore.QObject.connect(self.xValue,QtCore.SIGNAL("textEdited(QString)"),self.checkSpecialChars)
				QtCore.QObject.connect(self.yValue,QtCore.SIGNAL("textEdited(QString)"),self.checkSpecialChars)
				QtCore.QObject.connect(self.zValue,QtCore.SIGNAL("textEdited(QString)"),self.checkSpecialChars)
				QtCore.QObject.connect(self.radiusValue,QtCore.SIGNAL("textEdited(QString)"),self.checkSpecialChars)
				QtCore.QObject.connect(self.zValue,QtCore.SIGNAL("returnPressed()"),self.validatePoint)
				QtCore.QObject.connect(self.radiusValue,QtCore.SIGNAL("returnPressed()"),self.validatePoint)
				QtCore.QObject.connect(self.textValue,QtCore.SIGNAL("returnPressed()"),self.sendText)
				QtCore.QObject.connect(self.textValue,QtCore.SIGNAL("escaped()"),self.finish)
				QtCore.QObject.connect(self.zValue,QtCore.SIGNAL("returnPressed()"),self.xValue.setFocus)
				QtCore.QObject.connect(self.zValue,QtCore.SIGNAL("returnPressed()"),self.xValue.selectAll)

				QtCore.QObject.connect(self.finishButton,QtCore.SIGNAL("pressed()"),self.finish)
				QtCore.QObject.connect(self.closeButton,QtCore.SIGNAL("pressed()"),self.closeLine)
				QtCore.QObject.connect(self.undoButton,QtCore.SIGNAL("pressed()"),self.undoSegment)

				QtCore.QObject.connect(self.xValue,QtCore.SIGNAL("escaped()"),self.finish)
				QtCore.QObject.connect(self.xValue,QtCore.SIGNAL("undo()"),self.undoSegment)
				QtCore.QObject.connect(self.yValue,QtCore.SIGNAL("escaped()"),self.finish)
				QtCore.QObject.connect(self.yValue,QtCore.SIGNAL("undo()"),self.undoSegment)
				QtCore.QObject.connect(self.zValue,QtCore.SIGNAL("escaped()"),self.finish)
				QtCore.QObject.connect(self.zValue,QtCore.SIGNAL("undo()"),self.undoSegment)
				QtCore.QObject.connect(self.radiusValue,QtCore.SIGNAL("escaped()"),self.finish)

				QtCore.QObject.connect(self.draftToolbar,QtCore.SIGNAL("resized()"),self.relocate)
				QtCore.QObject.connect(self.colorButton,QtCore.SIGNAL("pressed()"),self.getcol)

				QtCore.QObject.connect(self.lockButton,QtCore.SIGNAL("toggled(bool)"),self.lockz)

				QtCore.QMetaObject.connectSlotsByName(draftToolbar)

			def retranslateUi(self, draftToolbar):
				self.cmdlabel.setText(QtGui.QApplication.translate("draftToolbar", "None", None, QtGui.QApplication.UnicodeUTF8))
				self.cmdlabel.setToolTip(QtGui.QApplication.translate("draftToolbar", "Active Draft command", None, QtGui.QApplication.UnicodeUTF8))
				self.xValue.setToolTip(QtGui.QApplication.translate("draftToolbar", "X coordinate of next point", None, QtGui.QApplication.UnicodeUTF8))
				self.yValue.setToolTip(QtGui.QApplication.translate("draftToolbar", "Y coordinate of next point", None, QtGui.QApplication.UnicodeUTF8))
				self.zValue.setToolTip(QtGui.QApplication.translate("draftToolbar", "Z coordinate of next point", None, QtGui.QApplication.UnicodeUTF8))
				self.labelRadius.setText(QtGui.QApplication.translate("draftToolbar", "Radius", None, QtGui.QApplication.UnicodeUTF8))
				self.radiusValue.setToolTip(QtGui.QApplication.translate("draftToolbar", "Radius of Circle", None, QtGui.QApplication.UnicodeUTF8))
				self.labelText.setText(QtGui.QApplication.translate("draftToolbar", "Text", None, QtGui.QApplication.UnicodeUTF8))
				self.isRelative.setText(QtGui.QApplication.translate("draftToolbar", "Relative", None, QtGui.QApplication.UnicodeUTF8))
				self.lockButton.setToolTip(QtGui.QApplication.translate("draftToolbar", "locks the Z coordinate", None, QtGui.QApplication.UnicodeUTF8))
				self.isRelative.setToolTip(QtGui.QApplication.translate("draftToolbar", "Coordinates relative to last point or absolute (SPACE)", None, QtGui.QApplication.UnicodeUTF8))
				self.finishButton.setText(QtGui.QApplication.translate("draftToolbar", "Finish", None, QtGui.QApplication.UnicodeUTF8))
				self.finishButton.setToolTip(QtGui.QApplication.translate("draftToolbar", "Finishes the current line without closing (F)", None, QtGui.QApplication.UnicodeUTF8))
				self.undoButton.setText(QtGui.QApplication.translate("draftToolbar", "Undo", None, QtGui.QApplication.UnicodeUTF8))
				self.undoButton.setToolTip(QtGui.QApplication.translate("draftToolbar", "Undo the last segment (CTRL+Z)", None, QtGui.QApplication.UnicodeUTF8))
				self.closeButton.setText(QtGui.QApplication.translate("draftToolbar", "Close", None, QtGui.QApplication.UnicodeUTF8))
				self.closeButton.setToolTip(QtGui.QApplication.translate("draftToolbar", "Finishes and closes the current line (C)", None, QtGui.QApplication.UnicodeUTF8))
				self.widthButton.setSuffix(QtGui.QApplication.translate("draftToolbar", "px", None, QtGui.QApplication.UnicodeUTF8))
				self.isCopy.setText(QtGui.QApplication.translate("draftToolbar", "Copy", None, QtGui.QApplication.UnicodeUTF8))
				self.isCopy.setToolTip(QtGui.QApplication.translate("draftToolbar", "If checked, objects will be copied instead of moved (C)", None, QtGui.QApplication.UnicodeUTF8))
				self.colorButton.setToolTip(QtGui.QApplication.translate("draftToolbar", "Current line color for new objects", None, QtGui.QApplication.UnicodeUTF8))
				self.widthButton.setToolTip(QtGui.QApplication.translate("draftToolbar", "Current line width for new objects", None, QtGui.QApplication.UnicodeUTF8))

#---------------------------------------------------------------------------
# Interface modes
#---------------------------------------------------------------------------

			def lineUi(self):
				self.cmdlabel.setText("Line")
				self.isRelative.show()
				self.finishButton.show()
				self.closeButton.show()
				self.undoButton.show()
				self.pointUi()

			def circleUi(self):
				self.cmdlabel.setText("Circle")
				self.pointUi()
				self.labelx.setText("Center X")

			def arcUi(self):
				self.cmdlabel.setText("Arc")
				self.labelx.setText("Center X")
				self.pointUi()

			def pointUi(self):
				self.xValue.setEnabled(True)
				self.yValue.setEnabled(True)
				self.labelx.show()
				self.labely.show()
				self.labelz.show()
				self.xValue.show()
				self.yValue.show()
				self.zValue.show()
				self.lockButton.show()
				self.xValue.setFocus()
				self.xValue.selectAll()

			def offUi(self):
				self.cmdlabel.setText("None")
				self.labelx.setText("X")
				self.labelx.hide()
				self.labely.hide()
				self.labelz.hide()
				self.xValue.hide()
				self.yValue.hide()
				self.zValue.hide()
				self.isRelative.hide()
				self.finishButton.hide()
				self.undoButton.hide()
				self.closeButton.hide()
				self.labelRadius.hide()
				self.radiusValue.hide()
				self.isCopy.hide()
				self.lockButton.hide()
				self.labelText.hide()
				self.textValue.hide()

			def radiusUi(self):
				self.labelx.hide()
				self.labely.hide()
				self.labelz.hide()
				self.xValue.hide()
				self.yValue.hide()
				self.zValue.hide()
				self.lockButton.hide()
				self.labelRadius.setText("Radius")
				self.labelRadius.show()
				self.radiusValue.show()

			def textUi(self):
				self.labelx.hide()
				self.labely.hide()
				self.labelz.hide()
				self.xValue.hide()
				self.yValue.hide()
				self.zValue.hide()
				self.lockButton.hide()
				self.labelText.show()
				self.textValue.show()
				self.textValue.setText('')
				self.textValue.setFocus()

			def relocate(self):
				"relocates the right-aligned buttons depending on the toolbar size"
				w=self.draftToolbar.geometry().width()
				self.widthButton.setGeometry(QtCore.QRect(w-100,2,50,22))
				self.colorButton.setGeometry(QtCore.QRect(w-140,2,30,22))

			def lockz(self,checked):
					self.zValue.setEnabled(not checked)


#---------------------------------------------------------------------------
# Check functions
#---------------------------------------------------------------------------

			def findicons(self):
				path1 = FreeCAD.ConfigGet("AppHomePath") + "Mod/Draft/icons/"
				path2 = FreeCAD.ConfigGet("UserAppData") + "Mod/Draft/icons/"
				if os.path.exists(path1): return path1
				else: return path2
					
			def getcol(self):
				self.color=QtGui.QColorDialog.getColor()
				self.colorPix.fill(self.color)
				self.colorButton.setIcon(QtGui.QIcon(self.colorPix))
				if FreeCAD.ParamGet("User parameter:BaseApp/Preferences/Mod/Draft").GetBool("saveonexit"):
					FreeCAD.ParamGet("User parameter:BaseApp/Preferences/Mod/Draft").SetBool("color",self.color.rgb())

			def checkx(self):
				if self.yValue.isEnabled():
					self.yValue.setFocus()
					self.yValue.selectAll()
				else:
					self.checky()

			def checky(self):
				if self.zValue.isEnabled():
					self.zValue.setFocus()
					self.zValue.selectAll()
				else:
					self.validatePoint()

			def validatePoint(self):
				"function for checking and sending numbers entered manually"
				if self.sourceCmd != None:
					if (self.labelRadius.isVisible()):
						try:
							rad=float(self.radiusValue.text())
						except ValueError:
							pass
						else:
							self.sourceCmd.numericRadius(rad)
					else:
						try:
							numx=float(self.xValue.text())
							numy=float(self.yValue.text())
							numz=float(self.zValue.text())
						except ValueError:
							pass
						else:
							if self.isRelative.isVisible() and self.isRelative.isChecked():
								if len(self.sourceCmd.node) > 0:
									last = self.sourceCmd.node[len(self.sourceCmd.node)-1]
									numx = last.x + numx
									numy = last.y + numy
									numz = last.z + numz
							self.sourceCmd.numericInput(numx,numy,numz)

			def finish(self):
				"finish button action"
				self.sourceCmd.finish(False)

			def closeLine(self):
				"close button action"
				self.sourceCmd.finish(True)
		
			def undoSegment(self):
				"undo last line segment"
				self.sourceCmd.undolast()

			def checkSpecialChars(self,txt):
				if txt.endsWith(" "):
					self.isRelative.setChecked(not self.isRelative.isChecked())
					for i in [self.xValue,self.yValue,self.zValue]:
						if (i.text() == txt): i.setText("")
				if txt.endsWith("f"):
					if self.finishButton.isVisible(): self.finish()
				if txt.endsWith("c"):
					if self.closeButton.isVisible(): self.closeLine()
					elif self.isCopy.isVisible(): self.isCopy.setChecked(not self.isCopy.isChecked())

			def sendText(self):
				self.sourceCmd.text=str(self.textValue.text())
				self.sourceCmd.createObject()

#---------------------------------------------------------------------------
# Initialization
#---------------------------------------------------------------------------

		self.app = QtGui.qApp
		self.mw = self.app.activeWindow()
		self.draftWidget = DraftDockWidget()
		self.ui = Ui_draftToolbar()
		self.ui.setupUi(self.draftWidget)
		self.draftWidget.setObjectName("draftToolbar")
		self.draftWidget.setWindowTitle("draftCommand")
		self.mw.addDockWidget(QtCore.Qt.TopDockWidgetArea,self.draftWidget)
		self.draftWidget.setVisible(False)
		self.draftWidget.toggleViewAction().setVisible(False)

