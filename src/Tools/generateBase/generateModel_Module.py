#!/usr/bin/env python

#
# Generated Sun May 27 16:29:48 2007 by generateDS.py.
#

import sys
import getopt
from xml.dom import minidom
from xml.dom import Node

#
# If you have installed IPython you can uncomment and use the following.
# IPython is available from http://ipython.scipy.org/.
#

## from IPython.Shell import IPShellEmbed
## args = ''
## ipshell = IPShellEmbed(args,
##     banner = 'Dropping into IPython',
##     exit_msg = 'Leaving Interpreter, back to program.')

# Then use the following line where and when you want to drop into the
# IPython shell:
#    ipshell('<some message> -- Entering ipshell.\nHit Ctrl-D to exit')

#
# Support/utility functions.
#

def showIndent(outfile, level):
    for idx in range(level):
        outfile.write('    ')

def quote_xml(inStr):
    s1 = inStr
    s1 = s1.replace('&', '&amp;')
    s1 = s1.replace('<', '&lt;')
    s1 = s1.replace('"', '&quot;')
    return s1

def quote_python(inStr):
    s1 = inStr
    if s1.find("'") == -1:
        if s1.find('\n') == -1:
            return "'%s'" % s1
        else:
            return "'''%s'''" % s1
    else:
        if s1.find('"') != -1:
            s1 = s1.replace('"', '\\"')
        if s1.find('\n') == -1:
            return '"%s"' % s1
        else:
            return '"""%s"""' % s1


class MixedContainer:
    # Constants for category:
    CategoryNone = 0
    CategoryText = 1
    CategorySimple = 2
    CategoryComplex = 3
    # Constants for content_type:
    TypeNone = 0
    TypeText = 1
    TypeString = 2
    TypeInteger = 3
    TypeFloat = 4
    TypeDecimal = 5
    TypeDouble = 6
    TypeBoolean = 7
    def __init__(self, category, content_type, name, value):
        self.category = category
        self.content_type = content_type
        self.name = name
        self.value = value
    def getCategory(self):
        return self.category
    def getContenttype(self, content_type):
        return self.content_type
    def getValue(self):
        return self.value
    def getName(self):
        return self.name
    def export(self, outfile, level, name):
        if self.category == MixedContainer.CategoryText:
            outfile.write(self.value)
        elif self.category == MixedContainer.CategorySimple:
            self.exportSimple(outfile, level, name)
        else:    # category == MixedContainer.CategoryComplex
            self.value.export(outfile, level, name)
    def exportSimple(self, outfile, level, name):
        if self.content_type == MixedContainer.TypeString:
            outfile.write('<%s>%s</%s>' % (self.name, self.value, self.name))
        elif self.content_type == MixedContainer.TypeInteger or \
                self.content_type == MixedContainer.TypeBoolean:
            outfile.write('<%s>%d</%s>' % (self.name, self.value, self.name))
        elif self.content_type == MixedContainer.TypeFloat or \
                self.content_type == MixedContainer.TypeDecimal:
            outfile.write('<%s>%f</%s>' % (self.name, self.value, self.name))
        elif self.content_type == MixedContainer.TypeDouble:
            outfile.write('<%s>%g</%s>' % (self.name, self.value, self.name))
    def exportLiteral(self, outfile, level, name):
        if self.category == MixedContainer.CategoryText:
            showIndent(outfile, level)
            outfile.write('MixedContainer(%d, %d, "%s", "%s"),\n' % \
                (self.category, self.content_type, self.name, self.value))
        elif self.category == MixedContainer.CategorySimple:
            showIndent(outfile, level)
            outfile.write('MixedContainer(%d, %d, "%s", "%s"),\n' % \
                (self.category, self.content_type, self.name, self.value))
        else:    # category == MixedContainer.CategoryComplex
            showIndent(outfile, level)
            outfile.write('MixedContainer(%d, %d, "%s",\n' % \
                (self.category, self.content_type, self.name,))
            self.value.exportLiteral(outfile, level + 1)
            showIndent(outfile, level)
            outfile.write(')\n')


#
# Data representation classes.
#

class GenerateModel:
    subclass = None
    def __init__(self, Module=None, PythonExport=None):
        if Module is None:
            self.Module = []
        else:
            self.Module = Module
        if PythonExport is None:
            self.PythonExport = []
        else:
            self.PythonExport = PythonExport
    def factory(*args_, **kwargs_):
        if GenerateModel.subclass:
            return GenerateModel.subclass(*args_, **kwargs_)
        else:
            return GenerateModel(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getModule(self): return self.Module
    def setModule(self, Module): self.Module = Module
    def addModule(self, value): self.Module.append(value)
    def insertModule(self, index, value): self.Module[index] = value
    def getPythonexport(self): return self.PythonExport
    def setPythonexport(self, PythonExport): self.PythonExport = PythonExport
    def addPythonexport(self, value): self.PythonExport.append(value)
    def insertPythonexport(self, index, value): self.PythonExport[index] = value
    def export(self, outfile, level, name_='GenerateModel'):
        showIndent(outfile, level)
        outfile.write('<%s>\n' % name_)
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='GenerateModel'):
        pass
    def exportChildren(self, outfile, level, name_='GenerateModel'):
        for Module_ in self.getModule():
            Module_.export(outfile, level)
        for PythonExport_ in self.getPythonexport():
            PythonExport_.export(outfile, level)
    def exportLiteral(self, outfile, level, name_='GenerateModel'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('Module=[\n')
        level += 1
        for Module in self.Module:
            showIndent(outfile, level)
            outfile.write('Module(\n')
            Module.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
        showIndent(outfile, level)
        outfile.write('PythonExport=[\n')
        level += 1
        for PythonExport in self.PythonExport:
            showIndent(outfile, level)
            outfile.write('PythonExport(\n')
            PythonExport.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Module':
            obj_ = Module.factory()
            obj_.build(child_)
            self.Module.append(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'PythonExport':
            obj_ = PythonExport.factory()
            obj_.build(child_)
            self.PythonExport.append(obj_)
# end class GenerateModel


class PythonExport:
    subclass = None
    def __init__(self, FatherNamespace='', Name='', FatherInclude='', Father='', Namespace='', Twin='', Documentation=None, Methode=None, Attribute=None, CustomAttributes=''):
        self.FatherNamespace = FatherNamespace
        self.Name = Name
        self.FatherInclude = FatherInclude
        self.Father = Father
        self.Namespace = Namespace
        self.Twin = Twin
        self.Documentation = Documentation
        if Methode is None:
            self.Methode = []
        else:
            self.Methode = Methode
        if Attribute is None:
            self.Attribute = []
        else:
            self.Attribute = Attribute
        self.CustomAttributes = CustomAttributes
    def factory(*args_, **kwargs_):
        if PythonExport.subclass:
            return PythonExport.subclass(*args_, **kwargs_)
        else:
            return PythonExport(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getDocumentation(self): return self.Documentation
    def setDocumentation(self, Documentation): self.Documentation = Documentation
    def getMethode(self): return self.Methode
    def setMethode(self, Methode): self.Methode = Methode
    def addMethode(self, value): self.Methode.append(value)
    def insertMethode(self, index, value): self.Methode[index] = value
    def getAttribute(self): return self.Attribute
    def setAttribute(self, Attribute): self.Attribute = Attribute
    def addAttribute(self, value): self.Attribute.append(value)
    def insertAttribute(self, index, value): self.Attribute[index] = value
    def getCustomattributes(self): return self.CustomAttributes
    def setCustomattributes(self, CustomAttributes): self.CustomAttributes = CustomAttributes
    def getFathernamespace(self): return self.FatherNamespace
    def setFathernamespace(self, FatherNamespace): self.FatherNamespace = FatherNamespace
    def getName(self): return self.Name
    def setName(self, Name): self.Name = Name
    def getFatherinclude(self): return self.FatherInclude
    def setFatherinclude(self, FatherInclude): self.FatherInclude = FatherInclude
    def getFather(self): return self.Father
    def setFather(self, Father): self.Father = Father
    def getNamespace(self): return self.Namespace
    def setNamespace(self, Namespace): self.Namespace = Namespace
    def getTwin(self): return self.Twin
    def setTwin(self, Twin): self.Twin = Twin
    def export(self, outfile, level, name_='PythonExport'):
        showIndent(outfile, level)
        outfile.write('<%s' % (name_, ))
        self.exportAttributes(outfile, level, name_='PythonExport')
        outfile.write('>\n')
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='PythonExport'):
        outfile.write(' FatherNamespace="%s"' % (self.getFathernamespace(), ))
        outfile.write(' Name="%s"' % (self.getName(), ))
        outfile.write(' FatherInclude="%s"' % (self.getFatherinclude(), ))
        outfile.write(' Father="%s"' % (self.getFather(), ))
        outfile.write(' Namespace="%s"' % (self.getNamespace(), ))
        outfile.write(' Twin="%s"' % (self.getTwin(), ))
    def exportChildren(self, outfile, level, name_='PythonExport'):
        if self.Documentation:
            self.Documentation.export(outfile, level)
        for Methode_ in self.getMethode():
            Methode_.export(outfile, level)
        for Attribute_ in self.getAttribute():
            Attribute_.export(outfile, level)
        showIndent(outfile, level)
        outfile.write('<CustomAttributes>%s</CustomAttributes>\n' % quote_xml(self.getCustomattributes()))
    def exportLiteral(self, outfile, level, name_='PythonExport'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('FatherNamespace = "%s",\n' % (self.getFathernamespace(),))
        showIndent(outfile, level)
        outfile.write('Name = "%s",\n' % (self.getName(),))
        showIndent(outfile, level)
        outfile.write('FatherInclude = "%s",\n' % (self.getFatherinclude(),))
        showIndent(outfile, level)
        outfile.write('Father = "%s",\n' % (self.getFather(),))
        showIndent(outfile, level)
        outfile.write('Namespace = "%s",\n' % (self.getNamespace(),))
        showIndent(outfile, level)
        outfile.write('Twin = "%s",\n' % (self.getTwin(),))
    def exportLiteralChildren(self, outfile, level, name_):
        if self.Documentation:
            showIndent(outfile, level)
            outfile.write('Documentation=Documentation(\n')
            self.Documentation.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        showIndent(outfile, level)
        outfile.write('Methode=[\n')
        level += 1
        for Methode in self.Methode:
            showIndent(outfile, level)
            outfile.write('Methode(\n')
            Methode.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
        showIndent(outfile, level)
        outfile.write('Attribute=[\n')
        level += 1
        for Attribute in self.Attribute:
            showIndent(outfile, level)
            outfile.write('Attribute(\n')
            Attribute.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
        showIndent(outfile, level)
        outfile.write('CustomAttributes=%s,\n' % quote_python(self.getCustomattributes()))
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        if attrs.get('FatherNamespace'):
            self.FatherNamespace = attrs.get('FatherNamespace').value
        if attrs.get('Name'):
            self.Name = attrs.get('Name').value
        if attrs.get('FatherInclude'):
            self.FatherInclude = attrs.get('FatherInclude').value
        if attrs.get('Father'):
            self.Father = attrs.get('Father').value
        if attrs.get('Namespace'):
            self.Namespace = attrs.get('Namespace').value
        if attrs.get('Twin'):
            self.Twin = attrs.get('Twin').value
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Documentation':
            obj_ = Documentation.factory()
            obj_.build(child_)
            self.setDocumentation(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Methode':
            obj_ = Methode.factory()
            obj_.build(child_)
            self.Methode.append(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Attribute':
            obj_ = Attribute.factory()
            obj_.build(child_)
            self.Attribute.append(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'CustomAttributes':
            CustomAttributes_ = ''
            for text__content_ in child_.childNodes:
                CustomAttributes_ += text__content_.nodeValue
            self.CustomAttributes = CustomAttributes_
# end class PythonExport


class Methode:
    subclass = None
    def __init__(self, Name='', Documentation=None, Parameter=None):
        self.Name = Name
        self.Documentation = Documentation
        if Parameter is None:
            self.Parameter = []
        else:
            self.Parameter = Parameter
    def factory(*args_, **kwargs_):
        if Methode.subclass:
            return Methode.subclass(*args_, **kwargs_)
        else:
            return Methode(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getDocumentation(self): return self.Documentation
    def setDocumentation(self, Documentation): self.Documentation = Documentation
    def getParameter(self): return self.Parameter
    def setParameter(self, Parameter): self.Parameter = Parameter
    def addParameter(self, value): self.Parameter.append(value)
    def insertParameter(self, index, value): self.Parameter[index] = value
    def getName(self): return self.Name
    def setName(self, Name): self.Name = Name
    def export(self, outfile, level, name_='Methode'):
        showIndent(outfile, level)
        outfile.write('<%s' % (name_, ))
        self.exportAttributes(outfile, level, name_='Methode')
        outfile.write('>\n')
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='Methode'):
        outfile.write(' Name="%s"' % (self.getName(), ))
    def exportChildren(self, outfile, level, name_='Methode'):
        if self.Documentation:
            self.Documentation.export(outfile, level)
        for Parameter_ in self.getParameter():
            Parameter_.export(outfile, level)
    def exportLiteral(self, outfile, level, name_='Methode'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('Name = "%s",\n' % (self.getName(),))
    def exportLiteralChildren(self, outfile, level, name_):
        if self.Documentation:
            showIndent(outfile, level)
            outfile.write('Documentation=Documentation(\n')
            self.Documentation.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        showIndent(outfile, level)
        outfile.write('Parameter=[\n')
        level += 1
        for Parameter in self.Parameter:
            showIndent(outfile, level)
            outfile.write('Parameter(\n')
            Parameter.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        if attrs.get('Name'):
            self.Name = attrs.get('Name').value
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Documentation':
            obj_ = Documentation.factory()
            obj_.build(child_)
            self.setDocumentation(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Parameter':
            obj_ = Parameter.factory()
            obj_.build(child_)
            self.Parameter.append(obj_)
# end class Methode


class Attribute:
    subclass = None
    def __init__(self, ReadOnly=0, Name='', Documentation=None, Parameter=None):
        self.ReadOnly = ReadOnly
        self.Name = Name
        self.Documentation = Documentation
        self.Parameter = Parameter
    def factory(*args_, **kwargs_):
        if Attribute.subclass:
            return Attribute.subclass(*args_, **kwargs_)
        else:
            return Attribute(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getDocumentation(self): return self.Documentation
    def setDocumentation(self, Documentation): self.Documentation = Documentation
    def getParameter(self): return self.Parameter
    def setParameter(self, Parameter): self.Parameter = Parameter
    def getReadonly(self): return self.ReadOnly
    def setReadonly(self, ReadOnly): self.ReadOnly = ReadOnly
    def getName(self): return self.Name
    def setName(self, Name): self.Name = Name
    def export(self, outfile, level, name_='Attribute'):
        showIndent(outfile, level)
        outfile.write('<%s' % (name_, ))
        self.exportAttributes(outfile, level, name_='Attribute')
        outfile.write('>\n')
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='Attribute'):
        outfile.write(' ReadOnly="%s"' % (self.getReadonly(), ))
        outfile.write(' Name="%s"' % (self.getName(), ))
    def exportChildren(self, outfile, level, name_='Attribute'):
        if self.Documentation:
            self.Documentation.export(outfile, level)
        if self.Parameter:
            self.Parameter.export(outfile, level)
    def exportLiteral(self, outfile, level, name_='Attribute'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('ReadOnly = "%s",\n' % (self.getReadonly(),))
        showIndent(outfile, level)
        outfile.write('Name = "%s",\n' % (self.getName(),))
    def exportLiteralChildren(self, outfile, level, name_):
        if self.Documentation:
            showIndent(outfile, level)
            outfile.write('Documentation=Documentation(\n')
            self.Documentation.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        if self.Parameter:
            showIndent(outfile, level)
            outfile.write('Parameter=Parameter(\n')
            self.Parameter.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        if attrs.get('ReadOnly'):
            if attrs.get('ReadOnly').value in ('true', '1'):
                self.ReadOnly = 1
            elif attrs.get('ReadOnly').value in ('false', '0'):
                self.ReadOnly = 0
            else:
                raise ValueError('Bad boolean attribute (ReadOnly)')
        if attrs.get('Name'):
            self.Name = attrs.get('Name').value
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Documentation':
            obj_ = Documentation.factory()
            obj_.build(child_)
            self.setDocumentation(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Parameter':
            obj_ = Parameter.factory()
            obj_.build(child_)
            self.setParameter(obj_)
# end class Attribute


class Module:
    subclass = None
    def __init__(self, Name='', Documentation=None, Dependencies=None, Content=None):
        self.Name = Name
        self.Documentation = Documentation
        self.Dependencies = Dependencies
        self.Content = Content
    def factory(*args_, **kwargs_):
        if Module.subclass:
            return Module.subclass(*args_, **kwargs_)
        else:
            return Module(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getDocumentation(self): return self.Documentation
    def setDocumentation(self, Documentation): self.Documentation = Documentation
    def getDependencies(self): return self.Dependencies
    def setDependencies(self, Dependencies): self.Dependencies = Dependencies
    def getContent(self): return self.Content
    def setContent(self, Content): self.Content = Content
    def getName(self): return self.Name
    def setName(self, Name): self.Name = Name
    def export(self, outfile, level, name_='Module'):
        showIndent(outfile, level)
        outfile.write('<%s' % (name_, ))
        self.exportAttributes(outfile, level, name_='Module')
        outfile.write('>\n')
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='Module'):
        outfile.write(' Name="%s"' % (self.getName(), ))
    def exportChildren(self, outfile, level, name_='Module'):
        if self.Documentation:
            self.Documentation.export(outfile, level)
        if self.Dependencies:
            self.Dependencies.export(outfile, level)
        if self.Content:
            self.Content.export(outfile, level)
    def exportLiteral(self, outfile, level, name_='Module'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('Name = "%s",\n' % (self.getName(),))
    def exportLiteralChildren(self, outfile, level, name_):
        if self.Documentation:
            showIndent(outfile, level)
            outfile.write('Documentation=Documentation(\n')
            self.Documentation.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        if self.Dependencies:
            showIndent(outfile, level)
            outfile.write('Dependencies=Dependencies(\n')
            self.Dependencies.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        if self.Content:
            showIndent(outfile, level)
            outfile.write('Content=Content(\n')
            self.Content.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        if attrs.get('Name'):
            self.Name = attrs.get('Name').value
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Documentation':
            obj_ = Documentation.factory()
            obj_.build(child_)
            self.setDocumentation(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Dependencies':
            obj_ = Dependencies.factory()
            obj_.build(child_)
            self.setDependencies(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Content':
            obj_ = Content.factory()
            obj_.build(child_)
            self.setContent(obj_)
# end class Module


class Dependencies:
    subclass = None
    def __init__(self, Module=None):
        if Module is None:
            self.Module = []
        else:
            self.Module = Module
    def factory(*args_, **kwargs_):
        if Dependencies.subclass:
            return Dependencies.subclass(*args_, **kwargs_)
        else:
            return Dependencies(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getModule(self): return self.Module
    def setModule(self, Module): self.Module = Module
    def addModule(self, value): self.Module.append(value)
    def insertModule(self, index, value): self.Module[index] = value
    def export(self, outfile, level, name_='Dependencies'):
        showIndent(outfile, level)
        outfile.write('<%s>\n' % name_)
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='Dependencies'):
        pass
    def exportChildren(self, outfile, level, name_='Dependencies'):
        for Module_ in self.getModule():
            Module_.export(outfile, level)
    def exportLiteral(self, outfile, level, name_='Dependencies'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('Module=[\n')
        level += 1
        for Module in self.Module:
            showIndent(outfile, level)
            outfile.write('Module(\n')
            Module.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Module':
            obj_ = Module.factory()
            obj_.build(child_)
            self.Module.append(obj_)
# end class Dependencies


class Content:
    subclass = None
    def __init__(self, Property=None, Feature=None, DocObject=None, GuiCommand=None, PreferencesPage=None):
        if Property is None:
            self.Property = []
        else:
            self.Property = Property
        if Feature is None:
            self.Feature = []
        else:
            self.Feature = Feature
        if DocObject is None:
            self.DocObject = []
        else:
            self.DocObject = DocObject
        if GuiCommand is None:
            self.GuiCommand = []
        else:
            self.GuiCommand = GuiCommand
        if PreferencesPage is None:
            self.PreferencesPage = []
        else:
            self.PreferencesPage = PreferencesPage
    def factory(*args_, **kwargs_):
        if Content.subclass:
            return Content.subclass(*args_, **kwargs_)
        else:
            return Content(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getProperty(self): return self.Property
    def setProperty(self, Property): self.Property = Property
    def addProperty(self, value): self.Property.append(value)
    def insertProperty(self, index, value): self.Property[index] = value
    def getFeature(self): return self.Feature
    def setFeature(self, Feature): self.Feature = Feature
    def addFeature(self, value): self.Feature.append(value)
    def insertFeature(self, index, value): self.Feature[index] = value
    def getDocobject(self): return self.DocObject
    def setDocobject(self, DocObject): self.DocObject = DocObject
    def addDocobject(self, value): self.DocObject.append(value)
    def insertDocobject(self, index, value): self.DocObject[index] = value
    def getGuicommand(self): return self.GuiCommand
    def setGuicommand(self, GuiCommand): self.GuiCommand = GuiCommand
    def addGuicommand(self, value): self.GuiCommand.append(value)
    def insertGuicommand(self, index, value): self.GuiCommand[index] = value
    def getPreferencespage(self): return self.PreferencesPage
    def setPreferencespage(self, PreferencesPage): self.PreferencesPage = PreferencesPage
    def addPreferencespage(self, value): self.PreferencesPage.append(value)
    def insertPreferencespage(self, index, value): self.PreferencesPage[index] = value
    def export(self, outfile, level, name_='Content'):
        showIndent(outfile, level)
        outfile.write('<%s>\n' % name_)
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='Content'):
        pass
    def exportChildren(self, outfile, level, name_='Content'):
        for Property_ in self.getProperty():
            Property_.export(outfile, level)
        for Feature_ in self.getFeature():
            Feature_.export(outfile, level)
        for DocObject_ in self.getDocobject():
            DocObject_.export(outfile, level)
        for GuiCommand_ in self.getGuicommand():
            showIndent(outfile, level)
            outfile.write('<GuiCommand>%s</GuiCommand>\n' % quote_xml(GuiCommand_))
        for PreferencesPage_ in self.getPreferencespage():
            showIndent(outfile, level)
            outfile.write('<PreferencesPage>%s</PreferencesPage>\n' % quote_xml(PreferencesPage_))
    def exportLiteral(self, outfile, level, name_='Content'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('Property=[\n')
        level += 1
        for Property in self.Property:
            showIndent(outfile, level)
            outfile.write('Property(\n')
            Property.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
        showIndent(outfile, level)
        outfile.write('Feature=[\n')
        level += 1
        for Feature in self.Feature:
            showIndent(outfile, level)
            outfile.write('Feature(\n')
            Feature.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
        showIndent(outfile, level)
        outfile.write('DocObject=[\n')
        level += 1
        for DocObject in self.DocObject:
            showIndent(outfile, level)
            outfile.write('DocObject(\n')
            DocObject.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
        showIndent(outfile, level)
        outfile.write('GuiCommand=[\n')
        level += 1
        for GuiCommand in self.GuiCommand:
            showIndent(outfile, level)
            outfile.write('%s,\n' % quote_python(GuiCommand))
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
        showIndent(outfile, level)
        outfile.write('PreferencesPage=[\n')
        level += 1
        for PreferencesPage in self.PreferencesPage:
            showIndent(outfile, level)
            outfile.write('%s,\n' % quote_python(PreferencesPage))
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Property':
            obj_ = Property.factory()
            obj_.build(child_)
            self.Property.append(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Feature':
            obj_ = Feature.factory()
            obj_.build(child_)
            self.Feature.append(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'DocObject':
            obj_ = DocObject.factory()
            obj_.build(child_)
            self.DocObject.append(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'GuiCommand':
            GuiCommand_ = ''
            for text__content_ in child_.childNodes:
                GuiCommand_ += text__content_.nodeValue
            self.GuiCommand.append(GuiCommand_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'PreferencesPage':
            PreferencesPage_ = ''
            for text__content_ in child_.childNodes:
                PreferencesPage_ += text__content_.nodeValue
            self.PreferencesPage.append(PreferencesPage_)
# end class Content


class Feature:
    subclass = None
    def __init__(self, Name='', Documentation=None, Property=None, ViewProvider=None):
        self.Name = Name
        self.Documentation = Documentation
        if Property is None:
            self.Property = []
        else:
            self.Property = Property
        self.ViewProvider = ViewProvider
    def factory(*args_, **kwargs_):
        if Feature.subclass:
            return Feature.subclass(*args_, **kwargs_)
        else:
            return Feature(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getDocumentation(self): return self.Documentation
    def setDocumentation(self, Documentation): self.Documentation = Documentation
    def getProperty(self): return self.Property
    def setProperty(self, Property): self.Property = Property
    def addProperty(self, value): self.Property.append(value)
    def insertProperty(self, index, value): self.Property[index] = value
    def getViewprovider(self): return self.ViewProvider
    def setViewprovider(self, ViewProvider): self.ViewProvider = ViewProvider
    def getName(self): return self.Name
    def setName(self, Name): self.Name = Name
    def export(self, outfile, level, name_='Feature'):
        showIndent(outfile, level)
        outfile.write('<%s' % (name_, ))
        self.exportAttributes(outfile, level, name_='Feature')
        outfile.write('>\n')
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='Feature'):
        outfile.write(' Name="%s"' % (self.getName(), ))
    def exportChildren(self, outfile, level, name_='Feature'):
        if self.Documentation:
            self.Documentation.export(outfile, level)
        for Property_ in self.getProperty():
            Property_.export(outfile, level)
        if self.ViewProvider:
            self.ViewProvider.export(outfile, level)
    def exportLiteral(self, outfile, level, name_='Feature'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('Name = "%s",\n' % (self.getName(),))
    def exportLiteralChildren(self, outfile, level, name_):
        if self.Documentation:
            showIndent(outfile, level)
            outfile.write('Documentation=Documentation(\n')
            self.Documentation.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        showIndent(outfile, level)
        outfile.write('Property=[\n')
        level += 1
        for Property in self.Property:
            showIndent(outfile, level)
            outfile.write('Property(\n')
            Property.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
        if self.ViewProvider:
            showIndent(outfile, level)
            outfile.write('ViewProvider=ViewProvider(\n')
            self.ViewProvider.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        if attrs.get('Name'):
            self.Name = attrs.get('Name').value
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Documentation':
            obj_ = Documentation.factory()
            obj_.build(child_)
            self.setDocumentation(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Property':
            obj_ = Property.factory()
            obj_.build(child_)
            self.Property.append(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'ViewProvider':
            obj_ = ViewProvider.factory()
            obj_.build(child_)
            self.setViewprovider(obj_)
# end class Feature


class DocObject:
    subclass = None
    def __init__(self, Name='', Documentation=None, Property=None):
        self.Name = Name
        self.Documentation = Documentation
        if Property is None:
            self.Property = []
        else:
            self.Property = Property
    def factory(*args_, **kwargs_):
        if DocObject.subclass:
            return DocObject.subclass(*args_, **kwargs_)
        else:
            return DocObject(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getDocumentation(self): return self.Documentation
    def setDocumentation(self, Documentation): self.Documentation = Documentation
    def getProperty(self): return self.Property
    def setProperty(self, Property): self.Property = Property
    def addProperty(self, value): self.Property.append(value)
    def insertProperty(self, index, value): self.Property[index] = value
    def getName(self): return self.Name
    def setName(self, Name): self.Name = Name
    def export(self, outfile, level, name_='DocObject'):
        showIndent(outfile, level)
        outfile.write('<%s' % (name_, ))
        self.exportAttributes(outfile, level, name_='DocObject')
        outfile.write('>\n')
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='DocObject'):
        outfile.write(' Name="%s"' % (self.getName(), ))
    def exportChildren(self, outfile, level, name_='DocObject'):
        if self.Documentation:
            self.Documentation.export(outfile, level)
        for Property_ in self.getProperty():
            Property_.export(outfile, level)
    def exportLiteral(self, outfile, level, name_='DocObject'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('Name = "%s",\n' % (self.getName(),))
    def exportLiteralChildren(self, outfile, level, name_):
        if self.Documentation:
            showIndent(outfile, level)
            outfile.write('Documentation=Documentation(\n')
            self.Documentation.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        showIndent(outfile, level)
        outfile.write('Property=[\n')
        level += 1
        for Property in self.Property:
            showIndent(outfile, level)
            outfile.write('Property(\n')
            Property.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        if attrs.get('Name'):
            self.Name = attrs.get('Name').value
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Documentation':
            obj_ = Documentation.factory()
            obj_.build(child_)
            self.setDocumentation(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Property':
            obj_ = Property.factory()
            obj_.build(child_)
            self.Property.append(obj_)
# end class DocObject


class Property:
    subclass = None
    def __init__(self, Type='', Name='', StartValue='', Documentation=None):
        self.Type = Type
        self.Name = Name
        self.StartValue = StartValue
        self.Documentation = Documentation
    def factory(*args_, **kwargs_):
        if Property.subclass:
            return Property.subclass(*args_, **kwargs_)
        else:
            return Property(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getDocumentation(self): return self.Documentation
    def setDocumentation(self, Documentation): self.Documentation = Documentation
    def getType(self): return self.Type
    def setType(self, Type): self.Type = Type
    def getName(self): return self.Name
    def setName(self, Name): self.Name = Name
    def getStartvalue(self): return self.StartValue
    def setStartvalue(self, StartValue): self.StartValue = StartValue
    def export(self, outfile, level, name_='Property'):
        showIndent(outfile, level)
        outfile.write('<%s' % (name_, ))
        self.exportAttributes(outfile, level, name_='Property')
        outfile.write('>\n')
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='Property'):
        outfile.write(' Type="%s"' % (self.getType(), ))
        outfile.write(' Name="%s"' % (self.getName(), ))
        if self.getStartvalue() is not None:
            outfile.write(' StartValue="%s"' % (self.getStartvalue(), ))
    def exportChildren(self, outfile, level, name_='Property'):
        if self.Documentation:
            self.Documentation.export(outfile, level)
    def exportLiteral(self, outfile, level, name_='Property'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('Type = "%s",\n' % (self.getType(),))
        showIndent(outfile, level)
        outfile.write('Name = "%s",\n' % (self.getName(),))
        showIndent(outfile, level)
        outfile.write('StartValue = "%s",\n' % (self.getStartvalue(),))
    def exportLiteralChildren(self, outfile, level, name_):
        if self.Documentation:
            showIndent(outfile, level)
            outfile.write('Documentation=Documentation(\n')
            self.Documentation.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        if attrs.get('Type'):
            self.Type = attrs.get('Type').value
        if attrs.get('Name'):
            self.Name = attrs.get('Name').value
        if attrs.get('StartValue'):
            self.StartValue = attrs.get('StartValue').value
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Documentation':
            obj_ = Documentation.factory()
            obj_.build(child_)
            self.setDocumentation(obj_)
# end class Property


class Documentation:
    subclass = None
    def __init__(self, Author=None, DeveloperDocu='', UserDocu=''):
        self.Author = Author
        self.DeveloperDocu = DeveloperDocu
        self.UserDocu = UserDocu
    def factory(*args_, **kwargs_):
        if Documentation.subclass:
            return Documentation.subclass(*args_, **kwargs_)
        else:
            return Documentation(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getAuthor(self): return self.Author
    def setAuthor(self, Author): self.Author = Author
    def getDeveloperdocu(self): return self.DeveloperDocu
    def setDeveloperdocu(self, DeveloperDocu): self.DeveloperDocu = DeveloperDocu
    def getUserdocu(self): return self.UserDocu
    def setUserdocu(self, UserDocu): self.UserDocu = UserDocu
    def export(self, outfile, level, name_='Documentation'):
        showIndent(outfile, level)
        outfile.write('<%s>\n' % name_)
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='Documentation'):
        pass
    def exportChildren(self, outfile, level, name_='Documentation'):
        if self.Author:
            self.Author.export(outfile, level)
        showIndent(outfile, level)
        outfile.write('<DeveloperDocu>%s</DeveloperDocu>\n' % quote_xml(self.getDeveloperdocu()))
        showIndent(outfile, level)
        outfile.write('<UserDocu>%s</UserDocu>\n' % quote_xml(self.getUserdocu()))
    def exportLiteral(self, outfile, level, name_='Documentation'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        if self.Author:
            showIndent(outfile, level)
            outfile.write('Author=Author(\n')
            self.Author.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        showIndent(outfile, level)
        outfile.write('DeveloperDocu=%s,\n' % quote_python(self.getDeveloperdocu()))
        showIndent(outfile, level)
        outfile.write('UserDocu=%s,\n' % quote_python(self.getUserdocu()))
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Author':
            obj_ = Author.factory()
            obj_.build(child_)
            self.setAuthor(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'DeveloperDocu':
            DeveloperDocu_ = ''
            for text__content_ in child_.childNodes:
                DeveloperDocu_ += text__content_.nodeValue
            self.DeveloperDocu = DeveloperDocu_
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'UserDocu':
            UserDocu_ = ''
            for text__content_ in child_.childNodes:
                UserDocu_ += text__content_.nodeValue
            self.UserDocu = UserDocu_
# end class Documentation


class Author:
    subclass = None
    def __init__(self, Name='', Licence='', EMail='', valueOf_=''):
        self.Name = Name
        self.Licence = Licence
        self.EMail = EMail
        self.valueOf_ = valueOf_
    def factory(*args_, **kwargs_):
        if Author.subclass:
            return Author.subclass(*args_, **kwargs_)
        else:
            return Author(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getName(self): return self.Name
    def setName(self, Name): self.Name = Name
    def getLicence(self): return self.Licence
    def setLicence(self, Licence): self.Licence = Licence
    def getEmail(self): return self.EMail
    def setEmail(self, EMail): self.EMail = EMail
    def getValueOf_(self): return self.valueOf_
    def setValueOf_(self, valueOf_): self.valueOf_ = valueOf_
    def export(self, outfile, level, name_='Author'):
        showIndent(outfile, level)
        outfile.write('<%s' % (name_, ))
        self.exportAttributes(outfile, level, name_='Author')
        outfile.write('>\n')
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='Author'):
        outfile.write(' Name="%s"' % (self.getName(), ))
        if self.getLicence() is not None:
            outfile.write(' Licence="%s"' % (self.getLicence(), ))
        outfile.write(' EMail="%s"' % (self.getEmail(), ))
    def exportChildren(self, outfile, level, name_='Author'):
        outfile.write(self.valueOf_)
    def exportLiteral(self, outfile, level, name_='Author'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('Name = "%s",\n' % (self.getName(),))
        showIndent(outfile, level)
        outfile.write('Licence = "%s",\n' % (self.getLicence(),))
        showIndent(outfile, level)
        outfile.write('EMail = "%s",\n' % (self.getEmail(),))
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('valueOf_ = "%s",\n' % (self.valueOf_,))
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        if attrs.get('Name'):
            self.Name = attrs.get('Name').value
        if attrs.get('Licence'):
            self.Licence = attrs.get('Licence').value
        if attrs.get('EMail'):
            self.EMail = attrs.get('EMail').value
    def buildChildren(self, child_, nodeName_):
        self.valueOf_ = ''
        for child in child_.childNodes:
            if child.nodeType == Node.TEXT_NODE:
                self.valueOf_ += child.nodeValue
# end class Author


class ViewProvider:
    subclass = None
    def __init__(self, Property=None):
        if Property is None:
            self.Property = []
        else:
            self.Property = Property
    def factory(*args_, **kwargs_):
        if ViewProvider.subclass:
            return ViewProvider.subclass(*args_, **kwargs_)
        else:
            return ViewProvider(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getProperty(self): return self.Property
    def setProperty(self, Property): self.Property = Property
    def addProperty(self, value): self.Property.append(value)
    def insertProperty(self, index, value): self.Property[index] = value
    def export(self, outfile, level, name_='ViewProvider'):
        showIndent(outfile, level)
        outfile.write('<%s>\n' % name_)
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='ViewProvider'):
        pass
    def exportChildren(self, outfile, level, name_='ViewProvider'):
        for Property_ in self.getProperty():
            Property_.export(outfile, level)
    def exportLiteral(self, outfile, level, name_='ViewProvider'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('Property=[\n')
        level += 1
        for Property in self.Property:
            showIndent(outfile, level)
            outfile.write('Property(\n')
            Property.exportLiteral(outfile, level)
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'Property':
            obj_ = Property.factory()
            obj_.build(child_)
            self.Property.append(obj_)
# end class ViewProvider


class Parameter:
    subclass = None
    def __init__(self, Type='', Name='', valueOf_=''):
        self.Type = Type
        self.Name = Name
        self.valueOf_ = valueOf_
    def factory(*args_, **kwargs_):
        if Parameter.subclass:
            return Parameter.subclass(*args_, **kwargs_)
        else:
            return Parameter(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getType(self): return self.Type
    def setType(self, Type): self.Type = Type
    def getName(self): return self.Name
    def setName(self, Name): self.Name = Name
    def getValueOf_(self): return self.valueOf_
    def setValueOf_(self, valueOf_): self.valueOf_ = valueOf_
    def export(self, outfile, level, name_='Parameter'):
        showIndent(outfile, level)
        outfile.write('<%s' % (name_, ))
        self.exportAttributes(outfile, level, name_='Parameter')
        outfile.write('>\n')
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='Parameter'):
        outfile.write(' Type="%s"' % (self.getType(), ))
        outfile.write(' Name="%s"' % (self.getName(), ))
    def exportChildren(self, outfile, level, name_='Parameter'):
        outfile.write(self.valueOf_)
    def exportLiteral(self, outfile, level, name_='Parameter'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('Type = "%s",\n' % (self.getType(),))
        showIndent(outfile, level)
        outfile.write('Name = "%s",\n' % (self.getName(),))
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('valueOf_ = "%s",\n' % (self.valueOf_,))
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        if attrs.get('Type'):
            self.Type = attrs.get('Type').value
        if attrs.get('Name'):
            self.Name = attrs.get('Name').value
    def buildChildren(self, child_, nodeName_):
        self.valueOf_ = ''
        for child in child_.childNodes:
            if child.nodeType == Node.TEXT_NODE:
                self.valueOf_ += child.nodeValue
# end class Parameter


from xml.sax import handler, make_parser

class SaxStackElement:
    def __init__(self, name='', obj=None):
        self.name = name
        self.obj = obj
        self.content = ''

#
# SAX handler
#
class SaxGeneratemodelHandler(handler.ContentHandler):
    def __init__(self):
        self.stack = []
        self.root = None

    def getRoot(self):
        return self.root

    def setDocumentLocator(self, locator):
        self.locator = locator
    
    def showError(self, msg):
        print '*** (showError):', msg
        sys.exit(-1)

    def startElement(self, name, attrs):
        done = 0
        if name == 'GenerateModel':
            obj = GenerateModel.factory()
            stackObj = SaxStackElement('GenerateModel', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'Module':
            obj = Module.factory()
            stackObj = SaxStackElement('Module', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'PythonExport':
            obj = PythonExport.factory()
            val = attrs.get('FatherNamespace', None)
            if val is not None:
                obj.setFathernamespace(val)
            val = attrs.get('Name', None)
            if val is not None:
                obj.setName(val)
            val = attrs.get('FatherInclude', None)
            if val is not None:
                obj.setFatherinclude(val)
            val = attrs.get('Father', None)
            if val is not None:
                obj.setFather(val)
            val = attrs.get('Namespace', None)
            if val is not None:
                obj.setNamespace(val)
            val = attrs.get('Twin', None)
            if val is not None:
                obj.setTwin(val)
            stackObj = SaxStackElement('PythonExport', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'Documentation':
            obj = Documentation.factory()
            stackObj = SaxStackElement('Documentation', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'Methode':
            obj = Methode.factory()
            val = attrs.get('Name', None)
            if val is not None:
                obj.setName(val)
            stackObj = SaxStackElement('Methode', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'Parameter':
            obj = Parameter.factory()
            val = attrs.get('Type', None)
            if val is not None:
                obj.setType(val)
            val = attrs.get('Name', None)
            if val is not None:
                obj.setName(val)
            stackObj = SaxStackElement('Parameter', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'Attribute':
            obj = Attribute.factory()
            val = attrs.get('ReadOnly', None)
            if val is not None:
                if val in ('true', '1'):
                    obj.setReadonly(1)
                elif val in ('false', '0'):
                    obj.setReadonly(0)
                else:
                    self.reportError('"ReadOnly" attribute must be boolean ("true", "1", "false", "0")')
            val = attrs.get('Name', None)
            if val is not None:
                obj.setName(val)
            stackObj = SaxStackElement('Attribute', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'CustomAttributes':
            stackObj = SaxStackElement('CustomAttributes', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'Dependencies':
            obj = Dependencies.factory()
            stackObj = SaxStackElement('Dependencies', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'Content':
            obj = Content.factory()
            stackObj = SaxStackElement('Content', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'Property':
            obj = Property.factory()
            stackObj = SaxStackElement('Property', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'Feature':
            obj = Feature.factory()
            val = attrs.get('Name', None)
            if val is not None:
                obj.setName(val)
            stackObj = SaxStackElement('Feature', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'ViewProvider':
            obj = ViewProvider.factory()
            stackObj = SaxStackElement('ViewProvider', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'DocObject':
            obj = DocObject.factory()
            val = attrs.get('Name', None)
            if val is not None:
                obj.setName(val)
            stackObj = SaxStackElement('DocObject', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'GuiCommand':
            stackObj = SaxStackElement('GuiCommand', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'PreferencesPage':
            stackObj = SaxStackElement('PreferencesPage', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'Author':
            obj = Author.factory()
            val = attrs.get('Name', None)
            if val is not None:
                obj.setName(val)
            val = attrs.get('Licence', None)
            if val is not None:
                obj.setLicence(val)
            val = attrs.get('EMail', None)
            if val is not None:
                obj.setEmail(val)
            stackObj = SaxStackElement('Author', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'DeveloperDocu':
            stackObj = SaxStackElement('DeveloperDocu', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'UserDocu':
            stackObj = SaxStackElement('UserDocu', None)
            self.stack.append(stackObj)
            done = 1
        if not done:
            self.reportError('"%s" element not allowed here.' % name)

    def endElement(self, name):
        done = 0
        if name == 'GenerateModel':
            if len(self.stack) == 1:
                self.root = self.stack[-1].obj
                self.stack.pop()
                done = 1
        elif name == 'Module':
            if len(self.stack) >= 2:
                self.stack[-2].obj.addModule(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'PythonExport':
            if len(self.stack) >= 2:
                self.stack[-2].obj.addPythonexport(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'Documentation':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setDocumentation(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'Methode':
            if len(self.stack) >= 2:
                self.stack[-2].obj.addMethode(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'Parameter':
            if len(self.stack) >= 2:
                self.stack[-2].obj.addParameter(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'Attribute':
            if len(self.stack) >= 2:
                self.stack[-2].obj.addAttribute(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'CustomAttributes':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setCustomattributes(content)
                self.stack.pop()
                done = 1
        elif name == 'Dependencies':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setDependencies(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'Content':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setContent(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'Property':
            if len(self.stack) >= 2:
                self.stack[-2].obj.addProperty(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'Feature':
            if len(self.stack) >= 2:
                self.stack[-2].obj.addFeature(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'ViewProvider':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setViewprovider(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'DocObject':
            if len(self.stack) >= 2:
                self.stack[-2].obj.addDocobject(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'GuiCommand':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.addGuicommand(content)
                self.stack.pop()
                done = 1
        elif name == 'PreferencesPage':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.addPreferencespage(content)
                self.stack.pop()
                done = 1
        elif name == 'Author':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setAuthor(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'DeveloperDocu':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setDeveloperdocu(content)
                self.stack.pop()
                done = 1
        elif name == 'UserDocu':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setUserdocu(content)
                self.stack.pop()
                done = 1
        if not done:
            self.reportError('"%s" element not allowed here.' % name)

    def characters(self, chrs, start, end):
        if len(self.stack) > 0:
            self.stack[-1].content += chrs[start:end]

    def reportError(self, mesg):
        locator = self.locator
        sys.stderr.write('Doc: %s  Line: %d  Column: %d\n' % \
            (locator.getSystemId(), locator.getLineNumber(), 
            locator.getColumnNumber() + 1))
        sys.stderr.write(mesg)
        sys.stderr.write('\n')
        sys.exit(-1)
        #raise RuntimeError

USAGE_TEXT = """
Usage: python <Parser>.py [ -s ] <in_xml_file>
Options:
    -s        Use the SAX parser, not the minidom parser.
"""

def usage():
    print USAGE_TEXT
    sys.exit(-1)


#
# SAX handler used to determine the top level element.
#
class SaxSelectorHandler(handler.ContentHandler):
    def __init__(self):
        self.topElementName = None
    def getTopElementName(self):
        return self.topElementName
    def startElement(self, name, attrs):
        self.topElementName = name
        raise StopIteration


def parseSelect(inFileName):
    infile = file(inFileName, 'r')
    topElementName = None
    parser = make_parser()
    documentHandler = SaxSelectorHandler()
    parser.setContentHandler(documentHandler)
    try:
        try:
            parser.parse(infile)
        except StopIteration:
            topElementName = documentHandler.getTopElementName()
        if topElementName is None:
            raise RuntimeError, 'no top level element'
        topElementName = topElementName.replace('-', '_').replace(':', '_')
        if topElementName not in globals():
            raise RuntimeError, 'no class for top element: %s' % topElementName
        topElement = globals()[topElementName]
        infile.seek(0)
        doc = minidom.parse(infile)
    finally:
        infile.close()
    rootNode = doc.childNodes[0]
    rootObj = topElement.factory()
    rootObj.build(rootNode)
    # Enable Python to collect the space used by the DOM.
    doc = None
    sys.stdout.write('<?xml version="1.0" ?>\n')
    rootObj.export(sys.stdout, 0)
    return rootObj


def saxParse(inFileName):
    parser = make_parser()
    documentHandler = SaxGeneratemodelHandler()
    parser.setDocumentHandler(documentHandler)
    parser.parse('file:%s' % inFileName)
    root = documentHandler.getRoot()
    sys.stdout.write('<?xml version="1.0" ?>\n')
    root.export(sys.stdout, 0)
    return root


def saxParseString(inString):
    parser = make_parser()
    documentHandler = SaxGeneratemodelHandler()
    parser.setDocumentHandler(documentHandler)
    parser.feed(inString)
    parser.close()
    rootObj = documentHandler.getRoot()
    #sys.stdout.write('<?xml version="1.0" ?>\n')
    #rootObj.export(sys.stdout, 0)
    return rootObj


def parse(inFileName):
    doc = minidom.parse(inFileName)
    rootNode = doc.documentElement
    rootObj = GenerateModel.factory()
    rootObj.build(rootNode)
    # Enable Python to collect the space used by the DOM.
    doc = None
    sys.stdout.write('<?xml version="1.0" ?>\n')
    rootObj.export(sys.stdout, 0, name_="GenerateModel")
    return rootObj


def parseString(inString):
    doc = minidom.parseString(inString)
    rootNode = doc.documentElement
    rootObj = GenerateModel.factory()
    rootObj.build(rootNode)
    # Enable Python to collect the space used by the DOM.
    doc = None
    sys.stdout.write('<?xml version="1.0" ?>\n')
    rootObj.export(sys.stdout, 0, name_="GenerateModel")
    return rootObj


def parseLiteral(inFileName):
    doc = minidom.parse(inFileName)
    rootNode = doc.documentElement
    rootObj = GenerateModel.factory()
    rootObj.build(rootNode)
    # Enable Python to collect the space used by the DOM.
    doc = None
    sys.stdout.write('from generateModel_Module import *\n\n')
    sys.stdout.write('rootObj = GenerateModel(\n')
    rootObj.exportLiteral(sys.stdout, 0, name_="GenerateModel")
    sys.stdout.write(')\n')
    return rootObj


def main():
    args = sys.argv[1:]
    if len(args) == 2 and args[0] == '-s':
        saxParse(args[1])
    elif len(args) == 1:
        parse(args[0])
    else:
        usage()


if __name__ == '__main__':
    main()
    #import pdb
    #pdb.run('main()')

