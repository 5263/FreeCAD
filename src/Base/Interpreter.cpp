/***************************************************************************
 *   (c) J�rgen Riegel (juergen.riegel@web.de) 2002                        *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License (LGPL)   *
 *   as published by the Free Software Foundation; either version 2 of     *
 *   the License, or (at your option) any later version.                   *
 *   for detail see the LICENCE text file.                                 *
 *                                                                         *
 *   FreeCAD is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with FreeCAD; if not, write to the Free Software        *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
 *   USA                                                                   *
 *                                                                         *
 *   Juergen Riegel 2002                                                   *
 ***************************************************************************/


#include "PreCompiled.h"

#ifndef _PreComp_
#   include <Python.h>
#   include <sstream>
#endif

#include "Console.h"
#include "Interpreter.h"
#include "PyTools.h"
#include "Exception.h"
#include "PyObjectBase.h"

char format2[1024];  //Warning! Can't go over 512 characters!!!
unsigned int format2_len = 1024;

using namespace Base;

#if PY_VERSION_HEX <= 0x02050000
#error "Use Python2.5.x or higher"
#endif


PyException::PyException(void)
{
    PP_Fetch_Error_Text();    /* fetch (and clear) exception */
    std::string prefix = PP_last_error_type; /* exception name text */
//  prefix += ": ";
    std::string error = PP_last_error_info;            /* exception data text */
#if 0
    // The Python exceptions might be thrown from nested functions, so take
    // into account not to add the same prefix several times
    std::string::size_type pos = error.find(prefix);
    if (pos == std::string::npos)
        _sErrMsg = prefix + error;
    else
        _sErrMsg = error;
#endif
    _sErrMsg = error;
    _errorType = prefix;


    _stackTrace = PP_last_error_trace;     /* exception traceback text */

    PyErr_Clear(); // must be called to keep Python interpreter in a valid state (Werner)

}

// ---------------------------------------------------------

SystemExitException::SystemExitException()
{
    _sErrMsg = "System exit";
}

SystemExitException::SystemExitException(const SystemExitException &inst)
        : Exception(inst)
{
}

// ---------------------------------------------------------


InterpreterSingleton::InterpreterSingleton()
{
    //Py_Initialize();
}

InterpreterSingleton::~InterpreterSingleton()
{

}


std::string InterpreterSingleton::runString(const char *sCmd)
{
    PyObject *module, *dict, *presult;          /* "exec code in d, d" */

    module = PP_Load_Module("__main__");         /* get module, init python */
    if (module == NULL)
        throw PyException();                         /* not incref'd */
    dict = PyModule_GetDict(module);            /* get dict namespace */
    if (dict == NULL)
        throw PyException();                           /* not incref'd */


    presult = PyRun_String(sCmd, Py_file_input, dict, dict); /* eval direct */
    if (!presult) {
        throw PyException();
    }

    PyObject* repr = PyObject_Repr(presult);
    Py_DECREF(presult);
    if (repr) {
        std::string ret(PyString_AsString(repr));
        Py_DECREF(repr);
        return ret;
    }
    else {
        PyErr_Clear();
        return std::string();
    }
}

void InterpreterSingleton::systemExit(void)
{
    /* This code is taken from the original Python code */
    PyObject *exception, *value, *tb;
    int exitcode = 0;

    PyErr_Fetch(&exception, &value, &tb);
    if (Py_FlushLine())
        PyErr_Clear();
    fflush(stdout);
    if (value == NULL || value == Py_None)
        goto done;
    if (PyInstance_Check(value)) {
        /* The error code should be in the `code' attribute. */
        PyObject *code = PyObject_GetAttrString(value, "code");
        if (code) {
            Py_DECREF(value);
            value = code;
            if (value == Py_None)
                goto done;
        }
        /* If we failed to dig out the 'code' attribute,
           just let the else clause below print the error. */
    }
    if (PyInt_Check(value))
        exitcode = (int)PyInt_AsLong(value);
    else {
        PyObject_Print(value, stderr, Py_PRINT_RAW);
        PySys_WriteStderr("\n");
        exitcode = 1;
    }
done:
    /* Restore and clear the exception info, in order to properly decref
     * the exception, value, and traceback.  If we just exit instead,
     * these leak, which confuses PYTHONDUMPREFS output, and may prevent
     * some finalizers from running.
     */
    PyErr_Restore(exception, value, tb);
    PyErr_Clear();
    Py_Exit(exitcode);
    /* NOTREACHED */
}

void InterpreterSingleton::runInteractiveString(const char *sCmd)
{
    PyObject *module, *dict, *presult;          /* "exec code in d, d" */

    module = PP_Load_Module("__main__");         /* get module, init python */
    if (module == NULL)
        throw PyException();                         /* not incref'd */
    dict = PyModule_GetDict(module);            /* get dict namespace */
    if (dict == NULL)
        throw PyException();                           /* not incref'd */

    presult = PyRun_String(sCmd, Py_single_input, dict, dict); /* eval direct */
    if (!presult) {
        if (PyErr_ExceptionMatches(PyExc_SystemExit)) {
            //systemExit();
            throw SystemExitException();
        }
        /* get latest python exception information */
        /* and print the error to the error output */
        PyObject *errobj, *errdata, *errtraceback;
        PyErr_Fetch(&errobj, &errdata, &errtraceback);

        Exception exc; // do not use PyException since this clears the error indicator
        if ( PyString_Check( errdata ) )
            exc.setMessage( PyString_AsString( errdata ) );
        PyErr_Restore(errobj, errdata, errtraceback);
        if ( PyErr_Occurred() )
            PyErr_Print();
        throw exc;
    }
}

void InterpreterSingleton::runFile(const char*pxFileName)
{
    std::ifstream file;
    file.open(pxFileName);
    if (file) {
        std::stringbuf buf;
        file >> &buf;
        file.close();

        runString(buf.str().c_str());
    }
    else {
        std::string err = "Unknown file: ";
        err += pxFileName;
        err += "\n";
        throw Exception(err);
    }
}

bool InterpreterSingleton::loadModule(const char* psModName)
{
    // buffer acrobatics
    //PyBuf ModName(psModName);
    PyObject *module;

    module = PP_Load_Module(psModName);

    if (!module )
        throw PyException();

    Py_XINCREF(module);

    return true;
}

void InterpreterSingleton::addPythonPaths(const char* /*Path*/)
{
    // not implemented so far
    assert(0);
}

int InterpreterSingleton::cleanup(void (*func)(void))
{
    return Py_AtExit( func );
}

void InterpreterSingleton::finalize()
{
    try {
        Py_Finalize();
    }
    catch (...) {}
}

void InterpreterSingleton::runStringArg(const char * psCom,...)
{
    // va stuff
    va_list namelessVars;
    va_start(namelessVars, psCom);  // Get the "..." vars
    int len = vsnprintf(format2, format2_len, psCom, namelessVars);
    va_end(namelessVars);
    if ( len == -1 ) {
        // argument too long
        assert(false);
    }

    runString(format2);
}


// Singelton:

InterpreterSingleton * InterpreterSingleton::_pcSingelton = 0;

InterpreterSingleton & InterpreterSingleton::Instance(void)
{
    // not initialized!
    if (!_pcSingelton)
        _pcSingelton = new InterpreterSingleton();
    return *_pcSingelton;
}

void InterpreterSingleton::Destruct(void)
{
    // not initialized or double destruct!
    assert(_pcSingelton);
    delete _pcSingelton;
    _pcSingelton = 0;
}

const char* InterpreterSingleton::init(int argc,char *argv[])
{
    Py_SetProgramName(argv[0]);
    Py_Initialize();
    PySys_SetArgv(argc, argv);
    return Py_GetPath();
}

int InterpreterSingleton::runCommandLine(const char *prompt)
{
    return PP_Run_Command_Line(prompt);
}

/**
 *  Runs a member method of an object with no parameter and no return value
 *  void (void). There are other methods to run with returns
 */
void InterpreterSingleton::runMethodVoid(PyObject *pobject, const char *method)
{
    if (PP_Run_Method(pobject ,    // object
                      method,  // run method
                      0,			   // no return type
                      0,		       // so no return object
                      "()")		   // no arguments
            != 0)
        throw PyException(/*"Error running InterpreterSingleton::RunMethodVoid()"*/);

}

PyObject* InterpreterSingleton::runMethodObject(PyObject *pobject, const char *method)
{
    PyObject *pcO;

    if (PP_Run_Method(pobject ,    // object
                      method,  // run method
                      "O",		   // return type
                      &pcO,		   // return object
                      "()")		   // no arguments
            != 0)
        throw PyException(/*"Error runing InterpreterSingleton::RunMethodObject()"*/);

    return pcO;
}

void InterpreterSingleton::runMethod(PyObject *pobject, const char *method,
                                     const char *resfmt,   void *cresult,        /* convert to c/c++ */
                                     const char *argfmt,   ...  )                /* convert to python */
{
    PyObject *pmeth, *pargs, *presult;
    va_list argslist;                              /* "pobject.method(args)" */
    va_start(argslist, argfmt);

    pmeth = PyObject_GetAttrString(pobject, method);
    if (pmeth == NULL)                             /* get callable object */
        throw Exception("Error runing InterpreterSingleton::RunMethod() method not defined");                                 /* bound method? has self */

    pargs = Py_VaBuildValue(argfmt, argslist);     /* args: c->python */

    if (pargs == NULL) {
        Py_DECREF(pmeth);
        throw Exception("InterpreterSingleton::RunMethod() wrong arguments");
    }

    presult = PyEval_CallObject(pmeth, pargs);   /* run interpreter */

    Py_DECREF(pmeth);
    Py_DECREF(pargs);
    if (PP_Convert_Result(presult, resfmt, cresult)!= 0) {
        if ( PyErr_Occurred() )
            PyErr_Print();
        throw Exception("Error runing InterpreterSingleton::RunMethod() exception in called method");
    }
}


void InterpreterSingleton::dbgObserveFile(const char* sFileName)
{
    if (sFileName)
        _cDebugFileName = sFileName;
    else
        _cDebugFileName = "";
}

void InterpreterSingleton::dbgSetBreakPoint(unsigned int /*uiLineNumber*/)
{

}

void InterpreterSingleton::dbgUnsetBreakPoint(unsigned int /*uiLineNumber*/)
{

}

void InterpreterSingleton::dbgStep(void)
{

}


const std::string InterpreterSingleton::strToPython(const char* Str)
{
    std::string result;
    const char *It=Str;

    while (*It != '\0') {
        switch (*It) {
        case '\\':
            result += "\\\\";
            break;
        case '\"':
            result += "\\\"";
            break;
        case '\'':
            result += "\\\'";
            break;
        default:
            result += *It;
        }
        It++;
    }

    return result;
}


PyObject *InterpreterSingleton::CreateFrom(const std::map<std::string,std::string> &StringMap)
{
    PyObject *Dict = PyDict_New();
    PyObject *pKey,*pValue;

    for (std::map<std::string,std::string>::const_iterator It = StringMap.begin();It != StringMap.end();It++) {
        pKey   = PyString_FromString(It->first.c_str());
        pValue = PyString_FromString(It->second.c_str());
        PyDict_SetItem(Dict, pKey, pValue);
    }

    return Dict;

}

