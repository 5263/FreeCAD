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


#include "PreCompiled.h"

#ifndef _PreComp_
#endif

#include <Base/Writer.h>

#include "kdl/chain.hpp"
#include "kdl/path_line.hpp"
#include "kdl/path_roundedcomposite.hpp"
#include "kdl/trajectory_composite.hpp"
#include "kdl/rotational_interpolation_sa.hpp"
#include "kdl/VelocityProfile_Trap.hpp"
#include "kdl/Trajectory_Segment.hpp"

#include "Trajectory.h"
#include "RobotAlgos.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
    #define M_PI    3.14159265358979323846 /* pi */
#endif

#ifndef M_PI_2
    #define M_PI_2  1.57079632679489661923 /* pi/2 */
#endif

using namespace Robot;
using namespace Base;
//using namespace KDL;


TYPESYSTEM_SOURCE(Robot::Trajectory , Base::Persistence);

Trajectory::Trajectory()
:pcTrajectory(0)
{

}

Trajectory::~Trajectory()
{
    for(std::vector<Waypoint*>::iterator it = vpcWaypoints.begin();it!=vpcWaypoints.end();++it)
        delete ( *it );
}


double Trajectory::getLength(void) const
{
    if(pcTrajectory)
        return pcTrajectory->GetPath()->PathLength();
    else
        return 0;
}

double Trajectory::getDuration(void) const
{
    if(pcTrajectory)
        return pcTrajectory->Duration();
    else
        return 0;
}

Placement Trajectory::getPosition(double time)
{
    if(pcTrajectory)
        return Placement(toPlacement(pcTrajectory->Pos(time)));
    else
        return Placement();
}

double Trajectory::getVelocity(double time)
{
    if(pcTrajectory){
        KDL::Vector vec = pcTrajectory->Vel(time).vel;
        Base::Vector3d vec2(vec[0],vec[1],vec[2]);
        return vec2.Length();
    }else
        return 0;
}


void Trajectory::generateTrajectory(void)
{
    // delete the old and create a new one
    if(pcTrajectory) delete (pcTrajectory);
    pcTrajectory = new KDL::Trajectory_Composite();

    // pointer to the pieces while iterating
    KDL::Trajectory_Segment     *pcTrak;
    KDL::Path                   *pcPath;
    KDL::VelocityProfile        *pcVelPrf;
    KDL::Frame                  Last;

    // handle the first waypoint special
    bool first=true;

    for(std::vector<Waypoint*>::const_iterator it = vpcWaypoints.begin();it!=vpcWaypoints.end();++it){
        if(first){
            Last = toFrame((*it)->EndPos);
            first = false;
        }else{
            switch((*it)->Type){
                case Waypoint::LINE:
                case Waypoint::PTP:{
                    KDL::Frame Next = toFrame((*it)->EndPos);
                    pcPath = new KDL::Path_Line(Last,
                                                Next,
                                                new KDL::RotationalInterpolation_SingleAxis,
                                                1.0
                                                );
                    pcVelPrf = new KDL::VelocityProfile_Trap((*it)->Velocity,(*it)->Velocity);
                    pcVelPrf->SetProfile(0,pcPath->PathLength());
                    pcTrak = new KDL::Trajectory_Segment(pcPath,pcVelPrf);
                    Last = Next;
                    break;}
                case Waypoint::WAIT:
                    break;
                    
            }
            pcTrajectory->Add(pcTrak);
        }
    }

}

void Trajectory::addWaypoint(const Waypoint &WPnt)
{
    Waypoint *tmp = new Waypoint(WPnt);
    vpcWaypoints.push_back(tmp);
}




unsigned int Trajectory::getMemSize (void) const
{
	return 0;
}

void Trajectory::Save (Writer &/*writer*/) const
{
}

void Trajectory::Restore(XMLReader &/*reader*/)
{
}




 