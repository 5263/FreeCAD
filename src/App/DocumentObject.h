/***************************************************************************
 *   Copyright (c) J�rgen Riegel          (juergen.riegel@web.de)          *
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




#ifndef APP_DOCUMENTOBJECT_H
#define APP_DOCUMENTOBJECT_H

#include <App/PropertyContainer.h>
#include <App/PropertyStandard.h>

#include <Base/TimeInfo.h>
#include <Base/PyCXX/Objects.hxx>

#include <bitset>


namespace App
{
class Document;
class DocumentObjectPy;

/** Return object for feature execution
*/
class AppExport DocumentObjectExecReturn
{
public:
    DocumentObjectExecReturn(const char* sWhy, DocumentObject* WhichObject=0)
        :Which(WhichObject)
    {
        if(sWhy)
            Why = sWhy;
    }

    std::string Why;
    DocumentObject* Which;
};



/** Base class of all Classes handled in the Document
 */
class AppExport DocumentObject: public App::PropertyContainer
{
    PROPERTY_HEADER(App::DocumentObject);

public:

    PropertyString Label;

    /// returns the type name of the ViewProvider
    virtual const char* getViewProviderName(void) const {
        return "";
    }
    /// Constructor
    DocumentObject(void);
    virtual ~DocumentObject();


    /// returns the name which is set in the document for this object (not the Name propertie!)
    const char *getNameInDocument(void) const;
    /// gets the document in which this Object is handled
    App::Document &getDocument(void) const;

    /** Set the property touched -> changed, cause recomputation in Update()
     */
    //@{
    /// set this feature touched (cause recomputation on depndend features)
    void touch(void);
    /// test if this feature is touched 
    bool isTouched(void) const {return StatusBits.test(0);}
    /// reset this feature touched 
    void purgeTouched(void){StatusBits.reset(0);setPropertyStatus(0,false);}
    /// set this feature to error 
    bool isError(void) const {return StatusBits.test(1);}
    /// remove the error from the object
    void purgeError(void){StatusBits.reset(1);}
    //@}

    /** get called by the document to recompute this feature
      * Normaly this methode get called in the processing of 
      * Document::recompute(). 
      * In execute() the outpupt properties get recomputed
      * with the data from linked objects and objects own 
      * properties.
      */
    virtual App::DocumentObjectExecReturn *execute(void);

    /** mustExecute
     *  We call this method to check if the object was modified to
     *  be invoked. If the object label or an argument is modified.
     *  If we must recompute the object - to call the method Execute().
     *  0: no recompution is needed
     *  1: recompution needed
     * -1: the document examine all links of this object and if one is touched -> recompute
     */ 
    virtual short mustExecute(void) const;


    /// get the status Message
    const char *getStatusString(void) const;//{return _cStatusMessage.c_str();}



    /** Called in case of losing a link
     * Get called by the document when a object got deleted a link property of this 
     * object ist pointing to. The standard behaivour of the DocumentObject implementation 
     * is to reset the links to nothing. You may overide this methode to implement 
     *additional or different behavior.
     */
    virtual void onLoseLinkToObject(DocumentObject*);
    virtual PyObject *getPyObject(void);

    friend class Document;
    friend class Transaction;

    static DocumentObjectExecReturn *StdReturn;

    /** status bits of the document object
      * the first 8 bits are used for the base system
      * the rest can be used in descendend classes to 
      * to mark special stati on the objects
      */   
    std::bitset<32> StatusBits;

protected:

    void setError(void){StatusBits.set(1);}
    void resetError(void){StatusBits.reset(1);}
    void setDocument(App::Document* doc);
    
    /// get called befor the value is changed
    virtual void onBevorChange(const Property* prop);
    /// get called by the container when a Proptery was changed
    virtual void onChanged(const Property* prop);

     /// python object of this class and all descendend
protected: // attributes
    Py::Object PythonObject;
    /// pointer to the document this object belongs to
    App::Document* _pDoc;

    // pointer to the document name string (for performance)
    const std::string *pcNameInDocument;
};

} //namespace App


#endif // APP_DOCUMENTOBJECT_H
