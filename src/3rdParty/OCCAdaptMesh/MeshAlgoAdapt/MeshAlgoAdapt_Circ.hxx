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

#ifndef _MeshAlgoAdapt_Circ_HeaderFile
#define _MeshAlgoAdapt_Circ_HeaderFile

#ifndef _gp_XY_HeaderFile
#include <gp_XY.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
class gp_XY;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

//! Describes  a  2d circle  with  a   size of  only 3 <br>
//!          Standard Real  numbers instead  of gp who  needs 7 <br>
//!          Standard Real numbers. <br>
class MeshAlgoAdapt_Circ  {

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


Standard_EXPORT MeshAlgoAdapt_Circ();


Standard_EXPORT MeshAlgoAdapt_Circ(const gp_XY& loc,const Standard_Real rad);


Standard_EXPORT   void SetLocation(const gp_XY& loc) ;


Standard_EXPORT   void SetRadius(const Standard_Real rad) ;

 const gp_XY& Location() const;

 const Standard_Real& Radius() const;





protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


 // Fields PRIVATE
 //
gp_XY location;
Standard_Real radius;


};


#include <MeshAlgoAdapt_Circ.lxx>



// other Inline functions and methods (like "C++: function call" methods)
//


#endif
