/***************************************************************************
 *   Copyright (c) 2005 Imetric 3D GmbH                                    *
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


#include "PreCompiled.h"
#include "Definitions.h"
#include "Iterator.h"

#include "Mesh.h"
#include "MeshIO.h"

#include <Base/Sequencer.h>

#include <math.h>



struct NODE {float x, y, z;};
struct TRIA {int iV[3];};
struct QUAD {int iVer[4];};

using namespace Mesh;


MeshSTL::MeshSTL (MeshKernel &rclM)
: _rclMesh(rclM)
{
}

bool MeshSTL::Load (FileStream &rstrIn)
{
  char szBuf[200];

  if ((rstrIn.IsOpen() == FALSE) || (rstrIn.IsBad() == TRUE))
    return FALSE;

  rstrIn.SetPosition(80);
  if (rstrIn.Read(szBuf, 80) == FALSE)
    return FALSE;
  szBuf[80] = 0; 

  if ((strstr(szBuf, "SOLID") == NULL)  && (strstr(szBuf, "FACET") == NULL)    && (strstr(szBuf, "NORMAL") == NULL) &&
      (strstr(szBuf, "VERTEX") == NULL) && (strstr(szBuf, "ENDFACET") == NULL) && (strstr(szBuf, "ENDLOOP") == NULL))
  {  // wahrscheinlich stl binaer
    rstrIn.SetPosition(0);
    return LoadBinary(rstrIn);
  }
  else
  {  // stl ascii
    rstrIn.SetPosition(0);
    return LoadAscii(rstrIn);
  }

  return TRUE;
}

bool MeshSTL::LoadAscii (FileStream &rstrIn)
{
  char            szLine[200], szKey1[200], szKey2[200];
  unsigned long           ulVertexCt, ulCt;
  float            fX, fY, fZ;
  MeshGeomFacet clFacet;
  std::vector<MeshGeomFacet>  clFacetAry;

  if ((rstrIn.IsOpen() == FALSE) || (rstrIn.IsBad() == TRUE))
    return FALSE;

  ulCt = rstrIn.FileSize();

  ulVertexCt = 0;
  while ((rstrIn.IsEof() == FALSE) && (rstrIn.IsBad() == FALSE))
  {
    rstrIn.ReadLine(szLine, 200);
    if (strncmp(szLine, "FACET", 5) == 0)  // normale
    {
      if (sscanf(szLine, "%s %s %f %f %f", szKey1, szKey2, &fX, &fY, &fZ) == 5)
        clFacet.SetNormal(Vector3D(fX, fY, fZ));
    }
    else if (strncmp(szLine, "VERTEX", 6) == 0)  // vertex
    {
      if (sscanf(szLine, "%s %f %f %f", szKey1, &fX, &fY, &fZ) == 4)
      {
        clFacet._aclPoints[ulVertexCt++].Set(fX, fY, fZ);
        if (ulVertexCt == 3)
        {
          ulVertexCt = 0;
          clFacetAry.push_back(clFacet);
        }
      }
    }

  }

  _rclMesh = clFacetAry;  

  return TRUE;  
}


bool MeshSTL::LoadBinary (FileStream &rstrIn)
{
  char                       szInfo[80];
  Vector3D                   clVects[4];
  unsigned short                     usAtt; 
  unsigned long                      ulCt;

  if ((rstrIn.IsOpen() == FALSE) || (rstrIn.IsBad() == TRUE))
    return FALSE;

  rstrIn.Read(szInfo, sizeof(szInfo));
 
  rstrIn.Read((char*)&ulCt, sizeof(ulCt));
  if (rstrIn.IsBad() == TRUE)
    return FALSE;

  unsigned long ulSize = rstrIn.FileSize(); 
  unsigned long ulFac = (ulSize - (80 + sizeof(unsigned long)))/50;

	if (ulCt > ulFac)
		return FALSE;// not a valid STL file

  Base::Sequencer().start("create mesh structure...", ulCt + 1);

  MeshPointBuilder  clMap;
  clMap.resize(ulCt*3);

  unsigned long k = 0;

  // n-Facets einlesen
  for (unsigned long i = 0; i < ulCt; i++)
  {
    // Normale, Eckpunkte
    rstrIn.Read((char*)&clVects, sizeof(clVects));

    if (rstrIn.IsBad() == TRUE)
      return FALSE;

    // Umlaufrichtung an Normale anpassen
    if ((((clVects[2] - clVects[1]) % (clVects[3] - clVects[1])) * clVects[0]) >= 0.0f)
    {
      clMap[k++].Set(0, i, clVects[1]);
      clMap[k++].Set(1, i, clVects[2]);
      clMap[k++].Set(2, i, clVects[3]);
    }
    else
    {
      clMap[k++].Set(0, i, clVects[1]);
      clMap[k++].Set(1, i, clVects[3]);
      clMap[k++].Set(2, i, clVects[2]);
    }

    // ueberlesen Attribut
    rstrIn.Read((char*)&usAtt, sizeof(usAtt));

    Base::Sequencer().next();
  } 

  _rclMesh.Assign(clMap);

  Base::Sequencer().stop();
 
  return TRUE;
}

bool MeshSTL::SaveAscii (FileStream &rstrOut) const
{
  MeshFacetIterator      clIter(_rclMesh), clEnd(_rclMesh);  
  const MeshGeomFacet *pclFacet;
  unsigned long                  i, ulCtFacet;
  char                   szBuf[200]; 

  if ((rstrOut.IsOpen() == FALSE) || (rstrOut.IsBad() == TRUE) ||
      (_rclMesh.CountFacets() == 0))
  {
    return FALSE;
  }

  strcpy(szBuf, "solid MESH\n");
  rstrOut.Write(szBuf, strlen(szBuf));

  clIter.Begin();
  clEnd.End();
  ulCtFacet = 0;
  while (clIter < clEnd)
  {
    pclFacet = &(*clIter);
      
    // normale
    sprintf(szBuf, "  facet normal %.4f %.4f %.4f\n", pclFacet->GetNormal().x,
             pclFacet->GetNormal().y,  pclFacet->GetNormal().z);
    rstrOut.Write(szBuf, strlen(szBuf));

    strcpy(szBuf, "    outer loop\n");
    rstrOut.Write(szBuf, strlen(szBuf));

    for (i = 0; i < 3; i++)
    {
      sprintf(szBuf, "      vertex %.4f %.4f %.4f\n", pclFacet->_aclPoints[i].x,
              pclFacet->_aclPoints[i].y, pclFacet->_aclPoints[i].z);
      rstrOut.Write(szBuf, strlen(szBuf));
    }

    strcpy(szBuf, "    endloop\n");
    rstrOut.Write(szBuf, strlen(szBuf));

    strcpy(szBuf, "  endfacet\n");
    rstrOut.Write(szBuf, strlen(szBuf));

    ++clIter; 
  } 

  strcpy(szBuf, "endsolid MESH\n");
  rstrOut.Write(szBuf, strlen(szBuf));
  
  return TRUE;
}

bool MeshSTL::SaveBinary (FileStream &rstrOut) const
{
  MeshFacetIterator      clIter(_rclMesh), clEnd(_rclMesh);  
  const MeshGeomFacet *pclFacet;
  unsigned long                  i, ulCtFacet;
  unsigned short                 usAtt;
  char                   szInfo[80];

  if ((rstrOut.IsOpen() == FALSE) || (rstrOut.IsBad() == TRUE) ||
      (_rclMesh.CountFacets() == 0))
  {
    return FALSE;
  }
 
  strcpy(szInfo, "MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH\n");
  rstrOut.Write(szInfo, strlen(szInfo));
  rstrOut << (unsigned long)(_rclMesh.CountFacets());

  usAtt = 0;
  clIter.Begin();
  clEnd.End();
  ulCtFacet = 0;
  while (clIter < clEnd)
  {
    pclFacet = &(*clIter);
    // Normale
    rstrOut << float(pclFacet->GetNormal().x) <<
               float(pclFacet->GetNormal().y) <<
               float(pclFacet->GetNormal().z);

    // Eckpunkte
    for (i = 0; i < 3; i++)
    {
      rstrOut << float(pclFacet->_aclPoints[i].x) <<
                 float(pclFacet->_aclPoints[i].y) <<
                 float(pclFacet->_aclPoints[i].z);
    }

    // Attribut 
    rstrOut << usAtt;

    ++clIter;
  }

  return TRUE;
}

bool MeshInventor::Load (FileStream &rstrIn)
{
  char            szLine[200];
  unsigned long           i, ulCt, ulPoints[3];
  long            lSeparator;
  bool            bFlag;
  Vector3D        clPoint;
  float            fX, fY, fZ;
  MeshGeomFacet          clFacet;
  std::vector<MeshGeomFacet>  clFacetAry;
  std::vector<Vector3D>         aclPoints;


  if ((rstrIn.IsOpen() == FALSE) || (rstrIn.IsBad() == TRUE))
    return FALSE;

  ulCt = rstrIn.FileSize();
///*
  bFlag = TRUE;
  while ((rstrIn.IsEof() == FALSE) && (rstrIn.IsBad() == FALSE) && bFlag)
  {
      rstrIn.ReadLine(szLine ,200);
      // read the normals
      if (strncmp(szLine, "VECTOR [", 8) == 0){
          while ((rstrIn.IsEof() == FALSE) && (rstrIn.IsBad() == FALSE) && bFlag)
          {  
              rstrIn.ReadLine(szLine, 200);

              if (sscanf(szLine, "%f %f %f", &fX, &fY, &fZ) == 3){
                  clFacet.SetNormal(Vector3D(fX, fY, fZ));
                  clFacetAry.push_back(clFacet);

              }
              else{
                  bFlag = FALSE;
              }
          }
      }
  }

//*/
  bFlag = TRUE;
  while ((rstrIn.IsEof() == FALSE) && (rstrIn.IsBad() == FALSE) && bFlag)
  {
      rstrIn.ReadLine(szLine ,200);
      // read the points
      if (strncmp(szLine, "POINT [", 7) == 0){
          while ((rstrIn.IsEof() == FALSE) && (rstrIn.IsBad() == FALSE) && bFlag)
          {
              rstrIn.ReadLine(szLine, 200);
              if (sscanf(szLine, "%f  %f  %f", &(clPoint.x), &(clPoint.y), &(clPoint.z)) == 3)
              {
                 aclPoints.push_back(clPoint);
              }
              else{
                  bFlag = FALSE;
              }
          }
      }
  }

  ulCt = 0;
  bFlag = TRUE;
  while ((rstrIn.IsEof() == FALSE) && (rstrIn.IsBad() == FALSE) && bFlag)
  {
      rstrIn.ReadLine(szLine ,200);
      // read the points of the facets
      if (strncmp(szLine, "COORDINDEX [ ", 13) == 0){
          while ((rstrIn.IsEof() == FALSE) && (rstrIn.IsBad() == FALSE) && bFlag)
          {
              rstrIn.ReadLine(szLine, 200);
              if (sscanf(szLine, "%lu, %lu, %lu, %ld",
                  &ulPoints[0], &ulPoints[1], &ulPoints[2], &lSeparator) == 4)
              {
                  clFacet = clFacetAry[ulCt];
                  for (i = 0; i < 3; i++)
                  {
                    clFacet._aclPoints[i] = aclPoints[ulPoints[i]];
                  }
                  clFacetAry[ulCt++] = clFacet;
              }
              else
              {
                  bFlag = FALSE;
              }
          }
      }
  }

  _rclMesh = clFacetAry;
  return TRUE;
}

bool MeshInventor::Save (FileStream &rstrOut) const
{
  MeshFacetIterator      clIter(_rclMesh), clEnd(_rclMesh);
  MeshPointIterator      clPtIter(_rclMesh), clPtEnd(_rclMesh);
  const MeshGeomFacet* pclFacet;
  MeshFacet              clFacet;
  unsigned long                  i, ulCtFacet, ulAllFacets = _rclMesh.CountFacets();
  char                   szBuf[200]; 

  if ((rstrOut.IsOpen() == FALSE) || (rstrOut.IsBad() == TRUE) ||
      (_rclMesh.CountFacets() == 0))
  {
    return FALSE;
  }

  // Einleitung
  strcpy(szBuf, "#Inventor V2.1 ascii\n\n");
  rstrOut.Write(szBuf, strlen(szBuf));
  sprintf(szBuf, "# Triangulation Data contains %lu Points and %lu Facets\n",
      _rclMesh.CountPoints(), _rclMesh.CountFacets());
  rstrOut.Write(szBuf, strlen(szBuf));
  strcpy(szBuf, "Separator { \n\n\n");
  rstrOut.Write(szBuf, strlen(szBuf));
  strcpy(szBuf, " Label {\n");
  rstrOut.Write(szBuf, strlen(szBuf));
  strcpy(szBuf, "  label triangle mesh\n  }\n");
  rstrOut.Write(szBuf, strlen(szBuf));
///*
  // hier stehen die Normalen der Facets
  strcpy(szBuf, "Normal { \n");
  rstrOut.Write(szBuf, strlen(szBuf));
  strcpy(szBuf, "vector [\n");
  rstrOut.Write(szBuf, strlen(szBuf));

  clIter.Begin();
  clEnd.End();
  ulCtFacet = 0;

  pclFacet = &(*clIter);
  sprintf(szBuf, "%.6f  %.6f  %.6f", pclFacet->GetNormal().x,
          pclFacet->GetNormal().y, pclFacet->GetNormal().z);
  rstrOut.Write(szBuf, strlen(szBuf));
  ++clIter;

  while (clIter < clEnd)
  {
      pclFacet = &(*clIter);

      sprintf(szBuf, ",\n%.6f  %.6f  %.6f", pclFacet->GetNormal().x, 
              pclFacet->GetNormal().y, pclFacet->GetNormal().z);
      rstrOut.Write(szBuf, strlen(szBuf));
      ++clIter;
  }
  strcpy(szBuf, "\n]\n}\n");
  rstrOut.Write(szBuf, strlen(szBuf));
//*/
  // hier stehen die Koordinaten der Punkte
  strcpy(szBuf, " NormalBinding {\n  value PER_FACE\n }\n");
  rstrOut.Write(szBuf, strlen(szBuf));
  strcpy(szBuf, "Coordinate3 { \npoint [\n");
  rstrOut.Write(szBuf, strlen(szBuf));

  clPtIter.Begin();
  clPtEnd.End();
  ulCtFacet = 0;

  sprintf(szBuf, "%.6f  %.6f  %.6f", clPtIter->x, clPtIter->y, clPtIter->z);
  rstrOut.Write(szBuf, strlen(szBuf));
  ++clPtIter;

  while (clPtIter < clPtEnd)
  {
      sprintf(szBuf, ",\n%.6f  %.6f  %.6f", clPtIter->x, clPtIter->y, clPtIter->z);
      rstrOut.Write(szBuf, strlen(szBuf));
      ++clPtIter;
  }
  strcpy(szBuf, "\n]\n}\n");
  rstrOut.Write(szBuf, strlen(szBuf));

  // hier stehen die Facets mit den Punktindizes
  strcpy(szBuf, "IndexedFaceSet { \n");
  rstrOut.Write(szBuf, strlen(szBuf));
  strcpy(szBuf, "coordIndex [ \n");
  rstrOut.Write(szBuf, strlen(szBuf));


  i = 0;
  while (i < ulAllFacets - 1)
  {
    clFacet = _rclMesh._aclFacetArray[i];

    sprintf(szBuf, "%lu, %lu, %lu, %d,\n", clFacet._aulPoints[0],
              clFacet._aulPoints[1], clFacet._aulPoints[2], -1);
    rstrOut.Write(szBuf, strlen(szBuf));
    
    ++i; 
  } 

  clFacet = _rclMesh._aclFacetArray[ulAllFacets - 1];
  sprintf(szBuf, "%lu, %lu, %lu, %d\n", clFacet._aulPoints[0],
            clFacet._aulPoints[1], clFacet._aulPoints[2], - 1);
  rstrOut.Write(szBuf, strlen(szBuf));

  strcpy(szBuf, "]\n}\n");
  rstrOut.Write(szBuf, strlen(szBuf));

  strcpy(szBuf, "#End of Triangulation Data \n  }\n\n");
  rstrOut.Write(szBuf, strlen(szBuf));

  return TRUE;
}
