# TemplatePyMod gui init module  
# (c) 2007 Juergen Riegel LGPL
#


class TemplatePyModWorkbench ( Workbench ):
	"Test workbench object"
	Icon = """
			/* XPM */
			static const char *test_icon[]={
			"16 16 2 1\",
			"a c #000000\",
			". c None\",
			"................\",
			"................\",
			"..############..\",
			"..############..\",
			"..############..\",
			"......####......\",
			"......####......\",
			"......####......\",
			"......####......\",
			"......####......\",
			"......####......\",
			"......####......\",
			"......####......\",
			"......####......\",
			"................\",
			"................\"};
			"""
	MenuText = "Python template"
	ToolTip = "Python template workbench"
	
	def Activate(self):
		Log ('Loading GUI of TemplatePyMod module...\n')
		import Commands

		self.appendToolbar("TemplateTools",["TemplatePyMod_Cmd1","TemplatePyMod_Cmd2"])

		menu = ["ModulePy &Commands","PyModuleCommands"]
		list = ["TemplatePyMod_Cmd1","TemplatePyMod_Cmd2"]
		self.appendCommandbar("PyModuleCommands",list)
		self.appendMenu(menu,list)

		Log ('Loading GUI of Test module... done\n')

Gui.addWorkbench(TemplatePyModWorkbench())
