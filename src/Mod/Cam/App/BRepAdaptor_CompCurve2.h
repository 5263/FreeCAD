#ifndef _BRepAdaptor_CompCurve2_HeaderFile
#define _BRepAdaptor_CompCurve2_HeaderFile

#ifndef _TopoDS_Wire_HeaderFile
#include <TopoDS_Wire.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Handle_BRepAdaptor_HArray1OfCurve_HeaderFile
#include <Handle_BRepAdaptor_HArray1OfCurve.hxx>
#endif
#ifndef _Handle_TColStd_HArray1OfReal_HeaderFile
#include <Handle_TColStd_HArray1OfReal.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Adaptor3d_Curve_HeaderFile
#include <Adaptor3d_Curve.hxx>
#endif
#ifndef _GeomAbs_Shape_HeaderFile
#include <GeomAbs_Shape.hxx>
#endif
#ifndef _Handle_Adaptor3d_HCurve_HeaderFile
#include <Handle_Adaptor3d_HCurve.hxx>
#endif
#ifndef _GeomAbs_CurveType_HeaderFile
#include <GeomAbs_CurveType.hxx>
#endif
#ifndef _Handle_Geom_BezierCurve_HeaderFile
#include <Handle_Geom_BezierCurve.hxx>
#endif
#ifndef _Handle_Geom_BSplineCurve_HeaderFile
#include <Handle_Geom_BSplineCurve.hxx>
#endif
class BRepAdaptor_HArray1OfCurve;
class TColStd_HArray1OfReal;
class Standard_NullObject;
class Standard_DomainError;
class Standard_OutOfRange;
class Standard_NoSuchObject;
class TopoDS_Wire;
class TopoDS_Edge;
class TColStd_Array1OfReal;
class Adaptor3d_HCurve;
class gp_Pnt;
class gp_Vec;
class gp_Lin;
class gp_Circ;
class gp_Elips;
class gp_Hypr;
class gp_Parab;
class Geom_BezierCurve;
class Geom_BSplineCurve;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

//! The Curve from BRepAdaptor allows to use a Wire <br>
//!          of the BRep topology like a 3D curve. <br>
//!  Warning: With this  class of curve,  C0 and C1 continuities <br>
//!          are not assumed. So be carful with some algorithm! <br>
class BRepAdaptor_CompCurve2  : public Adaptor3d_Curve {

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

//! Creates an undefined Curve with no Wire loaded. <br>
Standard_EXPORT BRepAdaptor_CompCurve2();


Standard_EXPORT BRepAdaptor_CompCurve2(const TopoDS_Wire& W,const Standard_Boolean KnotByCurvilinearAbcissa = Standard_False);

//! Creates a Curve  to  acces to the geometry of edge <br>
//!          <W>. <br>
//! <br>
Standard_EXPORT BRepAdaptor_CompCurve2(const TopoDS_Wire& W,const Standard_Boolean KnotByCurvilinearAbcissa,const Standard_Real First,const Standard_Real Last,const Standard_Real Tol);

//! Sets the  wire <W>. <br>
Standard_EXPORT   void Initialize(const TopoDS_Wire& W,const Standard_Boolean KnotByCurvilinearAbcissa) ;

//! Sets wire <W> and trimmed  parameter. <br>
Standard_EXPORT   void Initialize(const TopoDS_Wire& W,const Standard_Boolean KnotByCurvilinearAbcissa,const Standard_Real First,const Standard_Real Last,const Standard_Real Tol) ;

//! Set the flag Periodic. <br>
//!  Warning: This method has no effect if the wire is not closed <br>
Standard_EXPORT   void SetPeriodic(const Standard_Boolean Periodic) ;

//! Returns the wire. <br>
//! <br>
Standard_EXPORT  const TopoDS_Wire& Wire() const;

//! returns an  edge  and   one  parameter on them <br>
//!           corresponding to the parameter U. <br>
Standard_EXPORT   void Edge(const Standard_Real U,TopoDS_Edge& E,Standard_Real& UonE) const;


Standard_EXPORT   Standard_Real FirstParameter() const;


Standard_EXPORT   Standard_Real LastParameter() const;


Standard_EXPORT   GeomAbs_Shape Continuity() const;

//! Returns  the number  of  intervals for  continuity <br>
//!          <S>. May be one if Continuity(me) >= <S> <br>
Standard_EXPORT   Standard_Integer NbIntervals(const GeomAbs_Shape S) ;

//! Stores in <T> the  parameters bounding the intervals <br>
//!          of continuity <S>. <br>
//! <br>
//!          The array must provide  enough room to  accomodate <br>
//!          for the parameters. i.e. T.Length() > NbIntervals() <br>
Standard_EXPORT   void Intervals(TColStd_Array1OfReal& T,const GeomAbs_Shape S) ;


Standard_EXPORT   Standard_Boolean IsClosed() const;


Standard_EXPORT   Standard_Boolean IsPeriodic() const;


Standard_EXPORT   Standard_Real Period() const;

//! Computes the point of parameter U on the curve <br>
Standard_EXPORT   gp_Pnt Value(const Standard_Real U) const;

//! Computes the point of parameter U. <br>
Standard_EXPORT   void D0(const Standard_Real U,gp_Pnt& P) const;

//! Computes the point of parameter U on the curve <br>
//!  with its first derivative. <br>//! Raised if the continuity of the current interval <br>
//!  is not C1. <br>
Standard_EXPORT   void D1(const Standard_Real U,gp_Pnt& P,gp_Vec& V) const;


//!  Returns the point P of parameter U, the first and second <br>
//!  derivatives V1 and V2. <br>//! Raised if the continuity of the current interval <br>
//!  is not C2. <br>
Standard_EXPORT   void D2(const Standard_Real U,gp_Pnt& P,gp_Vec& V1,gp_Vec& V2) const;


//!  Returns the point P of parameter U, the first, the second <br>
//!  and the third derivative. <br>//! Raised if the continuity of the current interval <br>
//!  is not C3. <br>
Standard_EXPORT   void D3(const Standard_Real U,gp_Pnt& P,gp_Vec& V1,gp_Vec& V2,gp_Vec& V3) const;


//!  The returned vector gives the value of the derivative for the <br>
//!  order of derivation N. <br>//! Raised if the continuity of the current interval <br>
//!  is not CN. <br>//! Raised if N < 1. <br>
Standard_EXPORT   gp_Vec DN(const Standard_Real U,const Standard_Integer N) const;

//! returns the parametric resolution <br>
Standard_EXPORT   Standard_Real Resolution(const Standard_Real R3d) const;


Standard_EXPORT   GeomAbs_CurveType GetType() const;


Standard_EXPORT   gp_Lin Line() const;


Standard_EXPORT   gp_Circ Circle() const;


Standard_EXPORT   gp_Elips Ellipse() const;


Standard_EXPORT   gp_Hypr Hyperbola() const;


Standard_EXPORT   gp_Parab Parabola() const;


Standard_EXPORT   Standard_Integer Degree() const;


Standard_EXPORT   Standard_Boolean IsRational() const;


Standard_EXPORT   Standard_Integer NbPoles() const;


Standard_EXPORT   Standard_Integer NbKnots() const;


Standard_EXPORT   Handle_Geom_BezierCurve Bezier() const;


Standard_EXPORT   Handle_Geom_BSplineCurve BSpline() const;





protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


Standard_EXPORT   void Prepare(Standard_Real& W,Standard_Real& D,Standard_Integer& ind) const;


Standard_EXPORT   void InvPrepare(const Standard_Integer ind,Standard_Real& F,Standard_Real& D) const;


 // Fields PRIVATE
 //
TopoDS_Wire myWire;
Standard_Real TFirst;
Standard_Real TLast;
Standard_Real PTol;
Standard_Real myPeriod;
Handle_BRepAdaptor_HArray1OfCurve myCurves;
Handle_TColStd_HArray1OfReal myKnots;
Standard_Integer CurIndex;
Standard_Boolean Forward;
Standard_Boolean IsbyAC;
Standard_Boolean Periodic;


};




#endif
