/***************************************************************************
 *   Copyright (c) 2007 Werner Mayer <wmayer@users.sourceforge.net>        *
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


#ifndef MESH_SEGMENT_H
#define MESH_SEGMENT_H

#include <vector>
#include "Facet.h"
#include "Core/Iterator.h"

namespace Mesh
{

class MeshObject;

class MeshExport Segment
{
public:
    Segment(MeshObject*);
    Segment(MeshObject*, const std::vector<unsigned long>& inds);
    void addIndices(const std::vector<unsigned long>& inds);
    const std::vector<unsigned long>& getIndices() const;

    const Segment& operator = (const Segment&);
    bool operator == (const Segment&) const;

private:
    MeshObject* _mesh;
    std::vector<unsigned long> _indices;

public:
    class MeshExport FacetIter
    {
    public:
        FacetIter(const Segment*, std::vector<unsigned long>::const_iterator);
        FacetIter(const FacetIter& fi);
        ~FacetIter();

        FacetIter& operator=(const FacetIter& fi);
        Facet& operator*();
        Facet* operator->();
        bool operator==(const FacetIter& fi) const;
        bool operator!=(const FacetIter& fi) const;
        FacetIter& operator++();
        FacetIter& operator--();
    private:
        void dereference();
        const Segment* _segment;
        Facet _facet;
        MeshCore::MeshFacetIterator _f_it;
        std::vector<unsigned long>::const_iterator _it;
    };

    class MeshExport ConstFacetIter
    {
    public:
        ConstFacetIter(const Segment*, std::vector<unsigned long>::const_iterator);
        ConstFacetIter(const ConstFacetIter& fi);
        ~ConstFacetIter();

        ConstFacetIter& operator=(const ConstFacetIter& fi);
        const Facet& operator*() const;
        const Facet* operator->() const;
        bool operator==(const ConstFacetIter& fi) const;
        bool operator!=(const ConstFacetIter& fi) const;
        ConstFacetIter& operator++();
        ConstFacetIter& operator--();
    private:
        void dereference();
        const Segment* _segment;
        Facet _facet;
        MeshCore::MeshFacetIterator _f_it;
        std::vector<unsigned long>::const_iterator _it;
    };

    FacetIter facets_begin()
    { return FacetIter(this, _indices.begin()); }
    FacetIter facets_end()
    { return FacetIter(this, _indices.end()); }

    ConstFacetIter facets_begin() const
    { return ConstFacetIter(this, _indices.begin()); }
    ConstFacetIter facets_end() const
    { return ConstFacetIter(this, _indices.end()); }
};

} // namespace Mesh


#endif // MESH_SEGMENT_H
