
// This file is generated by src/Tools/generateTemaplates/templateClassPyExport.py out of the .XML file
// Every change you make here get lost at the next full rebuild!
// This File is normaly build as an include in LinePyImp.cpp! Its not intended to be in a project!

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <Base/PyObjectBase.h>
#include <Base/Console.h>
#include <Base/Exception.h>
#include <Base/PyCXX/Objects.hxx>

#define new DEBUG_CLIENTBLOCK

using Base::streq;
using namespace Part;

/// Type structure of LinePy
PyTypeObject LinePy::Type = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                                /*ob_size*/
    "GeomLine",                             /*tp_name*/
    sizeof(LinePy),                       /*tp_basicsize*/
    0,                                                /*tp_itemsize*/
    /* methods */
    PyDestructor,                                     /*tp_dealloc*/
    0,                                                /*tp_print*/
    __getattr,                                        /*tp_getattr*/
    __setattr,                                        /*tp_setattr*/
    0,                                                /*tp_compare*/
    __repr,                                           /*tp_repr*/
    0,                                                /*tp_as_number*/
    0,                                                /*tp_as_sequence*/
    0,                                                /*tp_as_mapping*/
    0,                                                /*tp_hash*/
    0,                                                /*tp_call */
    0,                                                /*tp_str  */
    0,                                                /*tp_getattro*/
    0,                                                /*tp_setattro*/
    /* --- Functions to access object as input/output buffer ---------*/
    0,                                                /* tp_as_buffer */
    /* --- Flags to define presence of optional/expanded features */
    Py_TPFLAGS_BASETYPE|Py_TPFLAGS_HAVE_CLASS,        /*tp_flags */
    "Describes an infinite line",           /*tp_doc */
    0,                                                /*tp_traverse */
    0,                                                /*tp_clear */
    0,                                                /*tp_richcompare */
    0,                                                /*tp_weaklistoffset */
    0,                                                /*tp_iter */
    0,                                                /*tp_iternext */
    Part::LinePy::Methods,                     /*tp_methods */
    0,                                                /*tp_members */
    Part::LinePy::GetterSetter,                     /*tp_getset */
    &Part::GeometryPy::Type,                        /*tp_base */
    0,                                                /*tp_dict */
    0,                                                /*tp_descr_get */
    0,                                                /*tp_descr_set */
    0,                                                /*tp_dictoffset */
    __PyInit,                                         /*tp_init */
    0,                                                /*tp_alloc */
    Part::LinePy::PyMake,/*tp_new */
    0,                                                /*tp_free   Low-level free-memory routine */
    0,                                                /*tp_is_gc  For PyObject_IS_GC */
    0,                                                /*tp_bases */
    0,                                                /*tp_mro    method resolution order */
    0,                                                /*tp_cache */
    0,                                                /*tp_subclasses */
    0,                                                /*tp_weaklist */
    0                                                 /*tp_del */
};

/// Methods structure of LinePy
PyMethodDef LinePy::Methods[] = {
    {"setStartPoint",
        (PyCFunction) staticCallback_setStartPoint,
        Py_NEWARGS,
        ""
    },
    {"setEndPoint",
        (PyCFunction) staticCallback_setEndPoint,
        Py_NEWARGS,
        ""
    },
    {NULL, NULL, 0, NULL}		/* Sentinel */
};

/// Attribute structure of LinePy
PyGetSetDef LinePy::GetterSetter[] = {
    {NULL, NULL, NULL, NULL, NULL}		/* Sentinel */
};

// setStartPoint() callback and implementer
// PyObject*  LinePy::setStartPoint(PyObject *args){};
// has to be implemented in LinePyImp.cpp
PyObject * LinePy::staticCallback_setStartPoint (PyObject *self, PyObject *args, PyObject * /*kwd*/)
{
    // test if twin object not allready deleted
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    // test if object is set Const
    if (((PyObjectBase*) self)->isConst()){
        PyErr_SetString(PyExc_ReferenceError, "This object is immutable, you can not set any attribute or call a non const method");
        return NULL;
    }

    try { // catches all exeptions coming up from c++ and generate a python exeption
        return ((LinePy*)self)->setStartPoint(args);
    }
    catch(Base::Exception &e) // catch the FreeCAD exeptions
    {
        std::string str;
        str += "FreeCAD exception thrown (";
        str += e.what();
        str += ")";
        e.ReportException();
        PyErr_SetString(PyExc_Exception,str.c_str());
        return NULL;
    }
    catch(boost::filesystem::filesystem_error &e) // catch boost filesystem exeption
    {
        std::string str;
        str += "File system exeption thrown (";
        //str += e.who();
        //str += ", ";
        str += e.what();
        str += ")\n";
        Base::Console().Error(str.c_str());
        PyErr_SetString(PyExc_Exception,str.c_str());
        return NULL;
    }
    catch(const Py::Exception&)
    {
        // The exception text is already set
        return NULL;
    }
    catch(const char *e) // catch simple string exceptions
    {
        Base::Console().Error(e);
        PyErr_SetString(PyExc_Exception,e);
        return NULL;
    }
    // in debug not all exceptions will be catched to get the attention of the developer!
#ifndef DONT_CATCH_CXX_EXCEPTIONS 
    catch(std::exception &e) // catch other c++ exeptions
    {
        std::string str;
        str += "FC++ exception thrown (";
        str += e.what();
        str += ")";
        Base::Console().Error(str.c_str());
        PyErr_SetString(PyExc_Exception,str.c_str());
        return NULL;
    }
    catch(...)  // catch the rest!
    {
        PyErr_SetString(PyExc_Exception,"Unknown C++ exception");
        return NULL;
    }
#endif
}

// setEndPoint() callback and implementer
// PyObject*  LinePy::setEndPoint(PyObject *args){};
// has to be implemented in LinePyImp.cpp
PyObject * LinePy::staticCallback_setEndPoint (PyObject *self, PyObject *args, PyObject * /*kwd*/)
{
    // test if twin object not allready deleted
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    // test if object is set Const
    if (((PyObjectBase*) self)->isConst()){
        PyErr_SetString(PyExc_ReferenceError, "This object is immutable, you can not set any attribute or call a non const method");
        return NULL;
    }

    try { // catches all exeptions coming up from c++ and generate a python exeption
        return ((LinePy*)self)->setEndPoint(args);
    }
    catch(Base::Exception &e) // catch the FreeCAD exeptions
    {
        std::string str;
        str += "FreeCAD exception thrown (";
        str += e.what();
        str += ")";
        e.ReportException();
        PyErr_SetString(PyExc_Exception,str.c_str());
        return NULL;
    }
    catch(boost::filesystem::filesystem_error &e) // catch boost filesystem exeption
    {
        std::string str;
        str += "File system exeption thrown (";
        //str += e.who();
        //str += ", ";
        str += e.what();
        str += ")\n";
        Base::Console().Error(str.c_str());
        PyErr_SetString(PyExc_Exception,str.c_str());
        return NULL;
    }
    catch(const Py::Exception&)
    {
        // The exception text is already set
        return NULL;
    }
    catch(const char *e) // catch simple string exceptions
    {
        Base::Console().Error(e);
        PyErr_SetString(PyExc_Exception,e);
        return NULL;
    }
    // in debug not all exceptions will be catched to get the attention of the developer!
#ifndef DONT_CATCH_CXX_EXCEPTIONS 
    catch(std::exception &e) // catch other c++ exeptions
    {
        std::string str;
        str += "FC++ exception thrown (";
        str += e.what();
        str += ")";
        Base::Console().Error(str.c_str());
        PyErr_SetString(PyExc_Exception,str.c_str());
        return NULL;
    }
    catch(...)  // catch the rest!
    {
        PyErr_SetString(PyExc_Exception,"Unknown C++ exception");
        return NULL;
    }
#endif
}



//--------------------------------------------------------------------------
// Parents structure
//--------------------------------------------------------------------------
PyParentObject LinePy::Parents[] = { PARENTSPartLinePy };

//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------
LinePy::LinePy(GeomLine *pcObject, PyTypeObject *T)
    : GeometryPy(reinterpret_cast<GeometryPy::PointerType>(pcObject), T)
{
}


//--------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------
LinePy::~LinePy()                                // Everything handled in parent
{
}

//--------------------------------------------------------------------------
// LinePy representation
//--------------------------------------------------------------------------
PyObject *LinePy::_repr(void)
{
    return Py_BuildValue("s", representation());
}

//--------------------------------------------------------------------------
// LinePy Attributes
//--------------------------------------------------------------------------
PyObject *LinePy::_getattr(char *attr)				// __getattr__ function: note only need to handle new state
{
    try {
        // getter method for special Attributes (e.g. dynamic ones)
        PyObject *r = getCustomAttributes(attr);
        if(r) return r;
    }
#ifndef DONT_CATCH_CXX_EXCEPTIONS 
    catch(Base::Exception &e) // catch the FreeCAD exeptions
    {
        std::string str;
        str += "FreeCAD exception thrown (";
        str += e.what();
        str += ")";
        e.ReportException();
        PyErr_SetString(PyExc_Exception,str.c_str());
        return NULL;
    }
    catch(std::exception &e) // catch other c++ exeptions
    {
        std::string str;
        str += "FC++ exception thrown (";
        str += e.what();
        str += ")";
        Base::Console().Error(str.c_str());
        PyErr_SetString(PyExc_Exception,str.c_str());
        return NULL;
    }
    catch(const Py::Exception&)
    {
        // The exception text is already set
        return NULL;
    }
    catch(...)  // catch the rest!
    {
        PyErr_SetString(PyExc_Exception,"Unknown C++ exception");
        return NULL;
    }
#else  // DONT_CATCH_CXX_EXCEPTIONS  
    catch(Base::Exception &e) // catch the FreeCAD exeptions
    {
        std::string str;
        str += "FreeCAD exception thrown (";
        str += e.what();
        str += ")";
        e.ReportException();
        PyErr_SetString(PyExc_Exception,str.c_str());
        return NULL;
    }
    catch(const Py::Exception&)
    {
        // The exception text is already set
        return NULL;
    }
#endif  // DONT_CATCH_CXX_EXCEPTIONS

    _getattr_up(GeometryPy);
}

int LinePy::_setattr(char *attr, PyObject *value) 	// __setattr__ function: note only need to handle new state
{
    try {
        // setter for  special Attributes (e.g. dynamic ones)
        int r = setCustomAttributes(attr, value);
        if(r==1) return 0;
    }
#ifndef DONT_CATCH_CXX_EXCEPTIONS 
    catch(Base::Exception &e) // catch the FreeCAD exeptions
    {
        std::string str;
        str += "FreeCAD exception thrown (";
        str += e.what();
        str += ")";
        e.ReportException();
        PyErr_SetString(PyExc_Exception,str.c_str());
        return -1;
    }
    catch(std::exception &e) // catch other c++ exeptions
    {
        std::string str;
        str += "FC++ exception thrown (";
        str += e.what();
        str += ")";
        Base::Console().Error(str.c_str());
        PyErr_SetString(PyExc_Exception,str.c_str());
        return -1;
    }
    catch(const Py::Exception&)
    {
        // The exception text is already set
        return -1;
    }
    catch(...)  // catch the rest!
    {
        PyErr_SetString(PyExc_Exception,"Unknown C++ exception");
        return -1;
    }
#else  // DONT_CATCH_CXX_EXCEPTIONS  
    catch(Base::Exception &e) // catch the FreeCAD exeptions
    {
        std::string str;
        str += "FreeCAD exception thrown (";
        str += e.what();
        str += ")";
        e.ReportException();
        PyErr_SetString(PyExc_Exception,str.c_str());
        return -1;
    }
    catch(const Py::Exception&)
    {
        // The exception text is already set
        return -1;
    }
#endif  // DONT_CATCH_CXX_EXCEPTIONS

    return GeometryPy::_setattr(attr, value);
}

GeomLine *LinePy::getGeomLinePtr(void) const
{
    return static_cast<GeomLine *>(_pcTwinPointer);
}

#if 0
/* From here on come the methods you have to implement, but NOT in this module. Implement in LinePyImp.cpp! This prototypes 
 * are just for convenience when you add a new method.
 */

PyObject *LinePy::PyMake(struct _typeobject *, PyObject *, PyObject *)  // Python wrapper
{
    // create a new instance of LinePy and the Twin object 
    return new LinePy(new GeomLine);
}

// constructor method
int LinePy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// returns a string which represents the object e.g. when printed in python
const char *LinePy::representation(void) const
{
    return "<GeomLine object>";
}

PyObject* LinePy::setStartPoint(PyObject *args)
{
    PyErr_SetString(PyExc_NotImplementedError, "Not yet implemented");
    return 0;
}

PyObject* LinePy::setEndPoint(PyObject *args)
{
    PyErr_SetString(PyExc_NotImplementedError, "Not yet implemented");
    return 0;
}

PyObject *LinePy::getCustomAttributes(const char* attr) const
{
    return 0;
}

int LinePy::setCustomAttributes(const char* attr, PyObject *obj)
{
    return 0; 
}
#endif



