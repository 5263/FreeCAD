/** \file $RCSfile$
 *  \brief The attribute module
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
 *   Jürgen Riegel 2002                                                    *
 *                                                                         *
 ***************************************************************************/
 


#ifndef _Attribute_h_
#define _Attribute_h_


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif


#include <TCollection_ExtendedString.hxx>
#include <TDF_Attribute.hxx>
#include <Handle_TDF_Attribute.hxx>
#include <Handle_TDF_RelocationTable.hxx>
#include <Standard_OStream.hxx>
class Standard_DomainError;
class Standard_GUID;
class TDF_Label;
class TCollection_ExtendedString;
class TDF_Attribute;
class TDF_RelocationTable;

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_TDF_Attribute_HeaderFile
#include <Handle_TDF_Attribute.hxx>
#endif



class Standard_Transient;
class Handle_Standard_Type;
class Handle(TDF_Attribute);
class TDataStd_Name;


Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(FCAttribute);



class FCAttribute;


/** Handle class for FCAttribute
 */
class Handle_FCAttribute :public Handle_TDF_Attribute
{
public:
	AppExport void* operator new(size_t,void* anAddress){return anAddress;}
	AppExport void* operator new(size_t size){return Standard::Allocate(size);}
	AppExport void  operator delete(void *anAddress){if (anAddress) Standard::Free((Standard_Address&)anAddress);}
	Handle_FCAttribute():Handle(TDF_Attribute)() {} 
	Handle_FCAttribute(const Handle(FCAttribute)& aHandle) : Handle(TDF_Attribute)(aHandle){}

	Handle_FCAttribute(const FCAttribute* anItem) : Handle(TDF_Attribute)((TDF_Attribute *)anItem){}

	Handle_FCAttribute& operator=(const Handle(FCAttribute)& aHandle)
	{
		Assign(&(*aHandle));
		return *this;
	}

	Handle_FCAttribute& operator=(const FCAttribute* anItem)
	{
		Assign((Standard_Transient *)anItem);
		return *this;
	}

	FCAttribute* operator->() 
	{
		return (FCAttribute *)(ControlAccess());
	}

	FCAttribute* operator->() const{return(FCAttribute *)ControlAccess();}
	~Handle_FCAttribute();
	static const Handle_FCAttribute DownCast(const Handle(Standard_Transient)& AnObject);
};



class FCAttribute : public TDF_Attribute 
{
public:

	/// Constructor
	AppExport FCAttribute();
	/// Destructor
	AppExport ~FCAttribute();

	/// Delivers the GUID of the Object
	AppExport static const Standard_GUID& GetID() ;


	/// Saving to a stream
	AppExport virtual  Standard_OStream& Dump(Standard_OStream& anOS) const;

	/// copy
	AppExport void Restore(const Handle(TDF_Attribute)& with) ;

	/// Set data
	AppExport void Set(const TCollection_ExtendedString& S) ;
	/// Get data
	AppExport TCollection_ExtendedString Get() const;

	/// not shure
	AppExport static  Handle_FCAttribute Set(const TDF_Label& label,const TCollection_ExtendedString& string) ;

	/// Get the unique ID of the Attribute
	AppExport const Standard_GUID& ID() const;

	/// Get a empty instance
	AppExport Handle_TDF_Attribute NewEmpty() const;

	/// some kind of pasting ???
	AppExport void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;


	/// needet for CasCade handles 
    AppExport void* operator new(size_t,void* anAddress)	{return anAddress;}
    AppExport void* operator new(size_t size)				{return Standard::Allocate(size);}
    AppExport void  operator delete(void *anAddress)		{if (anAddress) Standard::Free((Standard_Address&)anAddress);}
	          friend Handle_Standard_Type& FCAttribute_Type_();
	AppExport const Handle(Standard_Type)& DynamicType() const;
	AppExport Standard_Boolean	       IsKind(const Handle(Standard_Type)&) const;

private: 

	/// Data member string
	TCollection_ExtendedString myString;

};





#endif
