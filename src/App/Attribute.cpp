/** \file $RCSfile$
 *  \brief The attribute module
 *  \author $Author$
 *  \version $Revision$
 *  \date    $Date$
 */

#include "../Config.h"
#ifdef _PreComp_
#	include "PreCompiled.h"
#else
#	include <TDF_Label.hxx>
#	include <TDF_ChildIterator.hxx>
#	include <TDF_Tool.hxx>
#	include <TCollection_AsciiString.hxx>
#	include <TDF_ListIteratorOfAttributeList.hxx>
#	include <Standard_GUID.hxx>
#endif

#include "Attribute.h"


//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& TDataStd_Name::GetID () 
{
  static Standard_GUID TDataStd_NameID("2a96b608-ec8b-11d0-bee7-080009dc3333");
  return TDataStd_NameID;
}

//=======================================================================
//function : Set
//purpose  : 
//=======================================================================
Handle(TDataStd_Name) TDataStd_Name::Set(const TDF_Label& label,const TCollection_ExtendedString& string) 
{
  Handle(TDataStd_Name) N;
  if (!label.FindAttribute(TDataStd_Name::GetID(), N)) { 
    N = new TDataStd_Name ();   
    label.AddAttribute(N);
  }
  N->Set(string);    
  return N;  
}

TDataStd_Name::TDataStd_Name () {}

void TDataStd_Name::Set (const TCollection_ExtendedString& S) 
{
 
  Backup();
  myString = S;
  //TCollection_ExtendedString tmpS(S);
  //tmpS.RemoveAll(':');
  //myString = tmpS;
  //myEmpty = Standard_False;
}

TCollection_ExtendedString TDataStd_Name::Get () const {return myString;}

const Standard_GUID& TDataStd_Name::ID () const { return GetID(); }


Handle(TDF_Attribute) TDataStd_Name::NewEmpty () const
{  
  return new TDataStd_Name(); 
}

void TDataStd_Name::Restore(const Handle(TDF_Attribute)& with) 
{
  myString = Handle(TDataStd_Name)::DownCast (with)->Get();
}


void TDataStd_Name::Paste (const Handle(TDF_Attribute)& into,
		           const Handle(TDF_RelocationTable)& RT) const
{
  Handle(TDataStd_Name)::DownCast (into)->Set (myString);
}


Standard_OStream& TDataStd_Name::Dump (Standard_OStream& anOS) const
{
  TDF_Attribute::Dump(anOS);
  anOS << " Name=|"<<myString<<"|"<<endl;
  return anOS;
}




//**************************************************************************
//**************************************************************************
// FCPyParametrGrp
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//--------------------------------------------------------------------------
// Type structure
//--------------------------------------------------------------------------
 
PyTypeObject FCPyAttribute::Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,						/*ob_size*/
	"Attribute",		    /*tp_name*/
	sizeof(FCPyAttribute),  /*tp_basicsize*/
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
PyMethodDef FCPyAttribute::Methods[] = {
  {"GetId",            (PyCFunction) sPyGetId,           Py_NEWARGS},

  {NULL, NULL}		/* Sentinel */
};

//--------------------------------------------------------------------------
// Parents structure
//--------------------------------------------------------------------------
PyParentObject FCPyAttribute::Parents[] = {&FCPyObject::Type,&FCPyAttribute::Type, NULL};     

//--------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------
FCPyAttribute::FCPyAttribute(const Handle(TDF_Attribute) &hAttribute, PyTypeObject *T ) 
	:FCPyObject(T),_hAttribute(hAttribute)
{
	//GetConsole().Log("Create Param Group %p\n",this);
}

PyObject *FCPyAttribute::PyMake(PyObject *ignored, PyObject *args)	// Python wrapper
{
  //return new FCPyParameterGrp();			// Make new Python-able object
  return 0;
}

//--------------------------------------------------------------------------
//  FCPyParametrGrp destructor 
//--------------------------------------------------------------------------
FCPyAttribute::~FCPyAttribute()						// Everything handled in parent
{
	//GetConsole().Log("Destroy ParameterGrp %p\n",this);
} 

//--------------------------------------------------------------------------
// FCPyParametrGrp Attributes
//--------------------------------------------------------------------------
PyObject *FCPyAttribute::_getattr(char *attr)				// __getattr__ function: note only need to handle new state
{ 
	/*
	try{
		// Access the number of attributes at this FCPyParametrGrp
		if (streq(attr, "AttributeCount"))						
			return Py_BuildValue("i", _cLabel.NbAttributes()); 
		else if (streq(attr, "ChildrenCount"))					
			return Py_BuildValue("i", _cLabel.NbChildren()); 
		else if (streq(attr, "Root"))						
			return _pcDocument->HasPyLabel( _cLabel.Root()); 
		else if (streq(attr, "Father"))						
			return _pcDocument->HasPyLabel( _cLabel.Father()); 
		else if (streq(attr, "Real")){
			Handle(TDataStd_Real) RealAttr;
			if(_cLabel.FindAttribute(TDataStd_Real::GetID(),RealAttr))
				return Py_BuildValue("d",RealAttr->Get()); 
			else
				return 0;
		}else if (streq(attr, "Int")){
			Handle(TDataStd_Integer) IntAttr;
			if(_cLabel.FindAttribute(TDataStd_Integer::GetID(),IntAttr))
				return Py_BuildValue("d",IntAttr->Get()); 
			else
				return 0;
		}else if (streq(attr, "Name")){
			Handle(TDataStd_Name) NameAttr;
			if(_cLabel.FindAttribute(TDataStd_Name::GetID(),NameAttr))
				//return Py_BuildValue("u",NameAttr->Get().ToExtString()); 
#ifdef __linux // u is unicode as ToExtString is!
				return Py_BuildValue("u",NameAttr->Get().ToExtString()); 
#else
				return Py_BuildValue("s",NameAttr->Get()); 
#endif				
			else
				return 0;
		}else*/
			_getattr_up(FCPyObject); 						// send to parent
/*	}catch(...){
		GetConsole().Log("Exception in FCPyParametrGrp::_getattr()\n");
		return 0;
	}*/
		return 0;
} 

int FCPyAttribute::_setattr(char *attr, PyObject *value) 	// __setattr__ function: note only need to handle new state
{ /*
	if (streq(attr, "Real"))						// settable new state
		TDataStd_Real::Set(_cLabel, PyFloat_AsDouble(value)); 
	else if (streq(attr, "Int"))						// settable new state
		TDataStd_Integer::Set(_cLabel, PyInt_AsLong(value)); 
	else if (streq(attr, "Name"))						// settable new state
		TDataStd_Name::Set(_cLabel, (short*)PyString_AsString(value)); 
		//TDataStd_Name::Set(_cLabel, (short*)PyUnicode_AsUnicode(value)); 
	else  
 */
		return FCPyObject::_setattr(attr, value);	// send up to parent
	return 0;
} 


//--------------------------------------------------------------------------
// Python wrappers
//--------------------------------------------------------------------------

PyObject *FCPyAttribute::PyGetId(PyObject *args)
{ 
	{
		PyErr_SetString(PyExc_IOError, "GetGroup failed");
		return 0L;
	}
} 



