/***************************************************************************
 *   Copyright (c) 2009 J�rgen Riegel <juergen.riegel@web.de>              *
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

#include "TaskDlgSimulate.h"
#include "TaskRobot6Axis.h"
#include "TaskTrajectory.h"

using namespace RobotGui;


//**************************************************************************
//**************************************************************************
// TaskDialog
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TaskDlgSimulate::TaskDlgSimulate(Robot::RobotObject *pcRobotObject,Robot::TrajectoryObject *pcTrajectoryObject)
    : TaskDialog()
{
    TaskRobot6Axis *rob  = new TaskRobot6Axis(pcRobotObject);
    TaskTrajectory *trac = new TaskTrajectory(pcRobotObject,pcTrajectoryObject);
    
    QObject::connect(trac ,SIGNAL(axisChanged(float,float,float,float,float,float)),
                     rob  ,SLOT  (setAxis(float,float,float,float,float,float)));

    Content.push_back(rob);
    Content.push_back(trac);
}

TaskDlgSimulate::~TaskDlgSimulate()
{

}

//==== calls from the TaskView ===============================================================


void TaskDlgSimulate::open()
{
    
}
void TaskDlgSimulate::clicked(QAbstractButton *)
{
    
}
void TaskDlgSimulate::accept()
{
    
}
void TaskDlgSimulate::reject()
{
    
}

void TaskDlgSimulate::helpRequested()
{

}


#include "moc_TaskDlgSimulate.cpp"
