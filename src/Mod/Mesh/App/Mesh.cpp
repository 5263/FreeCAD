/***************************************************************************
 *   Copyright (c) Juergen Riegel         <juergen.riegel@web.de>          *
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
#ifndef _PreComp_
#endif


#include "Mesh.h"
#include "Core/MeshKernel.h"

using namespace Mesh;



void DataWithPropertyBag::Add(PropertyBag* New, const char* Name)
{
  _Properties[Name] = New;
}

void DataWithPropertyBag::Remove(const char* Name)
{
  
}


void DataWithPropertyBag::RemoveType(const char* TypeName)
{

}


PropertyBag* DataWithPropertyBag::Get(const char* Name)
{
  return _Properties[Name];
}


PropertyBag* DataWithPropertyBag::GetFirstOfType(const char* TypeName)
{
  for( std::map<std::string,PropertyBag*>::iterator It = _Properties.begin();It!=_Properties.end();It++)
    if( strcmp(It->second->GetType(),TypeName)==0 )
      return It->second;

  return 0;
}


std::list<PropertyBag*> DataWithPropertyBag::GetAllOfType(const char* TypeName)
{
  std::list<PropertyBag*> List;

  for( std::map<std::string,PropertyBag*>::iterator It = _Properties.begin();It!=_Properties.end();It++)
    if( strcmp(It->second->GetType(),TypeName)==0 )
      List.push_back( It->second);

    return List;
}


std::set<std::string> DataWithPropertyBag::GetAllTypes(void)
{
  std::set<std::string> Set;
  
  for( std::map<std::string,PropertyBag*>::iterator It = _Properties.begin();It!=_Properties.end();It++)
    Set.insert( It->second->GetType());

  return Set;
}





