/***************************************************************************
 *   Copyright (c) Berthold Grupp          2005                            *
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


#ifndef MESH_SETOPERATIONS_H
#define MESH_SETOPERATIONS_H

#ifndef _PreComp_
# include <Wm3IntrSegment3Plane3.h>
# include <Wm3Vector3.h>
# include <list>
# include <map>
#endif

#include <Wm3Plane3.h>

#include "MeshKernel.h"
#include "Elements.h"
#include "Iterator.h"
#include "Visitor.h"

#include <Base/Builder3D.h>

// forward declarations

namespace Base
{
  class Vector2D;
  class Vector3D;
  class ViewProjMethod;
  class Polygon2D;
}

using Base::Vector2D;
using Base::Vector3D;
using Base::ViewProjMethod;
using Base::Polygon2D;

namespace MeshCore
{

class MeshGeomFacet;
class MeshGeomEdge;
class MeshKernel;
class MeshFacetGrid;
class MeshFacetArray;
class MeshFacetIterator;

/**
 * The MeshAlgorithm class provides algorithms base on meshes.
 */
class AppMeshExport SetOperations
{
public:
  enum OperationType { Union, Intersect, Difference };

  /// Construction
  SetOperations (MeshKernel &cutMesh1, MeshKernel &cutMesh2, MeshKernel &result, OperationType opType, float minDistanceToPoint = 1e-5f);
  /// Destruction
  virtual ~SetOperations (void);

public:

  /** Cut this mesh with another one. The result is a list of polylines
   * If the distance of the polyline to one of the points is less than minDistanceToPoint the polyline goes direct to the point
   */
  // void Do (MeshKernel &cutMesh1, MeshKernel &cutMesh2, float minDistanceToPoint = 1.0e-5f );
  void Do ();

protected:
  MeshKernel   &_cutMesh0;             /** Mesh for set operations source 1 */
  MeshKernel   &_cutMesh1;             /** Mesh for set operations source 2 */
  MeshKernel   &_resultMesh;           /** Result mesh */
  OperationType _operationType;        /** Set Operation Type */
  float         _minDistanceToPoint;   /** Minimal distance to facet corner points */
  float         _saveMinMeshDistance;

private:
  // Helper class cutting edge to his two attached facets
  class Edge
  {
    public:
      MeshPoint         pt1, pt2;              // edge
      
      Edge (MeshPoint p1, MeshPoint p2)
      {
        if (p1 < p2)
        {
          pt1 = p1;
          pt2 = p2;
        }
        else
        {
          pt2 = p1;
          pt1 = p2;
        }
      }

      bool operator == (const Edge &edge) const
      {
        return (pt1 == edge.pt1) && (pt2 == edge.pt2);
      }

      bool operator < (const Edge &edge) const
      {
        if (pt1 == edge.pt1)
          return pt2 < edge.pt2;
        else
          return pt1 < edge.pt1;
      }
  };

  class EdgeInfo : public Edge
  {
    public:
      int               fcounter[2];           // counter of facets attacted to the edge
      MeshGeomFacet     facets[2][2];          // Geom-Facets attached to the edge
      unsigned long     facet[2];              // underlying Facet-Index

      EdgeInfo (MeshPoint p1, MeshPoint p2) : Edge(p1, p2)
      {
        fcounter[0] = 0;
        fcounter[1] = 0;
      }
  };

  class CollectFacetVisitor : public MeshFacetVisitor
  {
    public:
      std::vector<unsigned long> &_facets;
      MeshKernel                 &_mesh;
      std::set<EdgeInfo>         &_edges;
      int                         _side;
      float                       _mult;
      int                         _addFacets; // 0: add facets to the result 1: do not add facets to the result
      Base::Builder3D& _builder;

      CollectFacetVisitor (MeshKernel& mesh, std::vector<unsigned long>& facets, std::set<EdgeInfo>& edges, int side, float mult, Base::Builder3D& builder);
      bool Visit (MeshFacet &rclFacet, const MeshFacet &rclFrom, unsigned long ulFInd, unsigned long ulLevel);
      bool AllowVisit (MeshFacet& rclFacet, MeshFacet& rclFrom, unsigned long ulFInd, unsigned long ulLevel, unsigned short neighbourIndex);
  };

  /** all edges */
  std::set<EdgeInfo> _edges;
  /** map from facet index to his cutted points (mesh 1 and mesh 2) Key: Facet-Index  Value: List of MeshPoints */
  std::map<unsigned long, std::list<MeshPoint> > _facet2points[2];
  /** Facets collected from region growing */
  std::vector<MeshGeomFacet> _facetsOf[2];

  std::vector<MeshGeomFacet> _newMeshFacets[2];

  /** Cut mesh 1 with mesh 2 */
  void Cut (std::set<unsigned long>& facetsNotCuttingEdge0, std::set<unsigned long>& facetsCuttingEdge1);
  /** Trianglute each facets cutted with his cutting points */
  void TriangulateMesh (MeshKernel &cutMesh, int side);
  /** search facets for adding (with region growing) */
  void CollectFacets (int side, float mult);


  /** visual debugger */
  Base::Builder3D _builder;

};


}; // namespace MeshCore 

#endif  // MESH_SETOPERATIONS_H 
