class ArchWorkbench(Workbench):
	"Arch workbench object"
	Icon = """
                /* XPM */
                static char * arch_xpm[] = {
                "16 16 9 1",
                " 	c None",
                ".	c #543016",
                "+	c #6D2F08",
                "@	c #954109",
                "#	c #874C24",
                "$	c #AE6331",
                "%	c #C86423",
                "&	c #FD7C26",
                "*	c #F5924F",
                "                ",
                "                ",
                "       #        ",
                "      ***$#     ",
                "    .*******.   ",
                "   *##$****#+   ",
                " #**%&&##$#@@   ",
                ".$**%&&&&+@@+   ",
                "@&@#$$%&&@@+..  ",
                "@&&&%#.#$#+..#$.",
                " %&&&&+%#.$**$@+",
                "   @%&+&&&$##@@+",
                "     @.&&&&&@@@ ",
                "        @%&&@@  ",
                "           @+   ",
                "                "};
			"""
	MenuText = "Arch"
	ToolTip = "Architecture workbench"
	
	def Initialize(self):
                import Wall,Cell
                Gui.addCommand('Arch_Wall',Wall.CommandWall())
                Gui.addCommand('Arch_Cell',Floor.CommandFloor())
                tools = ["Arch_Wall","Arch_Cell"]
                self.appendToolbar("Arch tools",tools)
                self.appendMenu("Architecture",tools)
		Log ('Loading Arch module... done\n')
	def Activated(self):
		Msg("ArchWorkbench::Activated()\n")
	def Deactivated(self):
		Msg("ArchWorkbench::Deactivated()\n")

Gui.addIconPath("/home/yorik/Apps/FreeCAD/src/Mod/Arch/Resources/icons")
Gui.addWorkbench(ArchWorkbench)
App.addImportType("Industry Foundation Classes (*.ifc)","importIFC") 

