/***************************************************************************
 *   Copyright (c) J�rgen Riegel          (juergen.riegel@web.de) 2002     *
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

#include "PropertyStandard.h"
#include "MaterialPy.h"
#define new DEBUG_CLIENTBLOCK
using namespace App;
using namespace Base;
using namespace std;




//**************************************************************************
//**************************************************************************
// PropertyInteger
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyInteger , App::Property);

//**************************************************************************
// Construction/Destruction


PropertyInteger::PropertyInteger()
{

}


PropertyInteger::~PropertyInteger()
{

}

//**************************************************************************
// Base class implementer


void PropertyInteger::setValue(long lValue)
{
    aboutToSetValue();
    _lValue=lValue;
    hasSetValue();
}

long PropertyInteger::getValue(void) const
{
    return _lValue;
}

PyObject *PropertyInteger::getPyObject(void)
{
    return Py_BuildValue("l", _lValue);
}

void PropertyInteger::setPyObject(PyObject *value)
{ 
    if (PyInt_Check(value)) {
        aboutToSetValue();
        _lValue = PyInt_AsLong(value);
        hasSetValue();
    } 
    else {
        std::string error = std::string("type must be int, not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

void PropertyInteger::Save (Writer &writer) const
{
    writer.Stream() << writer.ind() << "<Integer value=\"" <<  _lValue <<"\"/>" << std::endl;
}

void PropertyInteger::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("Integer");
    // get the value of my Attribute
    _lValue = reader.getAttributeAsInteger("value");
}

Property *PropertyInteger::Copy(void) const
{
    PropertyInteger *p= new PropertyInteger();
    p->_lValue = _lValue;
    return p;
}

void PropertyInteger::Paste(const Property &from)
{
    aboutToSetValue();
    _lValue = dynamic_cast<const PropertyInteger&>(from)._lValue;
    hasSetValue();
}


//**************************************************************************
//**************************************************************************
// PropertyPath
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyPath , App::Property);

//**************************************************************************
// Construction/Destruction

       
PropertyPath::PropertyPath()
{

}


PropertyPath::~PropertyPath()
{

}



//**************************************************************************
// Base class implementer


//**************************************************************************
// Seter getter for the property


void PropertyPath::setValue(const boost::filesystem::path &Path)
{
    aboutToSetValue();
    _cValue = Path;
    hasSetValue();
}

void PropertyPath::setValue(const char * Path)
{
    aboutToSetValue();
    _cValue = boost::filesystem::path(Path,boost::filesystem::no_check );
    //_cValue = boost::filesystem::path(Path,boost::filesystem::native );
    //_cValue = boost::filesystem::path(Path,boost::filesystem::windows_name );
    hasSetValue();
}

boost::filesystem::path PropertyPath::getValue(void) const
{
    return _cValue;
}

PyObject *PropertyPath::getPyObject(void)
{
    const std::string s = _cValue.string() ;
    std::string s2 = _cValue.native_file_string();
    std::string s3 = _cValue.native_directory_string();

    // decomposition functions:
    std::string  r = _cValue.root_name();
    std::string  d = _cValue.root_directory() ;
    std::string  l = _cValue.leaf();

    // Returns a new reference, don't increment it!
    PyObject *p = PyUnicode_DecodeUTF8(_cValue.native_file_string().c_str(),_cValue.native_file_string().size(),0);
    if(!p)
        throw Base::Exception("UTF8 conversion failure at PropertyString::getPyObject()");
    return p;

  //return Py_BuildValue("s", _cValue.c_str());
}

void PropertyPath::setPyObject(PyObject *value)
{
    if (PyUnicode_Check(value))
        value = PyUnicode_AsUTF8String(value);

    if (PyString_Check(value)) {
        aboutToSetValue();
        _cValue = PyString_AsString(value);
        hasSetValue();
    } else {
        std::string error = std::string("type must be str, not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}


void PropertyPath::Save (Writer &writer) const
{
    std::string val = encodeAttribute(_cValue.string());
    writer.Stream() << writer.ind() << "<Path value=\"" <<  val <<"\"/>" << std::endl;
}

void PropertyPath::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("Path");
    // get the value of my Attribute
    _cValue = reader.getAttribute("value");
}


Property *PropertyPath::Copy(void) const
{
    PropertyPath *p= new PropertyPath();
    p->_cValue = _cValue;
    return p;
}

void PropertyPath::Paste(const Property &from)
{
    aboutToSetValue();
    _cValue = dynamic_cast<const PropertyPath&>(from)._cValue;
    hasSetValue();
}


//**************************************************************************
//**************************************************************************
// PropertyEnumeration
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(PropertyEnumeration, App::PropertyInteger);

//**************************************************************************
// Construction/Destruction


PropertyEnumeration::PropertyEnumeration()
  : _EnumArray(0)
{

}


PropertyEnumeration::~PropertyEnumeration()
{

}

void PropertyEnumeration::setEnums(const char** plEnums)
{
    _EnumArray = plEnums;
# ifdef FC_DEBUG
    if (_EnumArray) {
        // check for NULL termination
        const char* p = *_EnumArray;
        unsigned int i=0;
        while(*(p++) != NULL)i++;
            // very unlikely to have enums with more then 5000 entries!
            assert(i<5000);
    }
# endif
}

void PropertyEnumeration::setValue(const char* value)
{
    // using string methods without set, use setEnums(const char** plEnums) first!
    assert(_EnumArray);

    // set zero if there is no enum array
    if(!_EnumArray){
        PropertyInteger::setValue(0);
        return;
    }

    unsigned int i=0;
    const char** plEnums = _EnumArray;

    // search for the right entry
    while(1){
        // end of list? set zero
        if(*plEnums==NULL){
            PropertyInteger::setValue(0);
            break;
        }
        if(strcmp(*plEnums,value)==0){
            PropertyInteger::setValue(i);
            break;
        }
        plEnums++;
        i++;
    }
}

void PropertyEnumeration::setValue(long value)
{
# ifdef FC_DEBUG
    assert(value>=0 && value<5000);
    if(_EnumArray){
        const char** plEnums = _EnumArray;
        long i=0;
        while(*(plEnums++) != NULL)i++;
        // very unlikely to have enums with more then 5000 entries!
        // Note: Do NOT call assert() because this code might be executed from Python console!
        if ( value < 0 || i <= value )
            throw Base::Exception("Out of range");
    }
# endif
    PropertyInteger::setValue(value);
}

/// checks if the property is set to a certain string value
bool PropertyEnumeration::isValue(const char* value) const
{
    // using string methods without set, use setEnums(const char** plEnums) first!
    assert(_EnumArray);
    return strcmp(_EnumArray[getValue()],value)==0;
}

/// checks if a string is included in the enumeration
bool PropertyEnumeration::isPartOf(const char* value) const
{
    // using string methods without set, use setEnums(const char** plEnums) first!
    assert(_EnumArray);

    const char** plEnums = _EnumArray;

    // search for the right entry
    while(1){
        // end of list?
        if(*plEnums==NULL) 
            return false;
        if(strcmp(*plEnums,value)==0) 
            return true;
        plEnums++;
    }
}

/// get the value as string
const char* PropertyEnumeration::getValueAsString(void) const
{
    // using string methods without set, use setEnums(const char** plEnums) first!
    assert(_EnumArray);
    return _EnumArray[getValue()];
}

std::vector<std::string> PropertyEnumeration::getEnumVector(void) const
{
    // using string methods without set, use setEnums(const char** plEnums) first!
    assert(_EnumArray);

    std::vector<std::string> result;
    const char** plEnums = _EnumArray;

    // end of list?
    while(*plEnums!=NULL){ 
        result.push_back(*plEnums);
        plEnums++;
    }

    return result;
}

const char** PropertyEnumeration::getEnums(void) const
{
    return _EnumArray;
}

PyObject *PropertyEnumeration::getPyObject(void)
{
    return Py_BuildValue("s", getValueAsString());
}

void PropertyEnumeration::setPyObject(PyObject *value)
{ 
    if (PyInt_Check(value)) {
        long val = PyInt_AsLong(value);
        if(_EnumArray){
            const char** plEnums = _EnumArray;
            long i=0;
            while(*(plEnums++) != NULL)i++;
            if (val < 0 || i <= val)
                throw Py::ValueError("Out of range");
            PropertyInteger::setValue(val);
        }
    }
    else if (PyString_Check(value)) {
        const char* str = PyString_AsString (value);
        if (isPartOf(str))
            setValue(PyString_AsString (value));
        else
            throw Py::ValueError("not a member of the enum");
    }
    else {
        std::string error = std::string("type must be int or str, not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

//**************************************************************************
//**************************************************************************
// PropertyIntegerConstraint
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyIntegerConstraint, App::PropertyInteger);

//**************************************************************************
// Construction/Destruction


PropertyIntegerConstraint::PropertyIntegerConstraint()
  : _ConstStruct(0)
{

}


PropertyIntegerConstraint::~PropertyIntegerConstraint()
{

}

void PropertyIntegerConstraint::setConstraints(const Constraints* sConstrain)
{
    _ConstStruct = sConstrain;
}

const PropertyIntegerConstraint::Constraints*  PropertyIntegerConstraint::getConstraints(void) const
{
    return _ConstStruct;
}

void PropertyIntegerConstraint::setPyObject(PyObject *value)
{ 
    if (PyInt_Check(value)) {
        long temp = PyInt_AsLong(value);
        if (_ConstStruct)
        {
            if(temp > _ConstStruct->UpperBound)
                temp = _ConstStruct->UpperBound;
            else if(temp < _ConstStruct->LowerBound)
                temp = _ConstStruct->LowerBound;
        }
        aboutToSetValue();
        _lValue = temp;
        hasSetValue();
    } 
    else {
        std::string error = std::string("type must be int, not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

//**************************************************************************
//**************************************************************************
// PropertyPercent
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyPercent , App::PropertyIntegerConstraint);

const PropertyIntegerConstraint::Constraints percent = {0,100,1};

//**************************************************************************
// Construction/Destruction


PropertyPercent::PropertyPercent()
{
    _ConstStruct = &percent;
}


PropertyPercent::~PropertyPercent()
{
}

//**************************************************************************
//**************************************************************************
// PropertyIntegerList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyIntegerList , App::PropertyLists);

//**************************************************************************
// Construction/Destruction


PropertyIntegerList::PropertyIntegerList()
{

}


PropertyIntegerList::~PropertyIntegerList()
{

}

//**************************************************************************
// Base class implementer


void PropertyIntegerList::setValue(long lValue)
{
    aboutToSetValue();
    _lValueList.resize(1);
    _lValueList[0]=lValue;
    hasSetValue();
}

void PropertyIntegerList::setValues(const std::vector<long>& values)
{
    aboutToSetValue();
    _lValueList = values;
    hasSetValue();
}

PyObject *PropertyIntegerList::getPyObject(void)
{
    PyObject* list = PyList_New(	getSize() );

    for(int i = 0;i<getSize(); i++)
        PyList_SetItem( list, i, PyInt_FromLong(	_lValueList[i]));

    return list;
}

void PropertyIntegerList::setPyObject(PyObject *value)
{ 
    if (PyList_Check(value)) {
        int nSize = PyList_Size(value);
        std::vector<long> values;
        values.resize(nSize);

        for (int i=0; i<nSize;++i) {
            PyObject* item = PyList_GetItem(value, i);
            if (!PyInt_Check(item)) {
                std::string error = std::string("type in list must be int, not ");
                error += item->ob_type->tp_name;
                throw Py::TypeError(error);
            }
            values[i] = PyInt_AsLong(item);
        }

        setValues(values);
    }
    else if (PyInt_Check(value)) {
        setValue(PyInt_AsLong(value));
    }
    else {
        std::string error = std::string("type must be int or list of int, not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

void PropertyIntegerList::Save (Writer &writer) const
{
    writer.Stream() << writer.ind() << "<IntegerList count=\"" <<  getSize() <<"\">" << endl;
    writer.incInd();
    for(int i = 0;i<getSize(); i++)
        writer.Stream() << writer.ind() << "<I v=\"" <<  _lValueList[i] <<"\"/>" << endl; ;
    writer.decInd();
    writer.Stream() << writer.ind() << "</IntegerList>" << endl ;
}

void PropertyIntegerList::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("IntegerList");
    // get the value of my Attribute
    int count = reader.getAttributeAsInteger("count");
    
    setSize(count);
    
    for(int i = 0;i<count; i++)
    {
        reader.readElement("I");
        _lValueList[i] = reader.getAttributeAsInteger("v");
    }
    
    reader.readEndElement("IntegerList");
}

Property *PropertyIntegerList::Copy(void) const
{
    PropertyIntegerList *p= new PropertyIntegerList();
    p->_lValueList = _lValueList;
    return p;
}

void PropertyIntegerList::Paste(const Property &from)
{
    aboutToSetValue();
    _lValueList = dynamic_cast<const PropertyIntegerList&>(from)._lValueList;
    hasSetValue();
}



//**************************************************************************
//**************************************************************************
// PropertyFloat
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyFloat , App::Property);

//**************************************************************************
// Construction/Destruction


PropertyFloat::PropertyFloat()
{

}


PropertyFloat::~PropertyFloat()
{

}

//**************************************************************************
// Base class implementer


void PropertyFloat::setValue(float lValue)
{
    aboutToSetValue();
    _dValue=lValue;
    hasSetValue();
}

float PropertyFloat::getValue(void) const
{
    return _dValue;
}

PyObject *PropertyFloat::getPyObject(void)
{
    return Py_BuildValue("d", _dValue);
}

void PropertyFloat::setPyObject(PyObject *value)
{
    if (PyFloat_Check(value)) {
        aboutToSetValue();
        _dValue = (float) PyFloat_AsDouble(value);
        hasSetValue();
    }
    else if(PyInt_Check(value)) {
        aboutToSetValue();
        _dValue = (float) PyInt_AsLong(value);
        hasSetValue();
    }
    else {
        std::string error = std::string("type must be float or int, not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

void PropertyFloat::Save (Writer &writer) const
{
    writer.Stream() << writer.ind() << "<Float value=\"" <<  _dValue <<"\"/>" << std::endl;
}

void PropertyFloat::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("Float");
    // get the value of my Attribute
    _dValue = (float) reader.getAttributeAsFloat("value");
}

Property *PropertyFloat::Copy(void) const
{
    PropertyFloat *p= new PropertyFloat();
    p->_dValue = _dValue;
    return p;
}

void PropertyFloat::Paste(const Property &from)
{
    aboutToSetValue();
    _dValue = dynamic_cast<const PropertyFloat&>(from)._dValue;
    hasSetValue();
}

//**************************************************************************
//**************************************************************************
// PropertyFloatConstraint
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(PropertyFloatConstraint, App::PropertyFloat);

//**************************************************************************
// Construction/Destruction


PropertyFloatConstraint::PropertyFloatConstraint()
  : _ConstStruct(0)
{

}


PropertyFloatConstraint::~PropertyFloatConstraint()
{

}

void PropertyFloatConstraint::setConstraints(const Constraints* sConstrain)
{
    _ConstStruct = sConstrain;
}

const PropertyFloatConstraint::Constraints*  PropertyFloatConstraint::getConstraints(void) const
{
    return _ConstStruct;
}

void PropertyFloatConstraint::setPyObject(PyObject *value)
{ 
    if (PyFloat_Check(value)) {
        float temp = (float)PyFloat_AsDouble(value);
        if (_ConstStruct) {
            if (temp > _ConstStruct->UpperBound)
                temp = _ConstStruct->UpperBound;
            else if (temp < _ConstStruct->LowerBound)
                temp = _ConstStruct->LowerBound;
        }
    
        aboutToSetValue();
        _dValue = temp;
        hasSetValue();
    } 
    else {
        std::string error = std::string("type must be float, not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

//**************************************************************************
//**************************************************************************
// PropertyDistance
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(PropertyDistance, App::PropertyFloat);

//**************************************************************************
//**************************************************************************
// PropertyAngle
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(PropertyAngle, App::PropertyFloat);


//**************************************************************************
// PropertyFloatList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyFloatList , App::PropertyLists);

//**************************************************************************
// Construction/Destruction


PropertyFloatList::PropertyFloatList()
{

}


PropertyFloatList::~PropertyFloatList()
{

}

//**************************************************************************
// Base class implementer


void PropertyFloatList::setValue(float lValue)
{
    aboutToSetValue();
    _lValueList.resize(1);
    _lValueList[0]=lValue;
    hasSetValue();
}

void PropertyFloatList::setValues(const std::vector<float>& values)
{
    aboutToSetValue();
    _lValueList = values;
    hasSetValue();
}

PyObject *PropertyFloatList::getPyObject(void)
{
    PyObject* list = PyList_New(	getSize() );
    
    for(int i = 0;i<getSize(); i++)
         PyList_SetItem( list, i, PyFloat_FromDouble(	_lValueList[i]));
    
    return list;
}

void PropertyFloatList::setPyObject(PyObject *value)
{ 
    if (PyList_Check(value)) {
        int nSize = PyList_Size(value);
        std::vector<float> values;
        values.resize(nSize);

        for (int i=0; i<nSize;++i) {
            PyObject* item = PyList_GetItem(value, i);
            if (!PyFloat_Check(item)) {
                std::string error = std::string("type in list must be float, not ");
                error += item->ob_type->tp_name;
                throw Py::TypeError(error);
            }
            
            values[i] = (float) PyFloat_AsDouble(item);
        }

        setValues(values);
    }
    else if (PyFloat_Check(value)) {
        setValue((float) PyFloat_AsDouble(value));
    } 
    else {
        std::string error = std::string("type must be float or list of float, not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

void PropertyFloatList::Save (Writer &writer) const
{
    if (writer.isForceXML()) {
        writer.Stream() << writer.ind() << "<FloatList count=\"" <<  getSize() <<"\">" << endl;
        writer.incInd();
        for(int i = 0;i<getSize(); i++)
            writer.Stream() << writer.ind() << "<F v=\"" <<  _lValueList[i] <<"\"/>" << endl; ;
        writer.decInd();
        writer.Stream() << writer.ind() <<"</FloatList>" << endl ;
    }
    else {
        writer.Stream() << writer.ind() << "<FloatList file=\"" << 
        writer.addFile(getName(), this) << "\"/>" << std::endl;
    }
}

void PropertyFloatList::Restore(Base::XMLReader &reader)
{
    reader.readElement("FloatList");
    string file (reader.getAttribute("file") );

    if(file == "") {
//  // read my Element
//  reader.readElement("FloatList");
//  // get the value of my Attribute
//  int count = reader.getAttributeAsInteger("count");
//
//  setSize(count);
//
//  for(int i = 0;i<count; i++)
//  {
//    reader.readElement("F");
//    _lValueList[i] = (float) reader.getAttributeAsFloat("v");
//  }
//
//  reader.readEndElement("FloatList");
    } else {
        // initate a file read
        reader.addFile(file.c_str(),this);
    }
}

void PropertyFloatList::SaveDocFile (Base::Writer &writer) const
{
    try {
        unsigned long uCt = getSize();
        writer.Stream().write((const char*)&uCt, sizeof(unsigned long));
        writer.Stream().write((const char*)&(_lValueList[0]), uCt*sizeof(float));
    } catch( const Base::Exception& e) {
        throw e;
    }
}

void PropertyFloatList::RestoreDocFile(Base::Reader &reader)
{
    try {
        _lValueList.clear();
        unsigned long uCt=ULONG_MAX;
        reader.read((char*)&uCt, sizeof(unsigned long));
        _lValueList.resize(uCt);
        reader.read((char*)&(_lValueList[0]), uCt*sizeof(float));
    } catch( const Base::Exception& e) {
        throw e;
    }
}

Property *PropertyFloatList::Copy(void) const
{
    PropertyFloatList *p= new PropertyFloatList();
    p->_lValueList = _lValueList;
    return p;
}

void PropertyFloatList::Paste(const Property &from)
{
    aboutToSetValue();
    _lValueList = dynamic_cast<const PropertyFloatList&>(from)._lValueList;
    hasSetValue();
}


//**************************************************************************
//**************************************************************************
// PropertyString
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyString , App::Property);

//**************************************************************************
// Construction/Destruction

       
PropertyString::PropertyString()
{

}


PropertyString::~PropertyString()
{

}



//**************************************************************************
// Base class implementer


//**************************************************************************
// Seter getter for the property


void PropertyString::setValue(const char* sString)
{
    if (sString) {
        aboutToSetValue();
        _cValue = sString;
        hasSetValue();
    }
}

void PropertyString::setValue(const std::string &sString)
{
    aboutToSetValue();
    _cValue = sString;
    hasSetValue();
}

const char* PropertyString::getValue(void) const
{
    return _cValue.c_str();
}

PyObject *PropertyString::getPyObject(void)
{
     PyObject *p = PyUnicode_DecodeUTF8(_cValue.c_str(),_cValue.size(),0);
    if (!p)
        throw Base::Exception("UTF8 conversion failure at PropertyString::getPyObject()");
    return p;
}

void PropertyString::setPyObject(PyObject *value)
{
    if(PyUnicode_Check( value ))
        value = PyUnicode_AsUTF8String(value);

    if (PyString_Check(value)) {
        aboutToSetValue();
        _cValue = PyString_AsString(value);
        hasSetValue();
    }
    else {
        std::string error = std::string("type must be string, not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

void PropertyString::Save (Writer &writer) const
{
    std::string val = encodeAttribute(_cValue);
    writer.Stream() << writer.ind() << "<String value=\"" <<  val <<"\"/>" << std::endl;
}

void PropertyString::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("String");
    // get the value of my Attribute
    _cValue = reader.getAttribute("value");
}


Property *PropertyString::Copy(void) const
{
    PropertyString *p= new PropertyString();
    p->_cValue = _cValue;
    return p;
}

void PropertyString::Paste(const Property &from)
{
    aboutToSetValue();
    _cValue = dynamic_cast<const PropertyString&>(from)._cValue;
    hasSetValue();
}


//**************************************************************************
// PropertyStringList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyStringList , App::PropertyLists);

//**************************************************************************
// Construction/Destruction

PropertyStringList::PropertyStringList()
{

}

PropertyStringList::~PropertyStringList()
{

}

//**************************************************************************
// Base class implementer

void PropertyStringList::setValue(const std::string& lValue)
{
    aboutToSetValue();
    _lValueList.resize(1);
    _lValueList[0]=lValue;
    hasSetValue();
}

void PropertyStringList::setValues(const std::vector<std::string>& lValue)
{
    aboutToSetValue();
    _lValueList=lValue;
    hasSetValue();
}

void PropertyStringList::setValues(const std::list<std::string>& lValue)
{
    aboutToSetValue();
    _lValueList.resize(lValue.size());
    std::copy(lValue.begin(), lValue.end(), _lValueList.begin());
    hasSetValue();
}

PyObject *PropertyStringList::getPyObject(void)
{
    PyObject* list = PyList_New(getSize());

    for(int i = 0;i<getSize(); i++)
    {
        PyObject* item = PyString_FromString(_lValueList[i].c_str());
        PyList_SetItem( list, i, item );
    }

    return list;
}

void PropertyStringList::setPyObject(PyObject *value)
{
    if (PyList_Check(value)) {
        int nSize = PyList_Size(value);
        std::vector<std::string> values;
        values.resize(nSize);

        for (int i=0; i<nSize;++i) {
            PyObject* item = PyList_GetItem(value, i);
            if (!PyString_Check(item)) {
                std::string error = std::string("type in list must be str, not ");
                error += item->ob_type->tp_name;
                throw Py::TypeError(error);
            }

            values[i] = PyString_AsString(item);
        }
        
        setValues(values);
    }
    else if (PyString_Check(value)) {
        setValue(PyString_AsString(value));
    }
    else {
        std::string error = std::string("type must be str or list of str, not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

unsigned int PropertyStringList::getMemSize (void) const
{
    unsigned int size=0;
    for(int i = 0;i<getSize(); i++) 
        size += _lValueList[i].size();
    return size;
}

void PropertyStringList::Save (Writer &writer) const
{
    writer.Stream() << writer.ind() << "<StringList count=\"" <<  getSize() <<"\">" << endl;
    writer.incInd();
    for(int i = 0;i<getSize(); i++) {
        std::string val = encodeAttribute(_lValueList[i]);
        writer.Stream() << writer.ind() << "<String value=\"" <<  val <<"\"/>" << endl;
    }
    writer.decInd();
    writer.Stream() << writer.ind() << "</StringList>" << endl ;
}

void PropertyStringList::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("StringList");
    // get the value of my Attribute
    int count = reader.getAttributeAsInteger("count");
    
    setSize(count);
    
    for(int i = 0;i<count; i++) {
        reader.readElement("String");
        _lValueList[i] = reader.getAttribute("value");
    }
    
    reader.readEndElement("StringList");
}

Property *PropertyStringList::Copy(void) const
{
    PropertyStringList *p= new PropertyStringList();
    p->_lValueList = _lValueList;
    return p;
}

void PropertyStringList::Paste(const Property &from)
{
    aboutToSetValue();
    _lValueList = dynamic_cast<const PropertyStringList&>(from)._lValueList;
    hasSetValue();
}

//**************************************************************************
//**************************************************************************
// PropertyBool
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyBool , App::Property);

//**************************************************************************
// Construction/Destruction

       
PropertyBool::PropertyBool()
{

}


PropertyBool::~PropertyBool()
{

}



//**************************************************************************
// Seter getter for the property

void PropertyBool::setValue(bool lValue)
{
    aboutToSetValue();
    _lValue=lValue;
    hasSetValue();
}

bool PropertyBool::getValue(void) const
{
    return _lValue;
}

PyObject *PropertyBool::getPyObject(void)
{
    if (_lValue)
        {Py_INCREF(Py_True); return Py_True;}
    else
        {Py_INCREF(Py_False); return Py_False;}

}

void PropertyBool::setPyObject(PyObject *value)
{
    if (PyBool_Check(value))
        setValue(value == Py_True);
    else if(PyInt_Check(value))
        setValue(PyInt_AsLong(value)!=0);
    else {
        std::string error = std::string("type must be bool, not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

void PropertyBool::Save (Writer &writer) const
{
    writer.Stream() << writer.ind() << "<Bool value=\"" ;
    if (_lValue)
        writer.Stream() << "true" <<"\"/>" ;
    else
        writer.Stream() << "false" <<"\"/>" ;
    writer.Stream() << std::endl;
}

void PropertyBool::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("Bool");
    // get the value of my Attribute
    string b = reader.getAttribute("value");
    if (b == "true")
        _lValue = true;
    else
        _lValue = false;
}


Property *PropertyBool::Copy(void) const
{
    PropertyBool *p= new PropertyBool();
    p->_lValue = _lValue;
    return p;
}

void PropertyBool::Paste(const Property &from)
{
    aboutToSetValue();
    _lValue = dynamic_cast<const PropertyBool&>(from)._lValue;
    hasSetValue();
}



//**************************************************************************
//**************************************************************************
// PropertyColor
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyColor , App::Property);

//**************************************************************************
// Construction/Destruction


PropertyColor::PropertyColor()
{

}


PropertyColor::~PropertyColor()
{

}

//**************************************************************************
// Base class implementer


void PropertyColor::setValue(const Color &col)
{
    aboutToSetValue();
    _cCol=col;
    hasSetValue();
}

void PropertyColor::setValue(float r, float g, float b, float a)
{
    aboutToSetValue();
    _cCol.set(r,g,b,a);
    hasSetValue();
}

const Color& PropertyColor::getValue(void) const 
{
    return _cCol;
}

PyObject *PropertyColor::getPyObject(void)
{
    PyObject* rgba = PyTuple_New(4);
    PyObject* r = PyFloat_FromDouble(_cCol.r);
    PyObject* g = PyFloat_FromDouble(_cCol.g);
    PyObject* b = PyFloat_FromDouble(_cCol.b);
    PyObject* a = PyFloat_FromDouble(_cCol.a);

    PyTuple_SetItem(rgba, 0, r);
    PyTuple_SetItem(rgba, 1, g);
    PyTuple_SetItem(rgba, 2, b);
    PyTuple_SetItem(rgba, 3, a);

    return rgba;
}

void PropertyColor::setPyObject(PyObject *value)
{
    App::Color cCol;
    if (PyTuple_Check(value) && PyTuple_Size(value) == 3) {
        PyObject* item;
        item = PyTuple_GetItem(value,0);
        if (PyFloat_Check(item))
            cCol.r = (float)PyFloat_AsDouble(item);
        else
            throw Base::Exception("Type in tuple must be float");
        item = PyTuple_GetItem(value,1);
        if (PyFloat_Check(item))
            cCol.g = (float)PyFloat_AsDouble(item);
        else
            throw Base::Exception("Type in tuple must be float");
        item = PyTuple_GetItem(value,2);
        if (PyFloat_Check(item))
            cCol.b = (float)PyFloat_AsDouble(item);
        else
            throw Base::Exception("Type in tuple must be float");
    }
    else if (PyTuple_Check(value) && PyTuple_Size(value) == 4) {
        PyObject* item;
        item = PyTuple_GetItem(value,0);
        if (PyFloat_Check(item))
            cCol.r = (float)PyFloat_AsDouble(item);
        else
            throw Base::Exception("Type in tuple must be float");
        item = PyTuple_GetItem(value,1);
        if (PyFloat_Check(item))
            cCol.g = (float)PyFloat_AsDouble(item);
        else
            throw Base::Exception("Type in tuple must be float");
        item = PyTuple_GetItem(value,2);
        if (PyFloat_Check(item))
            cCol.b = (float)PyFloat_AsDouble(item);
        else
            throw Base::Exception("Type in tuple must be float");
        item = PyTuple_GetItem(value,3);
        if (PyFloat_Check(item))
            cCol.a = (float)PyFloat_AsDouble(item);
        else
            throw Base::Exception("Type in tuple must be float");
    }
    else if (PyInt_Check(value)) {
        cCol.setPackedValue(PyInt_AsLong(value));
    }
    else {
        std::string error = std::string("type must be int or tuple of float, not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }

    setValue( cCol );
}

void PropertyColor::Save (Writer &writer) const
{
    writer.Stream() << writer.ind() << "<PropertyColor value=\"" 
    <<  _cCol.getPackedValue() <<"\"/>" << endl;
}

void PropertyColor::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("PropertyColor");
    // get the value of my Attribute
    unsigned long rgba = reader.getAttributeAsUnsigned("value");
    _cCol.setPackedValue(rgba);
}


Property *PropertyColor::Copy(void) const
{
    PropertyColor *p= new PropertyColor();
    p->_cCol = _cCol;
    return p;
}

void PropertyColor::Paste(const Property &from)
{
    aboutToSetValue();
    _cCol = dynamic_cast<const PropertyColor&>(from)._cCol;
    hasSetValue();
}



//**************************************************************************
// PropertyColorList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyColorList , App::PropertyLists);

//**************************************************************************
// Construction/Destruction

PropertyColorList::PropertyColorList()
{

}

PropertyColorList::~PropertyColorList()
{

}

//**************************************************************************
// Base class implementer

void PropertyColorList::setValue(const Color& lValue)
{
    aboutToSetValue();
    _lValueList.resize(1);
    _lValueList[0]=lValue;
    hasSetValue();
}

void PropertyColorList::setValues (const std::vector<Color>& values)
{
    aboutToSetValue();
    _lValueList=values;
    hasSetValue();
}

PyObject *PropertyColorList::getPyObject(void)
{
    PyObject* list = PyList_New(getSize());

    for(int i = 0;i<getSize(); i++) {
        PyObject* rgba = PyTuple_New(4);
        PyObject* r = PyFloat_FromDouble(_lValueList[i].r);
        PyObject* g = PyFloat_FromDouble(_lValueList[i].g);
        PyObject* b = PyFloat_FromDouble(_lValueList[i].b);
        PyObject* a = PyFloat_FromDouble(_lValueList[i].a);

        PyTuple_SetItem(rgba, 0, r);
        PyTuple_SetItem(rgba, 1, g);
        PyTuple_SetItem(rgba, 2, b);
        PyTuple_SetItem(rgba, 3, a);

        PyList_SetItem( list, i, rgba );
    }

    return list;
}

void PropertyColorList::setPyObject(PyObject *value)
{
    if (PyList_Check(value)) {
        int nSize = PyList_Size(value);
        std::vector<Color> values;
        values.resize(nSize);

        for (int i=0; i<nSize;++i) {
            PyObject* item = PyList_GetItem(value, i);
            PropertyColor col;
            col.setPyObject(item);
            values[i] = col.getValue();
        }

        setValues(values);
    }
    else if (PyTuple_Check(value) && PyTuple_Size(value) == 3) {
        PropertyColor col;
        col.setPyObject( value );
        setValue( col.getValue() );
    }
    else if (PyTuple_Check(value) && PyTuple_Size(value) == 4) {
        PropertyColor col;
        col.setPyObject( value );
        setValue( col.getValue() );
    }
    else {
        std::string error = std::string("not allowed type, ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

void PropertyColorList::Save (Writer &writer) const
{
    if (writer.isForceXML()) {
//    writer << "<ColorList count=\"" <<  getSize() <<"\"/>" << endl;
//    for(int i = 0;i<getSize(); i++)
//      writer << writer.ind() << "<PropertyColor value=\"" <<  _lValueList[i].getPackedValue() <<"\"/>" << endl;
//    writer << "</ColorList>" << endl ;
    } else {
        writer.Stream() << writer.ind() << "<ColorList file=\"" << writer.addFile(getName(), this) << "\"/>" << std::endl;
    }
}

void PropertyColorList::Restore(Base::XMLReader &reader)
{
    reader.readElement("ColorList");
    std::string file (reader.getAttribute("file") );

    if (file == "") {
//  // read my Element
//  reader.readElement("ColorList");
//  // get the value of my Attribute
//  int count = reader.getAttributeAsInteger("count");
//
//  setSize(count);
//
//  for(int i = 0;i<count; i++)
//  {
//    reader.readElement("PropertyColor");
//    _lValueList[i].setPackedValue( reader.getAttributeAsInteger("value") );
//  }
//
//  reader.readEndElement("ColorList");
    } else {
        // initate a file read
        reader.addFile(file.c_str(),this);
    }
}

void PropertyColorList::SaveDocFile (Base::Writer &writer) const
{
    try {
        unsigned long uCt = getSize();
        writer.Stream().write((const char*)&uCt, sizeof(unsigned long));
        writer.Stream().write((const char*)&(_lValueList[0]), uCt*sizeof(Color));
    } catch( const Base::Exception& e) {
        throw e;
    }
}

void PropertyColorList::RestoreDocFile(Base::Reader &reader)
{
    try {
        _lValueList.clear();
        unsigned long uCt=ULONG_MAX;
        reader.read((char*)&uCt, sizeof(unsigned long));
        _lValueList.resize(uCt);
        reader.read((char*)&(_lValueList[0]), uCt*sizeof(Color));
    } catch( const Base::Exception& e) {
        throw e;
    }
}



Property *PropertyColorList::Copy(void) const
{
    PropertyColorList *p= new PropertyColorList();
    p->_lValueList = _lValueList;
    return p;
}

void PropertyColorList::Paste(const Property &from)
{
    aboutToSetValue();
    _lValueList = dynamic_cast<const PropertyColorList&>(from)._lValueList;
    hasSetValue();
}


//**************************************************************************
//**************************************************************************
// PropertyMaterial
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyMaterial , App::Property);


PropertyMaterial::PropertyMaterial()
{

}

PropertyMaterial::~PropertyMaterial()
{

}

void PropertyMaterial::setValue(const Material &mat)
{
    aboutToSetValue();
    _cMat=mat;
    hasSetValue();
}

const Material& PropertyMaterial::getValue(void) const 
{
    return _cMat;
}

void PropertyMaterial::setAmbientColor(const Color& col)
{
    aboutToSetValue();
    _cMat.ambientColor = col;
    hasSetValue();
}

void PropertyMaterial::setDiffuseColor(const Color& col)
{
    aboutToSetValue();
    _cMat.diffuseColor = col;
    hasSetValue();
}

void PropertyMaterial::setSpecularColor(const Color& col)
{
    aboutToSetValue();
    _cMat.specularColor = col;
    hasSetValue();
}

void PropertyMaterial::setEmmisiveColor(const Color& col)
{
    aboutToSetValue();
    _cMat.emissiveColor = col;
    hasSetValue();
}

void PropertyMaterial::setShininess(float val)
{
    aboutToSetValue();
    _cMat.shininess = val;
    hasSetValue();
}

void PropertyMaterial::setTransparency(float val)
{
    aboutToSetValue();
    _cMat.transparency = val;
    hasSetValue();
}

PyObject *PropertyMaterial::getPyObject(void)
{
    return new MaterialPy(&_cMat);
}

void PropertyMaterial::setPyObject(PyObject *value)
{
    if (PyObject_TypeCheck(value, &(MaterialPy::Type))) {
        MaterialPy  *pcObject = (MaterialPy*)value;
        setValue( *pcObject->_pcMaterial );
    }
    else {
        std::string error = std::string("type must be 'Material', not ");
        error += value->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

void PropertyMaterial::Save (Writer &writer) const
{
    writer.Stream() << writer.ind() << "<PropertyMaterial ambientColor=\"" 
        <<  _cMat.ambientColor.getPackedValue() 
        << "\" diffuseColor=\"" <<  _cMat.diffuseColor.getPackedValue() 
        << "\" specularColor=\"" <<  _cMat.specularColor.getPackedValue()
        << "\" emissiveColor=\"" <<  _cMat.emissiveColor.getPackedValue()
        << "\" shininess=\"" <<  _cMat.shininess << "\" transparency=\"" 
        <<  _cMat.transparency << "\"/>" << endl;
}

void PropertyMaterial::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("PropertyMaterial");
    // get the value of my Attribute
    _cMat.ambientColor.setPackedValue(reader.getAttributeAsInteger("ambientColor"));
    _cMat.diffuseColor.setPackedValue(reader.getAttributeAsInteger("diffuseColor"));
    _cMat.specularColor.setPackedValue(reader.getAttributeAsInteger("specularColor"));
    _cMat.emissiveColor.setPackedValue(reader.getAttributeAsInteger("emissiveColor"));
    _cMat.shininess = (float)reader.getAttributeAsInteger("shininess");
    _cMat.transparency = (float)reader.getAttributeAsFloat("transparency");
}

Property *PropertyMaterial::Copy(void) const
{
    PropertyMaterial *p= new PropertyMaterial();
    p->_cMat = _cMat;
    return p;
}

void PropertyMaterial::Paste(const Property &from)
{
    aboutToSetValue();
    _cMat = dynamic_cast<const PropertyMaterial&>(from)._cMat;
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
