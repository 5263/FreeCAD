/***************************************************************************
 *   Copyright (c) 2008 Werner Mayer <wmayer[at]users.sourceforge.net>     *
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
# include <cfloat>
# include "InventorAll.h"
# include <QAction>
# include <QActionGroup>
# include <QApplication>
# include <QByteArray>
# include <QCursor>
# include <QList>
# include <QMenu>
# include <QMetaObject>
# include <QRegExp>
#endif
#include <Inventor/sensors/SoTimerSensor.h>

#include "NavigationStyle.h"
#include "View3DInventorViewer.h"
#include "Application.h"
#include "MenuManager.h"
#include "MouseModel.h"

using namespace Gui;

namespace Gui {
struct NavigationStyleP {
    int animationsteps;
    int animationdelta;
    SbVec3f focal1, focal2;
    SbRotation endRotation;
    SoTimerSensor * animsensor;

    NavigationStyleP()
    {
        this->animationsteps = 0;
    }
    static void viewAnimationCB(void * data, SoSensor * sensor);
};
}

NavigationStyleEvent::NavigationStyleEvent(const Base::Type& s)
  : QEvent(QEvent::User), t(s)
{
}

NavigationStyleEvent::~NavigationStyleEvent()
{
}

const Base::Type& NavigationStyleEvent::style() const
{ 
    return t;
}

#define PRIVATE(ptr) (ptr->pimpl)
#define PUBLIC(ptr) (ptr->pub)

TYPESYSTEM_SOURCE_ABSTRACT(Gui::NavigationStyle,Base::BaseClass);

NavigationStyle::NavigationStyle() : viewer(0), pcMouseModel(0)
{
    PRIVATE(this) = new NavigationStyleP();
    PRIVATE(this)->animsensor = new SoTimerSensor(NavigationStyleP::viewAnimationCB, this);
    initialize();
}

NavigationStyle::~NavigationStyle()
{
    finalize();
    if (PRIVATE(this)->animsensor->isScheduled())
        PRIVATE(this)->animsensor->unschedule();
    delete PRIVATE(this)->animsensor;
    delete PRIVATE(this);
}

NavigationStyle& NavigationStyle::operator = (const NavigationStyle& ns)
{
    this->panningplane = ns.panningplane;
    this->menuenabled = ns.menuenabled;
    this->spinanimatingallowed = ns.spinanimatingallowed;
    return *this;
}

void NavigationStyle::setViewer(View3DInventorViewer* view)
{
    this->viewer = view;
}

void NavigationStyle::initialize()
{
    this->currentmode = NavigationStyle::IDLE;
    this->prevRedrawTime = SbTime::getTimeOfDay();
    this->spinanimatingallowed = TRUE;
    this->spinsamplecounter = 0;
    this->spinincrement = SbRotation::identity();
    this->spinRotation.setValue(SbVec3f(0, 0, -1), 0);

    // FIXME: use a smaller sphere than the default one to have a larger
    // area close to the borders that gives us "z-axis rotation"?
    // 19990425 mortene.
    this->spinprojector = new SbSphereSheetProjector(SbSphere(SbVec3f(0, 0, 0), 0.8f));
    SbViewVolume volume;
    volume.ortho(-1, 1, -1, 1, -1, 1);
    this->spinprojector->setViewVolume(volume);

    this->log.size = 16;
    this->log.position = new SbVec2s [ 16 ];
    this->log.time = new SbTime [ 16 ];
    this->log.historysize = 0;

    this->menuenabled = TRUE;
    this->button1down = FALSE;
    this->button2down = FALSE;
    this->button3down = FALSE;
    this->ctrldown = FALSE;
    this->shiftdown = FALSE;
    this->altdown = FALSE;
}

void NavigationStyle::finalize()
{
    delete this->spinprojector;
    delete[] this->log.position;
    delete[] this->log.time;
}

void NavigationStyle::interactiveCountInc(void)
{
    viewer->interactiveCountInc();
}

void NavigationStyle::interactiveCountDec(void)
{
    viewer->interactiveCountDec();
}

int NavigationStyle::getInteractiveCount(void) const
{
    return viewer->getInteractiveCount();
}

SbBool NavigationStyle::isViewing(void) const
{
    return viewer->isViewing();
}

void NavigationStyle::setViewing(SbBool enable)
{
    viewer->setViewing(enable);
}

SbBool NavigationStyle::isSeekMode(void) const
{
    return viewer->isSeekMode();
}

void NavigationStyle::setSeekMode(SbBool enable)
{
    viewer->setSeekMode(enable);
}

SbBool NavigationStyle::seekToPoint(const SbVec2s screenpos)
{
    return viewer->seekToPoint(screenpos);
}

void NavigationStyle::seekToPoint(const SbVec3f& scenepos)
{
    viewer->seekToPoint(scenepos);
}

void NavigationStyle::setCameraOrientation(const SbRotation& rot)
{
    SoCamera* cam = viewer->getCamera();
    if (cam == 0) return;

    // Find global coordinates of focal point.
    SbVec3f direction;
    cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
    PRIVATE(this)->focal1 = cam->position.getValue() +
                            cam->focalDistance.getValue() * direction;
    PRIVATE(this)->focal2 = PRIVATE(this)->focal1;
    SoGetBoundingBoxAction action(viewer->getViewportRegion());
    action.apply(viewer->getSceneGraph());
    SbBox3f box = action.getBoundingBox();
    if (!box.isEmpty()) {
        rot.multVec(SbVec3f(0, 0, -1), direction);
        //float s = (this->focal1 - box.getCenter()).dot(direction);
        //this->focal2 = box.getCenter() + s * direction;
        // setting the center of the overall bounding box as the future focal point
        // seems to be a satisfactory solution
        PRIVATE(this)->focal2 = box.getCenter();
    }

    if (PRIVATE(this)->animsensor->isScheduled()) {
        PRIVATE(this)->animsensor->unschedule();
    }

    if (isAnimationEnabled()) {
        // get the amount of movement
        SbVec3f dir1, dir2;
        SbRotation cam_rot = cam->orientation.getValue();
        cam_rot.multVec(SbVec3f(0, 0, -1), dir1);
        rot.multVec(SbVec3f(0, 0, -1), dir2);
        float val = 0.5f*(1.0f + dir1.dot(dir2)); // value in range [0,1]
        int div = (int)(val * 20.0f);
        int steps = 20-div; // do it with max. 20 steps

        // check whether a movement is required
        if (steps > 0) {
            PRIVATE(this)->endRotation = rot; // this is the final camera orientation
            this->spinRotation = cam_rot;
            PRIVATE(this)->animationsteps = 5;
            PRIVATE(this)->animationdelta = std::max<int>(100/steps, 5);
            PRIVATE(this)->animsensor->setBaseTime(SbTime::getTimeOfDay());
            PRIVATE(this)->animsensor->schedule();
        }
        else {
            // due to possible round-off errors make sure that the
            // exact orientation is set
            cam->orientation.setValue(rot);
        }
    }
    else {
        // set to the given rotation
        cam->orientation.setValue(rot);
        cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
        cam->position = PRIVATE(this)->focal2 - cam->focalDistance.getValue() * direction;
    }
}

void NavigationStyleP::viewAnimationCB(void * data, SoSensor * sensor)
{
    NavigationStyle* that = reinterpret_cast<NavigationStyle*>(data);
    if (PRIVATE(that)->animationsteps > 0) {
        // here the camera rotates from the current rotation to a given
        // rotation (e.g. the standard views). To get this movement animated
        // we calculate an interpolated rotation and update the view after
        // each step
        float step = std::min<float>((float)PRIVATE(that)->animationsteps/100.0f, 1.0f);
        SbRotation slerp = SbRotation::slerp(that->spinRotation, PRIVATE(that)->endRotation, step);
        SbVec3f focalpoint = (1.0f-step)*PRIVATE(that)->focal1 + step*PRIVATE(that)->focal2;
        SoCamera* cam = that->viewer->getCamera();
        SbVec3f direction;
        cam->orientation.setValue(slerp);
        cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
        cam->position = focalpoint - cam->focalDistance.getValue() * direction;

        PRIVATE(that)->animationsteps += PRIVATE(that)->animationdelta;
        if (PRIVATE(that)->animationsteps > 100) {
            // now we have reached the end of the movement
            PRIVATE(that)->animationsteps=0;
            PRIVATE(that)->animsensor->unschedule();
            // set to the actual given rotation
            cam->orientation.setValue(PRIVATE(that)->endRotation);
            cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
            cam->position = PRIVATE(that)->focal2 - cam->focalDistance.getValue() * direction;
        }
    }
}

void NavigationStyle::boxZoom(const SbBox2s& box)
{
    SoCamera* cam = viewer->getCamera();
    if (!cam) return; // no camera 
    const SbViewportRegion & vp = viewer->getViewportRegion();
    SbViewVolume vv = cam->getViewVolume(vp.getViewportAspectRatio());

    short sizeX,sizeY;
    box.getSize(sizeX, sizeY);
    SbVec2s size = vp.getViewportSizePixels();

    // The bbox must not be empty i.e. width and length is zero, but it is possible that
    // either width or length is zero
    if (sizeX == 0 && sizeY == 0) 
        return;

    // Get the new center in normalized pixel coordinates
    short xmin,xmax,ymin,ymax;
    box.getBounds(xmin,ymin,xmax,ymax);
    const SbVec2f center((float) ((xmin+xmax)/2) / (float) SoQtMax((int)(size[0] - 1), 1),
                         (float) (size[1]-(ymin+ymax)/2) / (float) SoQtMax((int)(size[1] - 1), 1));

    SbPlane plane = vv.getPlane(cam->focalDistance.getValue());
    panCamera(cam,vp.getViewportAspectRatio(),plane, SbVec2f(0.5,0.5), center);

    // Set height or height angle of the camera
    float scaleX = (float)sizeX/(float)size[0];
    float scaleY = (float)sizeY/(float)size[1];
    float scale = std::max<float>(scaleX, scaleY);
    if (cam && cam->getTypeId() == SoOrthographicCamera::getClassTypeId()) {
        float height = static_cast<SoOrthographicCamera*>(cam)->height.getValue() * scale;
        static_cast<SoOrthographicCamera*>(cam)->height = height;
    }
    else if (cam && cam->getTypeId() == SoPerspectiveCamera::getClassTypeId()) {
        float height = static_cast<SoPerspectiveCamera*>(cam)->heightAngle.getValue() / 2.0f;
        height = 2.0f * atan(tan(height) * scale);
        static_cast<SoPerspectiveCamera*>(cam)->heightAngle = height;
    }
}

void NavigationStyle::viewAll()
{
    // Get the bounding box of the scene
    SoGetBoundingBoxAction action(viewer->getViewportRegion());
    action.apply(viewer->getSceneGraph());
    SbBox3f box = action.getBoundingBox();
    if (box.isEmpty()) return;

#if 0
    // check whether the box is very wide or tall, if not do nothing
    float box_width, box_height, box_depth;
    box.getSize( box_width, box_height, box_depth );
    if (box_width < 5.0f*box_height && box_width < 5.0f*box_depth && 
        box_height < 5.0f*box_width && box_height < 5.0f*box_depth && 
        box_depth < 5.0f*box_width && box_depth < 5.0f*box_height )
        return;
#endif

    SoCamera* cam = viewer->getCamera();
    if (!cam) return;

    SbViewVolume  vol = cam->getViewVolume();
    if (vol.ulf == vol.llf)
        return; // empty frustum (no view up vector defined)
    SbVec2f s = vol.projectBox(box);
    SbVec2s size = viewer->getSize();

    SbVec3f pt1, pt2, pt3, tmp;
    vol.projectPointToLine( SbVec2f(0.0f,0.0f), pt1, tmp );
    vol.projectPointToLine( SbVec2f(s[0],0.0f), pt2, tmp );
    vol.projectPointToLine( SbVec2f(0.0f,s[1]), pt3, tmp );

    float cam_width = (pt2-pt1).length();
    float cam_height = (pt3-pt1).length();

    // add a small border
    cam_height = 1.08f * std::max<float>((cam_width*(float)size[1])/(float)size[0],cam_height);

    float aspect = cam->aspectRatio.getValue();

    if (cam->getTypeId() == SoPerspectiveCamera::getClassTypeId()) {
        // set the new camera position dependent on the occupied space of projected bounding box
        //SbVec3f direction = cam->position.getValue() - box.getCenter();
        //float movelength = direction.length();
        //direction.normalize();
        //float fRatio = getViewportRegion().getViewportAspectRatio();
        //if ( fRatio > 1.0f ) {
        //  float factor = std::max<float>(s[0]/fRatio,s[1]);
        //  movelength = factor * movelength;
        //}
        //else {
        //    float factor = std::max<float>(s[0],s[1]/fRatio);
        //    movelength = factor * movelength;
        //}
        //cam->position.setValue(box.getCenter() + direction * movelength);
    }
    else if (cam->getTypeId() == SoOrthographicCamera::getClassTypeId()) {
        SoOrthographicCamera* ocam = (SoOrthographicCamera *)cam;  // safe downward cast, knows the type
        if (aspect < 1.0f)
            ocam->height = cam_height / aspect;
        else
            ocam->height = cam_height;
    }
}

/** Rotate the camera by the given amount, then reposition it so we're
 * still pointing at the same focal point.
 */
void NavigationStyle::reorientCamera(SoCamera * cam, const SbRotation & rot)
{
    if (cam == NULL) return;

    // Find global coordinates of focal point.
    SbVec3f direction;
    cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
    SbVec3f focalpoint = cam->position.getValue() +
                         cam->focalDistance.getValue() * direction;

    // Set new orientation value by accumulating the new rotation.
    cam->orientation = rot * cam->orientation.getValue();

    // Reposition camera so we are still pointing at the same old focal point.
    cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
    cam->position = focalpoint - cam->focalDistance.getValue() * direction;
}

void NavigationStyle::panCamera(SoCamera * cam, float aspectratio, const SbPlane & panplane,
                                const SbVec2f & currpos, const SbVec2f & prevpos)
{
    if (cam == NULL) return; // can happen for empty scenegraph
    if (currpos == prevpos) return; // useless invocation


    // Find projection points for the last and current mouse coordinates.
    SbViewVolume vv = cam->getViewVolume(aspectratio);
    SbLine line;
    vv.projectPointToLine(currpos, line);
    SbVec3f current_planept;
    panplane.intersect(line, current_planept);
    vv.projectPointToLine(prevpos, line);
    SbVec3f old_planept;
    panplane.intersect(line, old_planept);

    // Reposition camera according to the vector difference between the
    // projected points.
    cam->position = cam->position.getValue() - (current_planept - old_planept);
}

void NavigationStyle::pan(SoCamera* camera)
{
    // The plane we're projecting the mouse coordinates to get 3D
    // coordinates should stay the same during the whole pan
    // operation, so we should calculate this value here.
    if (camera == NULL) { // can happen for empty scenegraph
        this->panningplane = SbPlane(SbVec3f(0, 0, 1), 0);
    }
    else {
        const SbViewportRegion & vp = viewer->getViewportRegion();
        SbViewVolume vv = camera->getViewVolume(vp.getViewportAspectRatio());
        this->panningplane = vv.getPlane(camera->focalDistance.getValue());
    }
}

void NavigationStyle::panToCenter(const SbPlane & pplane, const SbVec2f & currpos)
{
    const SbViewportRegion & vp = viewer->getViewportRegion();
    float ratio = vp.getViewportAspectRatio();
    panCamera(viewer->getCamera(), ratio, pplane, SbVec2f(0.5,0.5), currpos);
}

/** Dependent on the camera type this will either shrink or expand the
 * height of the viewport (orthogonal camera) or move the camera
 * closer or further away from the focal point in the scene.
 */
void NavigationStyle::zoom(SoCamera * cam, float diffvalue)
{
    if (cam == NULL) return; // can happen for empty scenegraph
    SoType t = cam->getTypeId();
    SbName tname = t.getName();

    // This will be in the range of <0, ->>.
    float multiplicator = float(exp(diffvalue));

    if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId())) {

        // Since there's no perspective, "zooming" in the original sense
        // of the word won't have any visible effect. So we just increase
        // or decrease the field-of-view values of the camera instead, to
        // "shrink" the projection size of the model / scene.
        SoOrthographicCamera * oc = (SoOrthographicCamera *)cam;
        oc->height = oc->height.getValue() * multiplicator;

    }
    else {
        // FrustumCamera can be found in the SmallChange CVS module (it's
        // a camera that lets you specify (for instance) an off-center
        // frustum (similar to glFrustum())
        if (!t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()) &&
            tname != "FrustumCamera") {
 /*         static SbBool first = TRUE;
            if (first) {
                SoDebugError::postWarning("SoGuiFullViewerP::zoom",
                                          "Unknown camera type, "
                                          "will zoom by moving position, but this might not be correct.");
                first = FALSE;
            }*/
        }

        const float oldfocaldist = cam->focalDistance.getValue();
        const float newfocaldist = oldfocaldist * multiplicator;

        SbVec3f direction;
        cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);

        const SbVec3f oldpos = cam->position.getValue();
        const SbVec3f newpos = oldpos + (newfocaldist - oldfocaldist) * -direction;

        // This catches a rather common user interface "buglet": if the
        // user zooms the camera out to a distance from origo larger than
        // what we still can safely do floating point calculations on
        // (i.e. without getting NaN or Inf values), the faulty floating
        // point values will propagate until we start to get debug error
        // messages and eventually an assert failure from core Coin code.
        //
        // With the below bounds check, this problem is avoided.
        //
        // (But note that we depend on the input argument ''diffvalue'' to
        // be small enough that zooming happens gradually. Ideally, we
        // should also check distorigo with isinf() and isnan() (or
        // inversely; isinfite()), but those only became standardized with
        // C99.)
        const float distorigo = newpos.length();
        // sqrt(FLT_MAX) == ~ 1e+19, which should be both safe for further
        // calculations and ok for the end-user and app-programmer.
        if (distorigo > float(sqrt(FLT_MAX))) {
        }
        else {
            cam->position = newpos;
            cam->focalDistance = newfocaldist;
        }
    }
}

// Calculate a zoom/dolly factor from the difference of the current
// cursor position and the last.
void NavigationStyle::zoomByCursor(const SbVec2f & thispos, const SbVec2f & prevpos)
{
    // There is no "geometrically correct" value, 20 just seems to give
    // about the right "feel".
    zoom(viewer->getCamera(), (thispos[1] - prevpos[1]) * 10.0f/*20.0f*/);
}

/** Uses the sphere sheet projector to map the mouseposition onto
 * a 3D point and find a rotation from this and the last calculated point.
 */
void NavigationStyle::spin(const SbVec2f & pointerpos)
{
    if (this->log.historysize < 2) return;
    assert(this->spinprojector != NULL);

    const SbViewportRegion & vp = viewer->getViewportRegion();
    SbVec2s glsize(vp.getViewportSizePixels());
    SbVec2f lastpos;
    lastpos[0] = float(this->log.position[1][0]) / float(SoQtMax((int)(glsize[0]-1), 1));
    lastpos[1] = float(this->log.position[1][1]) / float(SoQtMax((int)(glsize[1]-1), 1));

    this->spinprojector->project(lastpos);
    SbRotation r;
    this->spinprojector->projectAndGetRotation(pointerpos, r);
    r.invert();
    this->reorientCamera(viewer->getCamera(), r);

    // Calculate an average angle magnitude value to make the transition
    // to a possible spin animation mode appear smooth.

    SbVec3f dummy_axis, newaxis;
    float acc_angle, newangle;
    this->spinincrement.getValue(dummy_axis, acc_angle);
    acc_angle *= this->spinsamplecounter; // weight
    r.getValue(newaxis, newangle);
    acc_angle += newangle;

    this->spinsamplecounter++;
    acc_angle /= this->spinsamplecounter;
    // FIXME: accumulate and average axis vectors aswell? 19990501 mortene.
    this->spinincrement.setValue(newaxis, acc_angle);

    // Don't carry too much baggage, as that'll give unwanted results
    // when the user quickly trigger (as in "click-drag-release") a spin
    // animation.
    if (this->spinsamplecounter > 3) this->spinsamplecounter = 3;
}

SbBool NavigationStyle::doSpin()
{
    if (this->log.historysize >= 3) {
        SbTime stoptime = (SbTime::getTimeOfDay() - this->log.time[0]);
        if (this->spinanimatingallowed && stoptime.getValue() < 0.100) {
            const SbViewportRegion & vp = viewer->getViewportRegion();
            const SbVec2s glsize(vp.getViewportSizePixels());
            SbVec3f from = this->spinprojector->project(SbVec2f(float(this->log.position[2][0]) / float(SoQtMax(glsize[0]-1, 1)),
                                                                float(this->log.position[2][1]) / float(SoQtMax(glsize[1]-1, 1))));
            SbVec3f to = this->spinprojector->project(this->lastmouseposition);
            SbRotation rot = this->spinprojector->getRotation(from, to);

            SbTime delta = (this->log.time[0] - this->log.time[2]);
            double deltatime = delta.getValue();
            rot.invert();
            rot.scaleAngle(float(0.200 / deltatime));

            SbVec3f axis;
            float radians;
            rot.getValue(axis, radians);
            if ((radians > 0.01f) && (deltatime < 0.300)) {
                this->spinRotation = rot;
                return TRUE;
            }
        }
    }

    return FALSE;
}

void NavigationStyle::updateAnimation()
{
    SbTime now = SbTime::getTimeOfDay();
    double secs = now.getValue() -  prevRedrawTime.getValue();
    this->prevRedrawTime = now;

    if (this->isAnimating()) {
        // here the camera rotates around a fix axis
        SbRotation deltaRotation = this->spinRotation;
        deltaRotation.scaleAngle(secs * 5.0);
        this->reorientCamera(viewer->getCamera(), deltaRotation);
    }
}

void NavigationStyle::redraw()
{
    if (pcMouseModel)
        pcMouseModel->redraw();
}

SbBool NavigationStyle::handleEventInForeground(const SoEvent* const e)
{
    SoHandleEventAction action(viewer->getViewportRegion());
    action.setEvent(e);
    action.apply(viewer->foregroundroot);
    return action.isHandled();
}

/*!
  Decide if it should be possible to start a spin animation of the
  model in the viewer by releasing the mouse button while dragging.

  If the \a enable flag is \c FALSE and we're currently animating, the
  spin will be stopped.
*/
void
NavigationStyle::setAnimationEnabled(const SbBool enable)
{
    this->spinanimatingallowed = enable;
    if (!enable && this->isAnimating()) { this->stopAnimating(); }
}

/*!
  Query whether or not it is possible to start a spinning animation by
  releasing the left mouse button while dragging the mouse.
*/

SbBool
NavigationStyle::isAnimationEnabled(void) const
{
    return this->spinanimatingallowed;
}

/*!
  Query if the model in the viewer is currently in spinning mode after
  a user drag.
*/
SbBool NavigationStyle::isAnimating(void) const
{
    return this->currentmode == NavigationStyle::SPINNING;
}

/*!
 * Starts programmatically the viewer in animation mode. The given axis direction
 * is always in screen coordinates, not in world coordinates.
 */
void NavigationStyle::startAnimating(const SbVec3f& axis, float velocity)
{
    if (!isAnimationEnabled()) return;

    this->spinincrement = SbRotation::identity();
    SbRotation rot;
    rot.setValue(axis, velocity);

    this->setViewing(true);
    this->setViewingMode(NavigationStyle::SPINNING);
    this->spinRotation = rot;
}

void NavigationStyle::stopAnimating(void)
{
    if (this->currentmode != NavigationStyle::SPINNING) {
        return;
    }
    this->setViewingMode(this->isViewing() ? 
        NavigationStyle::IDLE : NavigationStyle::INTERACT);
}

void NavigationStyle::startPicking(NavigationStyle::ePickMode mode)
{
    if (pcMouseModel)
        return;
  
    switch (mode)
    {
    case Lasso:
        pcMouseModel = new PolyPickerMouseModel();
        break;
    case Rectangle:
        pcMouseModel = new SelectionMouseModel();
        break;
    case BoxZoom:
        pcMouseModel = new BoxZoomMouseModel();
        break;
    case Clip:
        pcMouseModel = new PolyClipMouseModel();
        break;
    default:
        break;
    }

    if (pcMouseModel)
        pcMouseModel->grabMouseModel(viewer);
}

void NavigationStyle::stopPicking()
{
    pcPolygon.clear();
    delete pcMouseModel; 
    pcMouseModel = 0;
}

SbBool NavigationStyle::isPicking() const
{
    return (pcMouseModel ? TRUE : FALSE);
}

const std::vector<SbVec2f>& NavigationStyle::getPickedPolygon(SbBool* clip_inner) const
{
    if (clip_inner)
        *clip_inner = this->clipInner;
    return pcPolygon;
}

// This method adds another point to the mouse location log, used for spin
// animation calculations.
void NavigationStyle::addToLog(const SbVec2s pos, const SbTime time)
{
    // In case someone changes the const size setting at the top of this
    // file too small.
    assert (this->log.size > 2 && "mouse log too small!");

    if (this->log.historysize > 0 && pos == this->log.position[0]) {
#if SOQt_DEBUG && 0 // debug
        // This can at least happen under SoQt.
        SoDebugError::postInfo("NavigationStyle::addToLog", "got position already!");
#endif // debug
        return;
    }

    int lastidx = this->log.historysize;
    // If we've filled up the log, we should throw away the last item:
    if (lastidx == this->log.size) { lastidx--; }

    assert(lastidx < this->log.size);
    for (int i = lastidx; i > 0; i--) {
        this->log.position[i] = this->log.position[i-1];
        this->log.time[i] = this->log.time[i-1];
    }

    this->log.position[0] = pos;
    this->log.time[0] = time;
    if (this->log.historysize < this->log.size)
        this->log.historysize += 1;
}

// This method "clears" the mouse location log, used for spin
// animation calculations.
void NavigationStyle::clearLog(void)
{
    this->log.historysize = 0;
}

// The viewer is a state machine, and all changes to the current state
// are made through this call.
void NavigationStyle::setViewingMode(const ViewerMode newmode)
{
    const ViewerMode oldmode = this->currentmode;
    if (newmode == oldmode) { return; }

    switch (newmode) {
    case DRAGGING:
        // Set up initial projection point for the projector object when
        // first starting a drag operation.
        this->spinprojector->project(this->lastmouseposition);
        this->interactiveCountInc();
        this->clearLog();
        break;

    case SPINNING:
        this->interactiveCountInc();
        viewer->scheduleRedraw();
        break;

    case PANNING:
        pan(viewer->getCamera());
        this->interactiveCountInc();
        break;

    case ZOOMING:
        this->interactiveCountInc();
        break;

    case BOXZOOM:
        this->interactiveCountInc();
        break;

    default: // include default to avoid compiler warnings.
        break;
    }

    switch (oldmode) {
    case SPINNING:
    case DRAGGING:
    case PANNING:
    case ZOOMING:
    case BOXZOOM:
        this->interactiveCountDec();
        break;

    default:
        break;
    }

    viewer->setCursorRepresentation(newmode);
    this->currentmode = newmode;
}

int NavigationStyle::getViewingMode() const
{
    return (int)this->currentmode;
}

SbBool NavigationStyle::processSoEvent(const SoEvent * const ev)
{
    return viewer->processSoEventBase(ev);
}

void NavigationStyle::setPopupMenuEnabled(const SbBool on)
{
    this->menuenabled = on;
}

SbBool NavigationStyle::isPopupMenuEnabled(void) const
{
    return this->menuenabled;
}

void NavigationStyle::openPopupMenu(const SbVec2s& position)
{
    // ask workbenches and view provider, ...
    MenuItem* view = new MenuItem;
    Gui::Application::Instance->setupContextMenu("View", view);

    QMenu contextMenu(viewer->getGLWidget());
    QMenu subMenu;
    QActionGroup subMenuGroup(&subMenu);
    subMenuGroup.setExclusive(true);
    subMenu.setTitle(QObject::tr("Navigation styles"));

    MenuManager::getInstance()->setupContextMenu(view,contextMenu);
    contextMenu.addMenu(&subMenu);

    // add submenu at the end to select navigation style
    QRegExp rx(QString::fromAscii("^\\w+::(\\w+)Navigation\\w+$"));
    std::vector<Base::Type> types;
    Base::Type::getAllDerivedFrom(NavigationStyle::getClassTypeId(), types);
    for (std::vector<Base::Type>::iterator it = types.begin(); it != types.end(); ++it) {
        if (*it != NavigationStyle::getClassTypeId()) {
            QString data = QString::fromAscii(it->getName());
            QString name = data.mid(data.indexOf(QLatin1String("::"))+2);
            if (rx.indexIn(data) > -1) {
                name = QObject::tr("%1 navigation").arg(rx.cap(1));
                QAction* item = subMenuGroup.addAction(name);
                item->setData(QByteArray(it->getName()));
                item->setCheckable(true);
                if (*it == this->getTypeId())
                    item->setChecked(true);
                subMenu.addAction(item);
            }
        }
    }

    delete view;
    QAction* used = contextMenu.exec(QCursor::pos());
    if (used && subMenuGroup.actions().indexOf(used) >= 0 && used->isChecked()) {
        QByteArray type = used->data().toByteArray();
        QWidget* widget = viewer->getWidget();
        if (widget) {
            widget = widget->parentWidget();
            if (widget) {
                // this is the widget where the viewer is embedded
                Base::Type style = Base::Type::fromName((const char*)type);
                if (style != this->getTypeId()) {
                    QEvent* event = new NavigationStyleEvent(style);
                    QApplication::postEvent(widget, event);
                }
            }
        }
    }
}

// ----------------------------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::InventorNavigationStyle, Gui::NavigationStyle);

InventorNavigationStyle::InventorNavigationStyle()
{
}

InventorNavigationStyle::~InventorNavigationStyle()
{
}

SbBool InventorNavigationStyle::processSoEvent(const SoEvent * const ev)
{
    // If we're in picking mode then all events must be redirected to the
    // appropriate mouse model.
    if (pcMouseModel) {
        int hd=pcMouseModel->handleEvent(ev,viewer->getViewportRegion());
        if (hd==AbstractMouseModel::Continue||hd==AbstractMouseModel::Restart) {
            return TRUE;
        }
        else if (hd==AbstractMouseModel::Finish) {
            pcPolygon = pcMouseModel->getPolygon();
            clipInner = pcMouseModel->isInner();
            delete pcMouseModel; pcMouseModel = 0;
            return inherited::processSoEvent(ev);
        }
        else if (hd==AbstractMouseModel::Cancel) {
            pcPolygon.clear();
            delete pcMouseModel; pcMouseModel = 0;
            return inherited::processSoEvent(ev);
        }
    }

    // Events when in "ready-to-seek" mode are ignored, except those
    // which influence the seek mode itself -- these are handled further
    // up the inheritance hierarchy.
    if (this->isSeekMode()) { return inherited::processSoEvent(ev); }

    const SoType type(ev->getTypeId());

    const SbViewportRegion & vp = viewer->getViewportRegion();
    const SbVec2s size(vp.getViewportSizePixels());
    const SbVec2f prevnormalized = this->lastmouseposition;
    const SbVec2s pos(ev->getPosition());
    const SbVec2f posn((float) pos[0] / (float) SoQtMax((int)(size[0] - 1), 1),
                       (float) pos[1] / (float) SoQtMax((int)(size[1] - 1), 1));

    this->lastmouseposition = posn;

    // Set to TRUE if any event processing happened. Note that it is not
    // necessary to restrict ourselves to only do one "action" for an
    // event, we only need this flag to see if any processing happened
    // at all.
    SbBool processed = FALSE;

    const ViewerMode currentmode = this->currentmode;
    ViewerMode newmode = currentmode;

    // Mismatches in state of the modifier keys happens if the user
    // presses or releases them outside the viewer window.
    if (this->ctrldown != ev->wasCtrlDown()) {
        this->ctrldown = ev->wasCtrlDown();
    }
    if (this->shiftdown != ev->wasShiftDown()) {
        this->shiftdown = ev->wasShiftDown();
    }
    if (this->altdown != ev->wasAltDown()) {
        this->altdown = ev->wasAltDown();
    }

    // give the nodes in the foreground root the chance to handle events (e.g color bar)
    if (!processed && !viewer->isEditing()) {
        processed = handleEventInForeground(ev);
        if (processed)
            return TRUE;
    }

    // Keyboard handling
    if (type.isDerivedFrom(SoKeyboardEvent::getClassTypeId())) {
        const SoKeyboardEvent * const event = (const SoKeyboardEvent *) ev;
        const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;
        switch (event->getKey()) {
        case SoKeyboardEvent::LEFT_CONTROL:
        case SoKeyboardEvent::RIGHT_CONTROL:
            this->ctrldown = press;
            break;
        case SoKeyboardEvent::LEFT_SHIFT:
        case SoKeyboardEvent::RIGHT_SHIFT:
            this->shiftdown = press;
            break;
        case SoKeyboardEvent::LEFT_ALT:
        case SoKeyboardEvent::RIGHT_ALT:
            this->altdown = press;
            break;
        case SoKeyboardEvent::H:
            processed = TRUE;
            viewer->saveHomePosition();
            break;
        case SoKeyboardEvent::S:
        case SoKeyboardEvent::HOME:
        case SoKeyboardEvent::LEFT_ARROW:
        case SoKeyboardEvent::UP_ARROW:
        case SoKeyboardEvent::RIGHT_ARROW:
        case SoKeyboardEvent::DOWN_ARROW:
            if (!this->isViewing())
                this->setViewing(true);
            break;
        default:
            break;
        }
    }

    // Mouse Button / Spaceball Button handling
    if (type.isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {
        const SoMouseButtonEvent * const event = (const SoMouseButtonEvent *) ev;
        const int button = event->getButton();
        const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

        // SoDebugError::postInfo("processSoEvent", "button = %d", button);
        switch (button) {
        case SoMouseButtonEvent::BUTTON1:
            this->button1down = press;
            if (!press && ev->wasAltDown()) {
                float ratio = vp.getViewportAspectRatio();
                SbViewVolume vv = viewer->getCamera()->getViewVolume(ratio);
                this->panningplane = vv.getPlane(viewer->getCamera()->focalDistance.getValue());
                if (!this->seekToPoint(pos)) {
                    panToCenter(panningplane, posn);
                    this->interactiveCountDec();
                }
                processed = TRUE;
            }
            else if (press && (this->currentmode == NavigationStyle::SEEK_WAIT_MODE)) {
                newmode = NavigationStyle::SEEK_MODE;
                this->seekToPoint(pos); // implicitly calls interactiveCountInc()
                processed = TRUE;
            }
            else if (press && (this->currentmode == NavigationStyle::IDLE)) {
                this->setViewing(true);
                processed = TRUE;
            }
            else if (!press && (this->currentmode == NavigationStyle::DRAGGING)) {
                this->setViewing(false);
                processed = TRUE;
            }
            else if (viewer->isEditing() && (this->currentmode == NavigationStyle::SPINNING)) {
                processed = TRUE;
            }
            break;
        case SoMouseButtonEvent::BUTTON2:
            // If we are in edit mode then simply ignore the RMB events
            // to pass the event to the base class.
            if (!viewer->isEditing()) {
                // If we are in zoom or pan mode ignore RMB events otherwise
                // the canvas doesn't get any release events 
                if (this->currentmode != NavigationStyle::ZOOMING && 
                    this->currentmode != NavigationStyle::PANNING) {
                    if (this->isPopupMenuEnabled()) {
                        if (!press) { // release right mouse button
                            this->openPopupMenu(event->getPosition());
                        }
                    }
                }
            }
            break;
        case SoMouseButtonEvent::BUTTON3:
            if (press) {
                this->centerTime = ev->getTime();
                float ratio = vp.getViewportAspectRatio();
                SbViewVolume vv = viewer->getCamera()->getViewVolume(ratio);
                this->panningplane = vv.getPlane(viewer->getCamera()->focalDistance.getValue());
            }
            else {
                SbTime tmp = (ev->getTime() - this->centerTime);
                float dci = (float)QApplication::doubleClickInterval()/1000.0f;
                // is it just a middle click?
                if (tmp.getValue() < dci) {
                    if (!this->seekToPoint(pos)) {
                        panToCenter(panningplane, posn);
                        this->interactiveCountDec();
                    }
                    processed = TRUE;
                }
            }
            this->button3down = press;
            break;
        case SoMouseButtonEvent::BUTTON4:
            zoom(viewer->getCamera(), 0.05f);
            processed = TRUE;
            break;
        case SoMouseButtonEvent::BUTTON5:
            zoom(viewer->getCamera(), -0.05f);
            processed = TRUE;
            break;
        default:
            break;
        }
    }

    // Mouse Movement handling
    if (type.isDerivedFrom(SoLocation2Event::getClassTypeId())) {
        const SoLocation2Event * const event = (const SoLocation2Event *) ev;
        if (this->currentmode == NavigationStyle::ZOOMING) {
            this->zoomByCursor(posn, prevnormalized);
            processed = TRUE;
        }
        else if (this->currentmode == NavigationStyle::PANNING) {
            float ratio = vp.getViewportAspectRatio();
            panCamera(viewer->getCamera(), ratio, this->panningplane, posn, prevnormalized);
            processed = TRUE;
        }
        else if (this->currentmode == NavigationStyle::DRAGGING) {
            this->addToLog(event->getPosition(), event->getTime());
            this->spin(posn);
            processed = TRUE;
        }
    }

    // Spaceball & Joystick handling
    if (type.isDerivedFrom(SoMotion3Event::getClassTypeId())) {
        SoMotion3Event * const event = (SoMotion3Event *) ev;
        SoCamera * const camera = viewer->getCamera();
        if (camera) {
            SbVec3f dir = event->getTranslation();
            camera->orientation.getValue().multVec(dir,dir);
            camera->position = camera->position.getValue() + dir;
            camera->orientation = 
                event->getRotation() * camera->orientation.getValue();
            processed = TRUE;
        }
    }

    enum {
        BUTTON1DOWN = 1 << 0,
        BUTTON3DOWN = 1 << 1,
        CTRLDOWN =    1 << 2,
        SHIFTDOWN =   1 << 3
    };
    unsigned int combo =
        (this->button1down ? BUTTON1DOWN : 0) |
        (this->button3down ? BUTTON3DOWN : 0) |
        (this->ctrldown ? CTRLDOWN : 0) |
        (this->shiftdown ? SHIFTDOWN : 0);

    switch (combo) {
    case 0:
        if (currentmode == NavigationStyle::SPINNING) { break; }
        newmode = NavigationStyle::IDLE;

        if (currentmode == NavigationStyle::DRAGGING) {
            if (doSpin())
                newmode = NavigationStyle::SPINNING;
        }
        break;
    case BUTTON1DOWN:
        newmode = NavigationStyle::DRAGGING;
        break;
    case BUTTON3DOWN:
    case SHIFTDOWN|BUTTON1DOWN:
        newmode = NavigationStyle::PANNING;
        break;
    case CTRLDOWN:
    case CTRLDOWN|BUTTON1DOWN:
    case CTRLDOWN|SHIFTDOWN:
    case CTRLDOWN|SHIFTDOWN|BUTTON1DOWN:
        newmode = NavigationStyle::SELECTION;
        break;
    case BUTTON1DOWN|BUTTON3DOWN:
    case CTRLDOWN|BUTTON3DOWN:
        newmode = NavigationStyle::ZOOMING;
        break;

        // There are many cases we don't handle that just falls through to
        // the default case, like SHIFTDOWN, CTRLDOWN, CTRLDOWN|SHIFTDOWN,
        // SHIFTDOWN|BUTTON3DOWN, SHIFTDOWN|CTRLDOWN|BUTTON3DOWN, etc.
        // This is a feature, not a bug. :-)
        //
        // mortene.

    default:
        // The default will make a spin stop and otherwise not do
        // anything.
        if ((currentmode != NavigationStyle::SEEK_WAIT_MODE) &&
            (currentmode != NavigationStyle::SEEK_MODE)) {
            newmode = NavigationStyle::IDLE;
        }
        break;
    }

    if (newmode != currentmode) {
        this->setViewingMode(newmode);
    }

    // If not handled in this class, pass on upwards in the inheritance
    // hierarchy.
    if ((this->currentmode == NavigationStyle::SELECTION || viewer->isEditing()) && !processed)
        processed = inherited::processSoEvent(ev);
    else
        return TRUE;

    // check for left click without selecting something
    if (this->currentmode == NavigationStyle::SELECTION && !processed) {
        if (ev->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {
            SoMouseButtonEvent * const e = (SoMouseButtonEvent *) ev;
            if (SoMouseButtonEvent::isButtonReleaseEvent(e,SoMouseButtonEvent::BUTTON1)) {
                Gui::Selection().clearSelection();
            }
        }
    }

    return FALSE;
}

// ----------------------------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::CADNavigationStyle, Gui::NavigationStyle);

CADNavigationStyle::CADNavigationStyle() : _bRejectSelection(false), _bSpining(false)
{
}

CADNavigationStyle::~CADNavigationStyle()
{
}
#if 1
SbBool CADNavigationStyle::processSoEvent(const SoEvent * const ev)
{
    // If we're in picking mode then all events must be redirected to the
    // appropriate mouse model.
    if (pcMouseModel) {
        int hd=pcMouseModel->handleEvent(ev,viewer->getViewportRegion());
        if (hd==AbstractMouseModel::Continue||hd==AbstractMouseModel::Restart) {
            return TRUE;
        }
        else if (hd==AbstractMouseModel::Finish) {
            pcPolygon = pcMouseModel->getPolygon();
            clipInner = pcMouseModel->isInner();
            delete pcMouseModel; pcMouseModel = 0;
            return inherited::processSoEvent(ev);
        }
        else if (hd==AbstractMouseModel::Cancel) {
            pcPolygon.clear();
            delete pcMouseModel; pcMouseModel = 0;
            return inherited::processSoEvent(ev);
        }
    }
#if 0
    // Events when in "ready-to-seek" mode are ignored, except those
    // which influence the seek mode itself -- these are handled further
    // up the inheritance hierarchy.
    if (this->isSeekMode()) { return inherited::processSoEvent(ev); }
#else
    if (!this->isSeekMode() && this->isViewing())
        this->setViewing(false); // by default disable viewing mode to render the scene
#endif

    const SoType type(ev->getTypeId());

    const SbViewportRegion & vp = viewer->getViewportRegion();
    const SbVec2s size(vp.getViewportSizePixels());
    const SbVec2f prevnormalized = this->lastmouseposition;
    const SbVec2s pos(ev->getPosition());
    const SbVec2f posn((float) pos[0] / (float) SoQtMax((int)(size[0] - 1), 1),
                       (float) pos[1] / (float) SoQtMax((int)(size[1] - 1), 1));

    this->lastmouseposition = posn;

    // Set to TRUE if any event processing happened. Note that it is not
    // necessary to restrict ourselves to only do one "action" for an
    // event, we only need this flag to see if any processing happened
    // at all.
    SbBool processed = FALSE;

    const ViewerMode currentmode = this->currentmode;
    ViewerMode newmode = currentmode;
    ViewerMode oldmode = currentmode;

    // Mismatches in state of the modifier keys happens if the user
    // presses or releases them outside the viewer window.
    if (this->ctrldown != ev->wasCtrlDown()) {
        this->ctrldown = ev->wasCtrlDown();
    }
    if (this->shiftdown != ev->wasShiftDown()) {
        this->shiftdown = ev->wasShiftDown();
    }
    if (this->altdown != ev->wasAltDown()) {
        this->altdown = ev->wasAltDown();
    }

    // give the nodes in the foreground root the chance to handle events (e.g color bar)
    if (!processed && !viewer->isEditing()) {
        processed = handleEventInForeground(ev);
        if (processed)
            return TRUE;
    }

    // Keyboard handling
    if (type.isDerivedFrom(SoKeyboardEvent::getClassTypeId())) {
        const SoKeyboardEvent * const event = (const SoKeyboardEvent *) ev;
        const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;
        switch (event->getKey()) {
        case SoKeyboardEvent::LEFT_CONTROL:
        case SoKeyboardEvent::RIGHT_CONTROL:
            this->ctrldown = press;
            break;
        case SoKeyboardEvent::LEFT_SHIFT:
        case SoKeyboardEvent::RIGHT_SHIFT:
            this->shiftdown = press;
            break;
        case SoKeyboardEvent::LEFT_ALT:
        case SoKeyboardEvent::RIGHT_ALT:
            this->altdown = press;
            break;
        case SoKeyboardEvent::H:
            processed = TRUE;
            viewer->saveHomePosition();
            break;
        case SoKeyboardEvent::S:
        case SoKeyboardEvent::HOME:
        case SoKeyboardEvent::LEFT_ARROW:
        case SoKeyboardEvent::UP_ARROW:
        case SoKeyboardEvent::RIGHT_ARROW:
        case SoKeyboardEvent::DOWN_ARROW:
            if (!this->isViewing())
                this->setViewing(true);
            break;
        default:
            break;
        }
    }

    // Mouse Button / Spaceball Button handling
    if (type.isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {
        const SoMouseButtonEvent * const event = (const SoMouseButtonEvent *) ev;
        const int button = event->getButton();
        const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

        // SoDebugError::postInfo("processSoEvent", "button = %d", button);
        switch (button) {
        case SoMouseButtonEvent::BUTTON1:
            this->button1down = press;
            if (!press && ev->wasAltDown()) {
                float ratio = vp.getViewportAspectRatio();
                SbViewVolume vv = viewer->getCamera()->getViewVolume(ratio);
                this->panningplane = vv.getPlane(viewer->getCamera()->focalDistance.getValue());
                if (!this->seekToPoint(pos)) {
                    panToCenter(panningplane, posn);
                    this->interactiveCountDec();
                }
                processed = TRUE;
            }
            else if (press && (this->currentmode == NavigationStyle::SEEK_WAIT_MODE)) {
                newmode = NavigationStyle::SEEK_MODE;
                this->seekToPoint(pos); // implicitly calls interactiveCountInc()
                processed = TRUE;
            }
            //else if (press && (this->currentmode == NavigationStyle::IDLE)) {
            //    this->setViewing(true);
            //    processed = TRUE;
            //}
            else if (press && (this->currentmode == NavigationStyle::PANNING ||
                               this->currentmode == NavigationStyle::ZOOMING)) {
                newmode = NavigationStyle::DRAGGING;
                this->centerTime = ev->getTime();
                processed = TRUE;
            }
            else if (!press && (this->currentmode == NavigationStyle::DRAGGING)) {
                SbTime tmp = (ev->getTime() - this->centerTime);
                float dci = (float)QApplication::doubleClickInterval()/1000.0f;
                if (tmp.getValue() < dci) {
                    newmode = NavigationStyle::ZOOMING;
                }
                processed = TRUE;
            }
            else if (!press && (this->currentmode == NavigationStyle::DRAGGING)) {
                this->setViewing(false);
                processed = TRUE;
            }
            else if (viewer->isEditing() && (this->currentmode == NavigationStyle::SPINNING)) {
                processed = TRUE;
            }
            break;
        case SoMouseButtonEvent::BUTTON2:
            // If we are in edit mode then simply ignore the RMB events
            // to pass the event to the base class.
            if (!viewer->isEditing()) {
                // If we are in zoom or pan mode ignore RMB events otherwise
                // the canvas doesn't get any release events 
                if (this->currentmode != NavigationStyle::ZOOMING && 
                    this->currentmode != NavigationStyle::PANNING) {
                    if (this->isPopupMenuEnabled()) {
                        if (!press) { // release right mouse button
                            this->openPopupMenu(event->getPosition());
                        }
                    }
                }
            }
            break;
        case SoMouseButtonEvent::BUTTON3:
            if (press) {
                this->centerTime = ev->getTime();
                float ratio = vp.getViewportAspectRatio();
                SbViewVolume vv = viewer->getCamera()->getViewVolume(ratio);
                this->panningplane = vv.getPlane(viewer->getCamera()->focalDistance.getValue());
            }
            else {
                SbTime tmp = (ev->getTime() - this->centerTime);
                float dci = (float)QApplication::doubleClickInterval()/1000.0f;
                // is it just a middle click?
                if (tmp.getValue() < dci) {
                    if (!this->seekToPoint(pos)) {
                        panToCenter(panningplane, posn);
                        this->interactiveCountDec();
                    }
                    processed = TRUE;
                }
            }
            this->button3down = press;
            break;
        case SoMouseButtonEvent::BUTTON4:
            zoom(viewer->getCamera(), 0.05f);
            processed = TRUE;
            break;
        case SoMouseButtonEvent::BUTTON5:
            zoom(viewer->getCamera(), -0.05f);
            processed = TRUE;
            break;
        default:
            break;
        }
    }

    // Mouse Movement handling
    if (type.isDerivedFrom(SoLocation2Event::getClassTypeId())) {
        const SoLocation2Event * const event = (const SoLocation2Event *) ev;
        if (this->currentmode == NavigationStyle::ZOOMING) {
            this->zoomByCursor(posn, prevnormalized);
            processed = TRUE;
        }
        else if (this->currentmode == NavigationStyle::PANNING) {
            float ratio = vp.getViewportAspectRatio();
            panCamera(viewer->getCamera(), ratio, this->panningplane, posn, prevnormalized);
            processed = TRUE;
        }
        else if (this->currentmode == NavigationStyle::DRAGGING) {
            this->addToLog(event->getPosition(), event->getTime());
            this->spin(posn);
            processed = TRUE;
        }
    }

    // Spaceball & Joystick handling
    if (type.isDerivedFrom(SoMotion3Event::getClassTypeId())) {
        SoMotion3Event * const event = (SoMotion3Event *) ev;
        SoCamera * const camera = viewer->getCamera();
        if (camera) {
            SbVec3f dir = event->getTranslation();
            camera->orientation.getValue().multVec(dir,dir);
            camera->position = camera->position.getValue() + dir;
            camera->orientation = 
                event->getRotation() * camera->orientation.getValue();
            processed = TRUE;
        }
    }

    enum {
        BUTTON1DOWN = 1 << 0,
        BUTTON3DOWN = 1 << 1,
        CTRLDOWN =    1 << 2,
        SHIFTDOWN =   1 << 3
    };
    unsigned int combo =
        (this->button1down ? BUTTON1DOWN : 0) |
        (this->button3down ? BUTTON3DOWN : 0) |
        (this->ctrldown ? CTRLDOWN : 0) |
        (this->shiftdown ? SHIFTDOWN : 0);

    switch (combo) {
    case 0:
        if (currentmode == NavigationStyle::SPINNING) { break; }
        newmode = NavigationStyle::IDLE;

        //if (currentmode == NavigationStyle::DRAGGING) {
        //    if (doSpin())
        //        newmode = NavigationStyle::SPINNING;
        //}
        break;
    case BUTTON1DOWN:
        // make sure not to change the selection when stopping spinning
        if (currentmode == NavigationStyle::SPINNING)
            newmode = NavigationStyle::IDLE;
        else
            newmode = NavigationStyle::SELECTION;
        break;
    case BUTTON3DOWN:
        if (currentmode == NavigationStyle::SPINNING) { break; }
        else if (newmode == NavigationStyle::ZOOMING) { break; }
        newmode = NavigationStyle::PANNING;

        if (currentmode == NavigationStyle::DRAGGING) {
            if (doSpin()) {
                newmode = NavigationStyle::SPINNING;
                break;
            }
        }
        break;
    //case CTRLDOWN:
    //case CTRLDOWN|BUTTON1DOWN:
    //case CTRLDOWN|SHIFTDOWN:
    //case CTRLDOWN|SHIFTDOWN|BUTTON1DOWN:
    //    newmode = NavigationStyle::SELECTION;
    //    break;
    //case BUTTON1DOWN|BUTTON3DOWN:
    //case CTRLDOWN|BUTTON3DOWN:
    //    newmode = NavigationStyle::ZOOMING;
    //    break;

        // There are many cases we don't handle that just falls through to
        // the default case, like SHIFTDOWN, CTRLDOWN, CTRLDOWN|SHIFTDOWN,
        // SHIFTDOWN|BUTTON3DOWN, SHIFTDOWN|CTRLDOWN|BUTTON3DOWN, etc.
        // This is a feature, not a bug. :-)
        //
        // mortene.

    default:
        // The default will make a spin stop and otherwise not do
        // anything.
        //if ((currentmode != NavigationStyle::SEEK_WAIT_MODE) &&
        //    (currentmode != NavigationStyle::SEEK_MODE)) {
        //    newmode = NavigationStyle::IDLE;
        //}
        break;
    }

    if (newmode != currentmode) {
        this->setViewingMode(newmode);
    }

    // If not handled in this class, pass on upwards in the inheritance
    // hierarchy.
    if (/*(oldmode == NavigationStyle::SELECTION || viewer->isEditing()) && */!processed)
        processed = inherited::processSoEvent(ev);
    else
        return TRUE;

    // check for left click without selecting something
    if (oldmode == NavigationStyle::SELECTION && !processed) {
        if (ev->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {
            SoMouseButtonEvent * const e = (SoMouseButtonEvent *) ev;
            if (SoMouseButtonEvent::isButtonReleaseEvent(e,SoMouseButtonEvent::BUTTON1)) {
                Gui::Selection().clearSelection();
            }
        }
    }

    return FALSE;
}
#else
SbBool CADNavigationStyle::processSoEvent(const SoEvent * const ev)
{
    SbBool processed = FALSE;
    if (!this->isSeekMode() && this->isViewing())
        this->setViewing(false); // by default disable viewing mode to render the scene

    // Keybooard handling
    if (ev->getTypeId().isDerivedFrom(SoKeyboardEvent::getClassTypeId())) {
        SoKeyboardEvent * ke = (SoKeyboardEvent *)ev;
        switch (ke->getKey()) {
        case SoKeyboardEvent::LEFT_ALT:
        case SoKeyboardEvent::RIGHT_ALT:
        case SoKeyboardEvent::LEFT_CONTROL:
        case SoKeyboardEvent::RIGHT_CONTROL:
        case SoKeyboardEvent::LEFT_SHIFT:
        case SoKeyboardEvent::RIGHT_SHIFT:
            break;
        case SoKeyboardEvent::H:
            viewer->saveHomePosition();
            processed = TRUE;
            break;
        case SoKeyboardEvent::S:
            // processSoEvent() of the base class sets the seekMode() if needed
        case SoKeyboardEvent::HOME:
        case SoKeyboardEvent::LEFT_ARROW:
        case SoKeyboardEvent::UP_ARROW:
        case SoKeyboardEvent::RIGHT_ARROW:
        case SoKeyboardEvent::DOWN_ARROW:
            if (!this->isViewing())
                this->setViewing(true);
            break;
        default:
            break;
        }
    }

    static bool MoveMode=false;
    static bool MoveModeMoved=false;
    static bool ZoomMode=false;
    static bool RotMode =false;
    static bool dCliBut3=false;

    const SbViewportRegion & vp = viewer->getViewportRegion();
    const SbVec2s size(vp.getViewportSizePixels());
    const SbVec2f prevnormalized = lastmouseposition;
    const SbVec2s pos(ev->getPosition());
    const SbVec2f posn((float) pos[0] / (float) SoQtMax((int)(size[0] - 1), 1),
                       (float) pos[1] / (float) SoQtMax((int)(size[1] - 1), 1));

    lastmouseposition = posn;

    // switching the mouse modes
    if (ev->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {

        const SoMouseButtonEvent * const event = (const SoMouseButtonEvent *) ev;
        const int button = event->getButton();
        const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

        // SoDebugError::postInfo("processSoEvent", "button = %d", button);
        switch (button) {
        case SoMouseButtonEvent::BUTTON1:
            if (press) {
                _bRejectSelection = false;
                if (MoveMode) {
                    RotMode = true;
                    ZoomMode = false;
                    MoveTime = ev->getTime();

                    // Set up initial projection point for the projector object when
                    // first starting a drag operation.
                    spinprojector->project(lastmouseposition);
                    //interactiveCountInc();
                    clearLog();

                    viewer->setComponentCursor(SoQtCursor::getRotateCursor());
                    processed = TRUE;
                }
            }
            else {
                // if you come out of rotation dont deselect anything
                if (_bRejectSelection || MoveMode) {
                    _bRejectSelection=false;
                    processed = TRUE;
                }
                if (MoveMode) {
                    RotMode = false; 

                    SbTime tmp = (ev->getTime() - MoveTime);
                    float dci = (float)QApplication::doubleClickInterval()/1000.0f;
                    if (tmp.getValue() < dci/*0.300*/) {
                        ZoomMode = true;
                        viewer->setComponentCursor(SoQtCursor::getZoomCursor());
                    }
                    else {
                        ZoomMode = false;
                        viewer->setComponentCursor(SoQtCursor::getPanCursor());

                        float ratio = vp.getViewportAspectRatio();
                        SbViewVolume vv = viewer->getCamera()->getViewVolume(ratio);
                        panningplane = vv.getPlane(viewer->getCamera()->focalDistance.getValue());
       
                        // check on start spining
                        SbTime stoptime = (ev->getTime() - log.time[0]);
                        if (this->spinanimatingallowed && stoptime.getValue() < 0.100) {
                            const SbVec2s glsize(vp.getViewportSizePixels());
                            SbVec3f from = spinprojector->project(SbVec2f(float(log.position[2][0]) / float(SoQtMax(glsize[0]-1, 1)),
                                                                          float(log.position[2][1]) / float(SoQtMax(glsize[1]-1, 1))));
                            SbVec3f to = spinprojector->project(posn);
                            SbRotation rot = spinprojector->getRotation(from, to);

                            SbTime delta = (log.time[0] - log.time[2]);
                            double deltatime = delta.getValue();
                            rot.invert();
                            rot.scaleAngle(float(0.200 / deltatime));

                            SbVec3f axis;
                            float radians;
                            rot.getValue(axis, radians);
                            float dci = (float)QApplication::doubleClickInterval()/1000.0f;
                            if ((radians > 0.01f) && (deltatime < dci/*0.300*/)) {
                                _bSpining = true;
                                spinRotation = rot;
                                MoveMode = false;
                                // restore the previous cursor
                                viewer->getWidget()->setCursor(_oldCursor /*QCursor( Qt::ArrowCursor )*/);
                            }
                        }
                    }
                    processed = TRUE;
                }
            }
            break;
        case SoMouseButtonEvent::BUTTON2:
            break;
        case SoMouseButtonEvent::BUTTON3:
            //if (isEditing()) // in edit mode do not do interactions 
            //    break;
            if (press) {
                centerTime = ev->getTime();
                MoveMode = true;
                MoveModeMoved=false;
                _bSpining = false;
                dCliBut3 = false;
                float ratio = vp.getViewportAspectRatio();
                SbViewVolume vv = viewer->getCamera()->getViewVolume(ratio);
                panningplane = vv.getPlane(viewer->getCamera()->focalDistance.getValue());
                // save the current cursor before overriding
                _oldCursor = viewer->getWidget()->cursor();
                viewer->setComponentCursor(SoQtCursor::getPanCursor());
            }
            else {
                SbTime tmp = (ev->getTime() - centerTime);
                float dci = (float)QApplication::doubleClickInterval()/1000.0f;
                // is it just a middle click?
                if (tmp.getValue() < dci/*0.300*/ && !MoveModeMoved) {
                    if (!this->seekToPoint(pos)) {
                        panToCenter(panningplane, posn);
                        this->interactiveCountDec();
                    }
                }

                MoveMode = false;
                RotMode = false;
                ZoomMode = false;
                // restore the previous cursor
                viewer->getWidget()->setCursor( _oldCursor /*QCursor( Qt::ArrowCursor )*/);
                _bRejectSelection = true;
            }
            processed = TRUE;
            break;
        case SoMouseButtonEvent::BUTTON4:
            if (press) 
                zoom(viewer->getCamera(), 0.05f);

            processed = TRUE;
            break;
        case SoMouseButtonEvent::BUTTON5:
            if (press) 
                zoom(viewer->getCamera(), -0.05f);

            processed = TRUE;
            break;
        default:
            break;
        }
    }

    // Mouse Movement handling
    if (ev->getTypeId().isDerivedFrom(SoLocation2Event::getClassTypeId())) {
//      const SoLocation2Event * const event = (const SoLocation2Event *) ev;

        if (MoveMode && ZoomMode) {
            this->zoomByCursor(posn, prevnormalized);
            processed = TRUE;
        }
        else if(MoveMode && RotMode) {
            addToLog(ev->getPosition(), ev->getTime());
            spin(posn);
            processed = TRUE;
        }
        else if(MoveMode) {
            float ratio = vp.getViewportAspectRatio();
            panCamera(viewer->getCamera(),ratio,panningplane, posn, prevnormalized);
            MoveModeMoved=true;
            processed = TRUE;

        }
        else if(_bSpining) {
            processed = TRUE;
        }
    }

    // Spaceball & Joystick handling
    if (ev->getTypeId().isDerivedFrom(SoMotion3Event::getClassTypeId())) {
        SoMotion3Event * const event = (SoMotion3Event *) ev;
        SoCamera * const camera = viewer->getCamera();
        if (camera) {
            SbVec3f dir = event->getTranslation();
            camera->orientation.getValue().multVec(dir,dir);
            camera->position = camera->position.getValue() + dir;
            camera->orientation = 
                event->getRotation() * camera->orientation.getValue();
            processed = TRUE;
        }
    }

    // give the viewprovider the chance to handle the event
    if (!processed && !MoveMode && !RotMode) {
        if (pcMouseModel) {
            int hd=pcMouseModel->handleEvent(ev,viewer->getViewportRegion());
            if (hd==AbstractMouseModel::Continue||hd==AbstractMouseModel::Restart) {
                processed = TRUE;
            }
            else if (hd==AbstractMouseModel::Finish) {
                pcPolygon = pcMouseModel->getPolygon();
                clipInner = pcMouseModel->isInner();
                delete pcMouseModel; pcMouseModel = 0;
            }
            else if (hd==AbstractMouseModel::Cancel) {
                pcPolygon.clear();
                delete pcMouseModel; pcMouseModel = 0;
            }
        }
    }

    // give the nodes in the foreground root the chance to handle events (e.g color bar)
    // Note: this must be done _before_ ceding to the viewer  
    if (!processed) {
        processed = handleEventInForeground(ev);
    }

    // invokes the appropriate callback function when user interaction has started or finished
    bool bInteraction = (MoveMode||ZoomMode||RotMode|_bSpining);
    if (bInteraction && this->getInteractiveCount()==0)
        this->interactiveCountInc();
    // must not be in seek mode because it gets decremented in setSeekMode(FALSE)
    else if (!bInteraction && !dCliBut3 && this->getInteractiveCount()>0 && !this->isSeekMode())
        this->interactiveCountDec();

    if (!processed)
        processed = inherited::processSoEvent(ev);
    else 
        return TRUE;

    // right mouse button pressed
    if (!processed && !MoveMode && !RotMode) {
        if (ev->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {
            SoMouseButtonEvent * const e = (SoMouseButtonEvent *) ev;
            if ((e->getButton() == SoMouseButtonEvent::BUTTON2) && e->getState() == SoButtonEvent::UP) {
                if (this->isPopupMenuEnabled()) {
                    if (e->getState() == SoButtonEvent::UP) {
                        this->openPopupMenu(e->getPosition());
                    }

                    // Steal all RMB-events if the viewer uses the popup-menu.
                    return TRUE;
                }
            }
        }
    }

    // check for left click without selecting something
    if (!processed) {
        if (ev->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {
            SoMouseButtonEvent * const e = (SoMouseButtonEvent *) ev;
            if (SoMouseButtonEvent::isButtonReleaseEvent(e,SoMouseButtonEvent::BUTTON1)) {
                //Base::Console().Log("unhandled left button click!");
                Gui::Selection().clearSelection();
            }
        }
    }

    return FALSE;
}
#endif

#undef PRIVATE
#undef PUBLIC
