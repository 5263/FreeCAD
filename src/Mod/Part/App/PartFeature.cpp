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
#	include <TDF_Label.hxx>
#	include <TDF_ChildIterator.hxx>
#	include <TDF_Tool.hxx>
#	include <TCollection_AsciiString.hxx>
#	include <TDF_ListIteratorOfAttributeList.hxx>
#	include <TFunction_Logbook.hxx>
#	include <TFunction_DriverTable.hxx>
#	include <TFunction_Function.hxx>
# include <TNaming_Builder.hxx>
# include <TNaming_NamedShape.hxx>
# include <TopoDS_Shape.hxx>
#	include <Standard_GUID.hxx>
#endif

#include <Base/Console.h>
#include <Base/Exception.h>
using Base::Console;


#include "PartFeature.h"
//#include "Function.h"
//#include "Property.h"
//#include "PropertyAttr.h"

using namespace Part;


//===========================================================================
// Feature
//===========================================================================

PartFeature::PartFeature(void)
{

}
	
void PartFeature::SetShape(TopoDS_Shape &Shape)
{
 	TNaming_Builder B(_cFeatureLabel);
	B.Generated(Shape);
}

TopoDS_Shape PartFeature::GetShape(void)
{
  Handle(TNaming_NamedShape) ShapeToViewName;
  if (!( _cFeatureLabel.FindAttribute(TNaming_NamedShape::GetID(),ShapeToViewName) ))
    throw;

  // Now, let's get the TopoDS_Shape of these TNaming_NamedShape:
  return ShapeToViewName->Get();

}



#ifdef false
//===========================================================================
// FeaturePy - Python wrapper 
//===========================================================================

// The DocTypeStd python class 
class AppExport FeaturePy :public Base::PyObjectBase
{
	/// always start with Py_Header
	Py_Header;

public:
	FeaturePy(Feature *pcFeature, PyTypeObject *T = &Type);
	static PyObject *PyMake(PyObject *, PyObject *);

	~FeaturePy();

	//---------------------------------------------------------------------
	// python exports goes here +++++++++++++++++++++++++++++++++++++++++++	
	//---------------------------------------------------------------------

	virtual PyObject *_repr(void);  				// the representation
	PyObject *_getattr(char *attr);					// __getattr__ function
	int _setattr(char *attr, PyObject *value);		// __setattr__ function
	PYFUNCDEF_D(FeaturePy,AddFeature)


private:
  Feature *_pcFeature;

};

//--------------------------------------------------------------------------
// Type structure
//--------------------------------------------------------------------------

PyTypeObject FeaturePy::Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,						/*ob_size*/
	"FeaturePy",				/*tp_name*/
	sizeof(FeaturePy),			/*tp_basicsize*/
	0,						/*tp_itemsize*/
	/* methods */
	PyDestructor,	  		/*tp_dealloc*/
	0,			 			/*tp_print*/
	__getattr, 				/*tp_getattr*/
	__setattr, 				/*tp_setattr*/
	0,						/*tp_compare*/
	__repr,					/*tp_repr*/
	0,						/*tp_as_number*/
	0,						/*tp_as_sequence*/
	0,						/*tp_as_mapping*/
	0,						/*tp_hash*/
	0,						/*tp_call */
};

//--------------------------------------------------------------------------
// Methods structure
//--------------------------------------------------------------------------
PyMethodDef FeaturePy::Methods[] = {
//  {"Undo",         (PyCFunction) sPyUndo,         Py_NEWARGS},
	PYMETHODEDEF(AddFeature)

	{NULL, NULL}		/* Sentinel */
};

//--------------------------------------------------------------------------
// Parents structure
//--------------------------------------------------------------------------
PyParentObject FeaturePy::Parents[] = {&PyObjectBase::Type, NULL};     

//--------------------------------------------------------------------------
//t constructor
//--------------------------------------------------------------------------
FeaturePy::FeaturePy(Feature *pcFeature, PyTypeObject *T)
: PyObjectBase( T), _pcFeature(pcFeature)
{
	Base::Console().Log("Create FeaturePy: %p \n",this);
}

PyObject *FeaturePy::PyMake(PyObject *ignored, PyObject *args)	// Python wrapper
{
  //return new FeaturePy(name, n, tau, gamma);			// Make new Python-able object
	return 0;
}

Base::PyObjectBase *Feature::GetPyObject(void)
{
	return new FeaturePy(this);
}


//--------------------------------------------------------------------------
// destructor 
//--------------------------------------------------------------------------
FeaturePy::~FeaturePy()						// Everything handled in parent
{
	Base::Console().Log("Destroy FeaturePy: %p \n",this);
} 


//--------------------------------------------------------------------------
// FeaturePy representation
//--------------------------------------------------------------------------
PyObject *FeaturePy::_repr(void)
{
  std::stringstream a;
  a << "Feature: [ ";
  for(std::map<std::string,int>::const_iterator It = _pcFeature->_PropertiesMap.begin();It!=_pcFeature->_PropertiesMap.end();It++)
  {
    a << It->first << "=" << _pcFeature->GetProperty(It->first.c_str()).GetAsString() << "; ";
  }
  a << "]" << std::endl;
	return Py_BuildValue("s", a.str().c_str());
}
//--------------------------------------------------------------------------
// FeaturePy Attributes
//--------------------------------------------------------------------------
PyObject *FeaturePy::_getattr(char *attr)				// __getattr__ function: note only need to handle new state
{ 
	try{
		if (Base::streq(attr, "XXXX"))						
			return Py_BuildValue("i",1); 
		else
      // search in PropertyList
      if( _pcFeature->_PropertiesMap.find(attr) != _pcFeature->_PropertiesMap.end())
        return Py_BuildValue("s", _pcFeature->GetProperty(attr).GetAsString());
      else
			  _getattr_up(PyObjectBase); 						
	}catch(...){
		Py_Error(PyExc_Exception,"Error in get Attribute");
	}
} 

int FeaturePy::_setattr(char *attr, PyObject *value) 	// __setattr__ function: note only need to handle new state
{ 
	if (Base::streq(attr, "XXXX")){						// settable new state
		//_pcDoc->SetUndoLimit(PyInt_AsLong(value)); 
		return 1;
  }else
      // search in PropertyList
      if( _pcFeature->_PropertiesMap.find(attr) != _pcFeature->_PropertiesMap.end()){
        try{
          //char sBuf[256];
          //sprintf(sBuf,"%f",PyFloat_AsDouble(value));
          //_pcFeature->GetProperty(attr).Set(sBuf);
          (dynamic_cast<PropertyFloat&>(_pcFeature->GetProperty(attr))).SetValue(PyFloat_AsDouble(value));
        }catch(...){
          return 1;
        }
        return 0;
      }else
			  return PyObjectBase::_setattr(attr, value); 						
} 



//--------------------------------------------------------------------------
// Python wrappers
//--------------------------------------------------------------------------

PYFUNCIMP_D(FeaturePy,AddFeature)
{
	//char *pstr;
    //if (!PyArg_ParseTuple(args, "s", &pstr))     // convert args: Python->C 
    //    return NULL;                             // NULL triggers exception 

	Py_Error(PyExc_Exception,"Not implemented!");
}

//===========================================================================
// FeatureFactorySingleton - Factory for Features
//===========================================================================



FeatureFactorySingleton* FeatureFactorySingleton::_pcSingleton = NULL;

FeatureFactorySingleton& FeatureFactorySingleton::Instance(void)
{
  if (_pcSingleton == NULL)
    _pcSingleton = new FeatureFactorySingleton;
  return *_pcSingleton;
}

void FeatureFactorySingleton::Destruct (void)
{
  if (_pcSingleton != NULL)
    delete _pcSingleton;
}

Feature* FeatureFactorySingleton::Produce (const char* sName) const
{
	Feature* w = (Feature*)Factory::Produce(sName);

  // this Feature class is not registered
  if (!w)
  {
#ifdef FC_DEBUG
    Console().Warning("\"%s\" is not registered\n", sName);
#else
    Console().Log("\"%s\" is not registered\n", sName);
#endif
    return NULL;
  }

  return w;
}

#endif
