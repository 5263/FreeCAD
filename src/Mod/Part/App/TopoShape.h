/***************************************************************************
 *   Copyright (c) J�rgen Riegel          (juergen.riegel@web.de) 2002     *
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


#ifndef PART_TOPOSHAPE_H
#define PART_TOPOSHAPE_H

#include <gp_Vec.hxx>
#include <TopoDS_Wire.hxx>
#include <TopTools_ListOfShape.hxx>
#include <App/ComplexGeoData.h>

namespace Part
{

class AppPartExport ShapeSegment:public Data::Segment
{
    TYPESYSTEM_HEADER();

public:
    ShapeSegment(const TopoDS_Shape &ShapeIn):Shape(ShapeIn){}
    ShapeSegment(){}
    virtual std::string getName() const;

    TopoDS_Shape Shape;
};



/** The representation for a CAD Shape
 */
class AppPartExport TopoShape : public Data::ComplexGeoData
{
    TYPESYSTEM_HEADER();

public:
    TopoShape();
    TopoShape(const TopoDS_Shape&);
    TopoShape(const TopoShape&);
    ~TopoShape();

    void operator = (const TopoShape&);

    /** @name Placement control */
    //@{
    /// set the transformation of the CasCade Shape
    void setTransform(const Base::Matrix4D& rclTrf);
    /// get the transformation of the CasCade Shape
    Base::Matrix4D getTransform(void) const;
    /// Bound box from the CasCade shape
    Base::BoundBox3d getBoundBox(void)const;
    //@}

    /** @name Subelement management */
    //@{
    /** Sub type list
     *  List of different subelement types
     *  it is NOT a list of the subelements itself
     */
    virtual std::vector<const char*> getElementTypes(void) const;
    /// get the subelement by type and number
    virtual Data::Segment* getSubElement(const char* Type, unsigned long) const;
    //@}

    /** @name Save/restore */
    //@{
    void Save (Base::Writer &writer) const;
    void Restore(Base::XMLReader &reader);

    void SaveDocFile (Base::Writer &writer) const;
    void RestoreDocFile(Base::Reader &reader);
    unsigned int getMemSize (void) const;
    //@}

    /** @name Input/Output */
    //@{
    void read(const char *FileName);
    void write(const char *FileName) const;
    void importIges(const char *FileName);
    void importStep(const char *FileName);
    void importBrep(const char *FileName);
    void exportIges(const char *FileName) const;
    void exportStep(const char *FileName) const;
    void exportBrep(const char *FileName) const;
    void exportStl (const char *FileName) const;
    //@}

    /** @name Query*/
    //@{
    bool isNull() const;
    bool isValid() const;
    bool isClosed() const;
    //@}

    /** @name Boolean operation*/
    //@{
    TopoDS_Shape cut(TopoDS_Shape) const;
    TopoDS_Shape common(TopoDS_Shape) const;
    TopoDS_Shape fuse(TopoDS_Shape) const;
    TopoDS_Shape section(TopoDS_Shape) const;
    //@}

    /** Sweeping */
    //@{
    TopoDS_Shape makePipe(const TopoDS_Shape& profile) const;
    TopoDS_Shape makePipeShell(const TopTools_ListOfShape& profiles, const Standard_Boolean make_solid) const;
    TopoDS_Shape makePrism(const gp_Vec&) const;
    TopoDS_Shape revolve(const gp_Ax1&, double d) const;
    TopoDS_Shape makeThickSolid(const TopTools_ListOfShape& remFace,
        Standard_Real offset, Standard_Real tolerance) const;
    //@}

    /** @name Manipulation*/
    //@{
    TopoDS_Shape transformGeometry(const Base::Matrix4D&) const;
    void transformShape(const Base::Matrix4D&);
    TopoDS_Shape toNurbs() const;
    void sewShape();
    //@}

    /** @name Getting basic geometric entities */
    //@{
    void getFaces(std::vector<Base::Vector3d> &Points,std::vector<FacetTopo> &Topo,
        float Accuracy, uint16_t flags=0) const;
    void setFaces(const std::vector<Base::Vector3d> &Points,
                  const std::vector<FacetTopo> &Topo, float Accuracy=1.0e-06);
    //@}

    TopoDS_Shape _Shape;
};

} //namespace Part


#endif // PART_TOPOSHAPE_H
