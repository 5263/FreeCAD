/** \file AppPart.cpp
 *  \brief 
 *  \author $Author$
 *  \version $Revision$
 *  \date    $Date$
 */


/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *   for detail see the LICENCE text file.                                 *
 *   J�rgen Riegel 2002                                                    *
 *                                                                         *
 ***************************************************************************/
 
#include "PreCompiled.h"
#ifndef _PreComp_
#endif

#include <stdio.h>
#include <Python.h>

#include <Base/Console.h>
#include <App/Application.h>
#include <App/Document.h>

#include "PartDocType.h"


/* module functions */
static PyObject *                                 /* returns object */
message(PyObject *self, PyObject *args)           /* self unused in modules */
{                                                 /* args from python call */
    char *fromPython, result[64];
    if (! PyArg_Parse(args, "(s)", &fromPython))  /* convert Python -> C */
        return NULL;                              /* null=raise exception */
    else {
        strcpy(result, "Hello, ");                /* build up C string */
        strcat(result, fromPython);               /* add passed Python string */
        return Py_BuildValue("s", result);        /* convert C -> Python */
    }
}

/* module functions */
static PyObject *                                 /* returns object */
Box(PyObject *self, PyObject *args)               /* self unused in modules */
{                                                 /* args from python call */
	double  Float1,Float2,Float3,Float4,Float5,Float6;
    if (!PyArg_ParseTuple(args, "(dddddd)",&Float1,&Float2,&Float3,&Float4,&Float5,&Float6))     // convert args: Python->C
        return NULL;                              /* null=raise exception */
    else {


		return Py_None;                           /* convert C -> Python */
    }
}
/* development function for test purpose */
static PyObject *                                 /* returns object */
Temp(PyObject *self, PyObject *args)               /* self unused in modules */
{                                                 /* args from python call */
    if (!PyArg_ParseTuple(args, ""))            // convert args: Python->C
        return NULL;                              /* null=raise exception */
    else {

		GetConsole().Log("Part.Temp() is runing ....\n");

		FCDocument *doc = GetApplication().Active();
		if(!doc) return Py_None;

		GetConsole().Log("Doc init\n");
		FCPartDocType *pcType = new FCPartDocType();
		doc->InitType(pcType);

		GetConsole().Log("Type Get\n");
		if(strcmp(doc->GetDocType()->GetTypeName(),"Part")==0)
		{
			pcType = dynamic_cast<FCPartDocType*>( doc->GetDocType() );
			GetConsole().Log("Part doc detected\n");

		}

		return Py_None;                           /* convert C -> Python */
    }
}

/* registration table  */
static struct PyMethodDef hello_methods[] = {
    {"message", message, 1},       /* method name, C func ptr, always-tuple */
    {"AddBox" , Box,     1},       /* method name, C func ptr, always-tuple */
    {"Temp"   , Temp,    1},       /* method name, C func ptr, always-tuple */
    {NULL     , NULL      }        /* end of table marker */
};






// python intry
#ifdef FC_OS_WIN32
extern "C" {
void __declspec(dllexport) initPart() {

	(void) Py_InitModule("Part", hello_methods);   /* mod name, table ptr */

	GetApplication();

	GetConsole().Log("AppPart loaded\n");

	return;
}


} // extern "C" {
#endif
