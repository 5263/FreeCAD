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

#ifndef _MeshAlgoAdapt_DataMapOfIntegerCirc_HeaderFile
#define _MeshAlgoAdapt_DataMapOfIntegerCirc_HeaderFile

#ifndef _TCollection_BasicMap_HeaderFile
#include <TCollection_BasicMap.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Handle_MeshAlgoAdapt_DataMapNodeOfDataMapOfIntegerCirc_HeaderFile
#include <Handle_MeshAlgoAdapt_DataMapNodeOfDataMapOfIntegerCirc.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_DomainError;
class Standard_NoSuchObject;
class MeshAlgoAdapt_Circ;
class TColStd_MapIntegerHasher;
class MeshAlgoAdapt_DataMapNodeOfDataMapOfIntegerCirc;
class MeshAlgoAdapt_DataMapIteratorOfDataMapOfIntegerCirc;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif


class MeshAlgoAdapt_DataMapOfIntegerCirc  : public TCollection_BasicMap {

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


Standard_EXPORT MeshAlgoAdapt_DataMapOfIntegerCirc(const Standard_Integer NbBuckets = 1);


Standard_EXPORT   MeshAlgoAdapt_DataMapOfIntegerCirc& Assign(const MeshAlgoAdapt_DataMapOfIntegerCirc& Other) ;
  MeshAlgoAdapt_DataMapOfIntegerCirc& operator =(const MeshAlgoAdapt_DataMapOfIntegerCirc& Other) 
{
  return Assign(Other);
}



Standard_EXPORT   void ReSize(const Standard_Integer NbBuckets) ;


Standard_EXPORT   void Clear() ;
~MeshAlgoAdapt_DataMapOfIntegerCirc()
{
  Clear();
}



Standard_EXPORT   Standard_Boolean Bind(const Standard_Integer& K,const MeshAlgoAdapt_Circ& I) ;


Standard_EXPORT   Standard_Boolean IsBound(const Standard_Integer& K) const;


Standard_EXPORT   Standard_Boolean UnBind(const Standard_Integer& K) ;


Standard_EXPORT  const MeshAlgoAdapt_Circ& Find(const Standard_Integer& K) const;
 const MeshAlgoAdapt_Circ& operator()(const Standard_Integer& K) const
{
  return Find(K);
}



Standard_EXPORT   MeshAlgoAdapt_Circ& ChangeFind(const Standard_Integer& K) ;
  MeshAlgoAdapt_Circ& operator()(const Standard_Integer& K) 
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


Standard_EXPORT MeshAlgoAdapt_DataMapOfIntegerCirc(const MeshAlgoAdapt_DataMapOfIntegerCirc& Other);


 // Fields PRIVATE
 //


};





// other Inline functions and methods (like "C++: function call" methods)
//


#endif
