

#include "PreCompiled.h"

#ifndef _PreComp_
# include <sstream>
#endif

#include "Document.h"
#include <Base/FileInfo.h>
#include <Base/PyTools.h>
#include "DocumentObject.h"

// inclusion of the generated files (generated By DocumentPy.xml)
#include "DocumentPy.h"
#include "DocumentPy.cpp"

using namespace App;

// returns a string which represent the object e.g. when printed in python
const char *DocumentPy::representation(void) const
{
    return "<Document>";
}

PyObject*  DocumentPy::save(PyObject *args)
{
    if (!getDocumentObject()->save()) {
        PyErr_Format(PyExc_ValueError, "Object attribute 'FileName' is not set");
        return NULL;
    }

    const char* filename = getDocumentObject()->FileName.getValue();
    Base::FileInfo fi(filename);
    if (!fi.isReadable()) {
        PyErr_Format(PyExc_IOError, "No such file or directory: '%s'", filename);
        return NULL;
    }

    Py_Return;
}

PyObject*  DocumentPy::restore(PyObject *args)
{
    const char* filename = getDocumentObject()->FileName.getValue();
    if (!filename || *filename == '\0') {
        PyErr_Format(PyExc_ValueError, "Object attribute 'FileName' is not set");
        return NULL;
    }
    Base::FileInfo fi(filename);
    if (!fi.isReadable()) {
        PyErr_Format(PyExc_IOError, "No such file or directory: '%s'", filename);
        return NULL;
    }
    try {
        getDocumentObject()->restore();
    } catch (...) {
        PyErr_Format(PyExc_IOError, "Reading from file '%s' failed", filename);
        return NULL;
    }
    Py_Return;
}

PyObject*  DocumentPy::addObject(PyObject *args)
{
    char *sType,*sName=0;
    if (!PyArg_ParseTuple(args, "s|s", &sType,&sName))     // convert args: Python->C
        return NULL;                                         // NULL triggers exception 

    DocumentObject *pcFtr;

    pcFtr = getDocumentObject()->addObject(sType,sName);

    if(pcFtr)
        return pcFtr->getPyObject();
    else
    {
        char szBuf[200];
        snprintf(szBuf, 200, "No document object found of type '%s'", sType);
        Py_Error(PyExc_Exception,szBuf);
    }
}

PyObject*  DocumentPy::removeObject(PyObject *args)
{
    char *sName;
    if (!PyArg_ParseTuple(args, "s",&sName))     // convert args: Python->C
        return NULL;                             // NULL triggers exception


    DocumentObject *pcFtr = getDocumentObject()->getObject(sName);
    if(pcFtr) {
        getDocumentObject()->remObject( sName );
        Py_Return;
    } else {
        char szBuf[200];
        snprintf(szBuf, 200, "No feature found with name '%s'", sName);
        Py_Error(PyExc_Exception,szBuf);
    }
}

PyObject*  DocumentPy::openTransaction(PyObject *args)
{
    char *pstr=0;
    if (!PyArg_ParseTuple(args, "|s", &pstr))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 

    getDocumentObject()->openTransaction(pstr); 
    Py_Return; 
}

PyObject*  DocumentPy::abortTransaction(PyObject *args)
{
    getDocumentObject()->abortTransaction();
    Py_Return;
}

PyObject*  DocumentPy::commitTransaction(PyObject *args)
{
    getDocumentObject()->commitTransaction();
    Py_Return;
}

PyObject*  DocumentPy::undo(PyObject *args)
{
    if (getDocumentObject()->getAvailableUndos())
        getDocumentObject()->undo();
    Py_Return;
}

PyObject*  DocumentPy::redo(PyObject *args)
{
    if (getDocumentObject()->getAvailableRedos())
        getDocumentObject()->redo();
    Py_Return;
}

PyObject*  DocumentPy::clearUndos(PyObject *args)
{
    getDocumentObject()->clearUndos();
    Py_Return;
}

PyObject*  DocumentPy::recompute(PyObject *args)
{
    getDocumentObject()->recompute();
    Py_Return;
}

PyObject*  DocumentPy::getObject(PyObject *args)
{
    char *sName;
    if (!PyArg_ParseTuple(args, "s",&sName))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 

    DocumentObject *pcFtr = getDocumentObject()->getObject(sName);
    if(pcFtr)
        return pcFtr->getPyObject();
    else
        Py_Return;
}

Py::Object DocumentPy::getActiveObject(void) const
{
    DocumentObject *pcFtr = getDocumentObject()->getActiveObject();
    if(pcFtr)
        return Py::Object(pcFtr->getPyObject());
    return Py::None();
}

PyObject*  DocumentPy::supportedTypes(PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))     // convert args: Python->C 
        return NULL;                    // NULL triggers exception
    
    std::vector<Base::Type> ary;
    Base::Type::getAllDerivedFrom(App::DocumentObject::getClassTypeId(), ary);
    Py::List res;
    for (std::vector<Base::Type>::iterator it = ary.begin(); it != ary.end(); ++it)
        res.append(Py::String(it->getName()));
    return Py::new_reference_to(res);
}

void  DocumentPy::setActiveObject(Py::Object arg)
{
    throw Py::AttributeError("'Document' object attribute 'ActiveObject' is read-only");
}

Py::List DocumentPy::getObjects(void) const 
{
    std::map<std::string,DocumentObject*> features = getDocumentObject()->ObjectMap;
    Py::List res;

    for (std::map<std::string,DocumentObject*>::const_iterator It = features.begin();It != features.end();++It)
        //Note: Here we must force the Py::Object to own this Python object as getPyObject() increments the counter
        res.append(Py::Object(It->second->getPyObject(), true));

    return res;
}

Py::Int DocumentPy::getUndoMode(void) const
{
    return Py::Int(getDocumentObject()->getUndoMode());
}

void  DocumentPy::setUndoMode(Py::Int arg)
{
    getDocumentObject()->setUndoMode(arg); 
}

Py::Int DocumentPy::getUndoRedoMemSize(void) const
{
    return Py::Int((long)getDocumentObject()->getUndoMemSize());
}

Py::Int DocumentPy::getUndoCount(void) const
{
    return Py::Int((long)getDocumentObject()->getAvailableUndos());
}

Py::Int DocumentPy::getRedoCount(void) const
{
    return Py::Int((long)getDocumentObject()->getAvailableRedos());
}

Py::List DocumentPy::getUndoNames(void) const
{
    std::vector<std::string> vList = getDocumentObject()->getAvailableUndoNames();
    Py::List res;

    for (std::vector<std::string>::const_iterator It = vList.begin();It!=vList.end();++It)
        res.append(Py::String(*It));

    return res;
}

Py::List DocumentPy::getRedoNames(void) const
{
    std::vector<std::string> vList = getDocumentObject()->getAvailableRedoNames();
    Py::List res;

    for (std::vector<std::string>::const_iterator It = vList.begin();It!=vList.end();++It)
        res.append(Py::String(*It));

    return res;
}

Py::String  DocumentPy::getDependencyGraph(void) const
{
  std::stringstream out;
  getDocumentObject()->writeDependencyGraphViz(out);
  return Py::String(out.str());
}

PyObject *DocumentPy::getCustomAttributes(const char* attr) const
{
    // Note: Here we want to return only a document object if its
    // name matches 'attr'. However, it is possible to have an object
    // with the same name as an attribute. If so, we return 0 as other-
    // wise it wouldn't be possible to address this attribute any more.
    // The object must then be addressed by the getObject() method directly.
    App::Property* prop = getPropertyContainerObject()->getPropertyByName(attr);
    if (prop) return 0;
    PyObject *method = Py_FindMethod(this->Methods, const_cast<DocumentPy*>(this), attr);
    if (method) {
        Py_DECREF(method);
        return 0;
    } else if (PyErr_Occurred()) {
        PyErr_Clear();
    }
    // search for an object with this name
    DocumentObject* obj = getDocumentObject()->getObject(attr);
    return (obj ? obj->getPyObject() : 0);
}

int DocumentPy::setCustomAttributes(const char* attr, PyObject *)
{
    // Note: Here we want to return only a document object if its
    // name matches 'attr'. However, it is possible to have an object
    // with the same name as an attribute. If so, we return 0 as other-
    // wise it wouldn't be possible to address this attribute any more.
    // The object must then be addressed by the getObject() method directly.
    App::Property* prop = getPropertyContainerObject()->getPropertyByName(attr);
    if (prop) return 0;
    PyObject *method = Py_FindMethod(this->Methods, this, attr);
    if (method) {
        Py_DECREF(method);
        return 0;
    } else if (PyErr_Occurred()) {
        PyErr_Clear();
    }
    DocumentObject* obj = getDocumentObject()->getObject(attr);
    if (obj)
    {
        char szBuf[200];
        snprintf(szBuf, 200, "'Document' object attribute '%s' must not be set this way", attr);
        throw Py::AttributeError(szBuf); 
    }
    
    return 0;
}
