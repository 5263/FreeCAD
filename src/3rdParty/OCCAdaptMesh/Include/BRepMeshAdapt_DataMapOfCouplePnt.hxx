// File generated by CPPExt (Value)
//
//                     Copyright (C) 1991 - 2000 by  
//                      Matra Datavision SA.  All rights reserved.
//  
//                     Copyright (C) 2001 - 2004 by
//                     Open CASCADE SA.  All rights reserved.
// 
// This file is part of the Open CASCADE Technology software.
//
// This software may be distributed and/or modified under the terms and
// conditions of the Open CASCADE Public License as defined by Open CASCADE SA
// and appearing in the file LICENSE included in the packaging of this file.
//  
// This software is distributed on an "AS IS" basis, without warranty of any
// kind, and Open CASCADE SA hereby disclaims all such warranties,
// including without limitation, any warranties of merchantability, fitness
// for a particular purpose or non-infringement. Please see the License for
// the specific terms and conditions governing rights and limitations under the
// License.

#ifndef _BRepMeshAdapt_DataMapOfCouplePnt_HeaderFile
#define _BRepMeshAdapt_DataMapOfCouplePnt_HeaderFile

#ifndef _TCollection_BasicMap_HeaderFile
#include <TCollection_BasicMap.hxx>
#endif
#ifndef _Handle_BRepMeshAdapt_DataMapNodeOfDataMapOfCouplePnt_HeaderFile
#include <Handle_BRepMeshAdapt_DataMapNodeOfDataMapOfCouplePnt.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_DomainError;
class Standard_NoSuchObject;
class MeshShape_Couple;
class gp_Pnt;
class MeshShape_CoupleHasher;
class BRepMeshAdapt_DataMapNodeOfDataMapOfCouplePnt;
class BRepMeshAdapt_DataMapIteratorOfDataMapOfCouplePnt;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif


class BRepMeshAdapt_DataMapOfCouplePnt  : public TCollection_BasicMap {

public:

    void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
 // Methods PUBLIC
 // 


Standard_EXPORT BRepMeshAdapt_DataMapOfCouplePnt(const Standard_Integer NbBuckets = 1);


Standard_EXPORT   BRepMeshAdapt_DataMapOfCouplePnt& Assign(const BRepMeshAdapt_DataMapOfCouplePnt& Other) ;
  BRepMeshAdapt_DataMapOfCouplePnt& operator =(const BRepMeshAdapt_DataMapOfCouplePnt& Other) 
{
  return Assign(Other);
}



Standard_EXPORT   void ReSize(const Standard_Integer NbBuckets) ;


Standard_EXPORT   void Clear() ;
~BRepMeshAdapt_DataMapOfCouplePnt()
{
  Clear();
}



Standard_EXPORT   Standard_Boolean Bind(const MeshShape_Couple& K,const gp_Pnt& I) ;


Standard_EXPORT   Standard_Boolean IsBound(const MeshShape_Couple& K) const;


Standard_EXPORT   Standard_Boolean UnBind(const MeshShape_Couple& K) ;


Standard_EXPORT  const gp_Pnt& Find(const MeshShape_Couple& K) const;
 const gp_Pnt& operator()(const MeshShape_Couple& K) const
{
  return Find(K);
}



Standard_EXPORT   gp_Pnt& ChangeFind(const MeshShape_Couple& K) ;
  gp_Pnt& operator()(const MeshShape_Couple& K) 
{
  return ChangeFind(K);
}






protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


Standard_EXPORT BRepMeshAdapt_DataMapOfCouplePnt(const BRepMeshAdapt_DataMapOfCouplePnt& Other);


 // Fields PRIVATE
 //


};





// other Inline functions and methods (like "C++: function call" methods)
//


#endif
