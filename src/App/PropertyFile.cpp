/***************************************************************************
 *   (c) J�rgen Riegel (juergen.riegel@web.de) 2008                        *
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

#ifndef _PreComp_
#endif

/// Here the FreeCAD includes sorted by Base,App,Gui......

#include <Base/Exception.h>
#include <Base/Reader.h>
#include <Base/Writer.h>
#include <Base/Stream.h>
#include <Base/PyObjectBase.h>

#include "PropertyFile.h"
#include "Document.h"
#include "PropertyContainer.h"
#include "DocumentObject.h"
#define new DEBUG_CLIENTBLOCK
using namespace App;
using namespace Base;
using namespace std;



//**************************************************************************
// PropertyFileIncluded
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyFileIncluded , App::Property);

PropertyFileIncluded::PropertyFileIncluded()
{

}

PropertyFileIncluded::~PropertyFileIncluded()
{

}

std::string PropertyFileIncluded::getDocTransientPath(void)
{
	PropertyContainer *co = getContainer();
	if(co->isDerivedFrom(DocumentObject::getClassTypeId()))
		return dynamic_cast<DocumentObject*>(co)->getDocument()->TransientDir.getValue();

	return std::string();
}

void PropertyFileIncluded::setValue(const char* sFile, const char* sName)
{
   if (sFile) {
		if(_cValue == sFile)
			throw Base::Exception("Not possible to set the same file!");

	    aboutToSetValue(); // undo redo by move the file away with temp name

		// remove old file (if not moved by undo)
		Base::FileInfo value(_cValue);
		if(value.exists())
			value.deleteFile();

		Base::FileInfo file(sFile);
		std::string pathTrans = getDocTransientPath();
		std::string path = file.dirPath();
		std::string pathAct = value.dirPath();
	
		if(sName)
			_cValue = path + "/" + sName;
		else 
			if(value.fileName() == "")
				_cValue = path + "/" + file.fileName();

		if(path == pathTrans)
			file.renameFile(_cValue.c_str());
		else
			file.copyTo(_cValue.c_str());

        hasSetValue();
    }
}


const char* PropertyFileIncluded::getValue(void) const
{
     return _cValue.c_str();
}

PyObject *PropertyFileIncluded::getPyObject(void)
{
    PyObject *p = PyUnicode_DecodeUTF8(_cValue.c_str(),_cValue.size(),0);
    if (!p) throw Base::Exception("UTF8 conversion failure at PropertyString::getPyObject()");
    return p;
}

void PropertyFileIncluded::setPyObject(PyObject *value)
{
    std::string string;
    if (PyUnicode_Check(value)) {
        PyObject* unicode = PyUnicode_AsUTF8String(value);
        string = PyString_AsString(unicode);
        Py_DECREF(unicode);
    }
    else if (PyString_Check(value)) {
        string = PyString_AsString(value);
    }
    else if (PyFile_Check(value)) {
        PyObject* FileName = PyFile_Name(value);
        string = PyString_AsString(FileName);
    }
    else if (PyTuple_Check(value)) {
		if(PyTuple_Size(value) != 2)
			throw Py::TypeError("Tuple need size of (filePath,newFileName)"); 
		PyObject* file = PyTuple_GetItem(value,0);
		PyObject* name = PyTuple_GetItem(value,1);

		// decoding file
		std::string fileStr;
		if (PyUnicode_Check(file)) {
			PyObject* unicode = PyUnicode_AsUTF8String(file);
			fileStr = PyString_AsString(unicode);
			Py_DECREF(unicode);
		}
		else if (PyString_Check(file)) {
			fileStr = PyString_AsString(file);
		}
		else if (PyFile_Check(file)) {
			PyObject* FileName = PyFile_Name(file);
			fileStr = PyString_AsString(FileName);
		} else {
			std::string error = std::string("first in tuple must be a file or string");
			error += value->ob_type->tp_name;
			throw Py::TypeError(error);
		}

		// decoding name
		std::string nameStr;
		if (PyString_Check(name)) {
			nameStr = PyString_AsString(name);
		}
		else if (PyFile_Check(name)) {
			PyObject* FileName = PyFile_Name(name);
			nameStr = PyString_AsString(FileName);
		} else {
			std::string error = std::string("second in tuple must be a string");
			error += value->ob_type->tp_name;
			throw Py::TypeError(error);
		}

		setValue(fileStr.c_str(),nameStr.c_str());
		return;
	
	}
    else {
        std::string error = std::string("type must be str or file");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }

    // assign the string
    setValue(string.c_str());
}

void PropertyFileIncluded::Save (Writer &writer) const
{
    if (writer.isForceXML()) {
        writer.Stream() << writer.ind() << "<FileIncluded file=\"\">" << endl;
	
		// write the file in the XML stream
		writer.insertBinFile(_cValue.c_str());

        writer.Stream() << writer.ind() <<"</FileIncluded>" << endl ;
    }
    else {
		// instead initiate a extra file 
		Base::FileInfo file(_cValue.c_str());
        writer.Stream() << writer.ind() << "<FileIncluded file=\"" << 
        writer.addFile(file.fileName().c_str(), this) << "\"/>" << std::endl;
    }

}

void PropertyFileIncluded::Restore(Base::XMLReader &reader)
{
    reader.readElement("FileIncluded");
    string file (reader.getAttribute("file") );

    if (!file.empty()) {
        // initate a file read
        reader.addFile(file.c_str(),this);
    }
}

void PropertyFileIncluded::SaveDocFile (Base::Writer &writer) const
{
    Base::OutputStream to(writer.Stream());
	std::ifstream from(_cValue.c_str());
	
	if (!from) throw Base::Exception("PropertyFileIncluded::SaveDocFile() File in document transient dir deleted");
	
	char ch;
	while (from.get(ch)) to << (ch);

}

void PropertyFileIncluded::RestoreDocFile(Base::Reader &reader)
{
    Base::InputStream from(reader);

	std::ofstream to(_cValue.c_str());
	if (!to) throw Base::Exception("PropertyFileIncluded::RestoreDocFile() File in document transient dir deleted");

	int8_t ch;
	while(from){
		from >> ch;
		to.put(char(ch));
	}


    //uint32_t uCt=0;
    //str >> uCt;
    //std::vector<float> values(uCt);
    //for (std::vector<float>::iterator it = values.begin(); it != values.end(); ++it) {
    //    str >> *it;
    //}
    //setValues(values);
}

Property *PropertyFileIncluded::Copy(void) const
{
    PropertyFileIncluded *p= new PropertyFileIncluded();
    p->_cValue = _cValue;
    return p;
}

void PropertyFileIncluded::Paste(const Property &from)
{
    aboutToSetValue();
    _cValue = dynamic_cast<const PropertyFileIncluded&>(from)._cValue;
    hasSetValue();
}


//**************************************************************************
// PropertyFile
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyFile , App::PropertyString);

PropertyFile::PropertyFile()
{

}

PropertyFile::~PropertyFile()
{

}

