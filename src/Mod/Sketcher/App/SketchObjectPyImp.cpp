
#include "PreCompiled.h"

#include "Mod/Sketcher/App/SketchObject.h"
#include <Mod/Part/App/LinePy.h>
#include <Mod/Part/App/Geometry.h>
#include <Base/VectorPy.h>

// inclusion of the generated files (generated out of SketchObjectSFPy.xml)
#include "SketchObjectPy.h"
#include "SketchObjectPy.cpp"
// other python types
#include "ConstraintPy.h"

using namespace Sketcher;

// returns a string which represents the object e.g. when printed in python
std::string SketchObjectPy::representation(void) const
{
    return "<Sketcher::SketchObject>";
}


PyObject* SketchObjectPy::solve(PyObject *args)
{
    PyErr_SetString(PyExc_NotImplementedError, "Not yet implemented");
    return 0;
}

PyObject* SketchObjectPy::addGeometry(PyObject *args)
{
    PyObject *pcObj;
    if (!PyArg_ParseTuple(args, "O", &pcObj))
        return 0;

    if (PyObject_TypeCheck(pcObj, &(Part::GeometryPy::Type))) {
        Part::Geometry *geo = static_cast<Part::GeometryPy*>(pcObj)->getGeometryPtr();
        return Py::new_reference_to(Py::Int(this->getSketchObjectPtr()->addGeometry(geo)));
    }
    Py_Return; 
}

PyObject* SketchObjectPy::addConstraint(PyObject *args)
{
    PyObject *pcObj;
    if (!PyArg_ParseTuple(args, "O", &pcObj))
        return 0;

    if (PyObject_TypeCheck(pcObj, &(Sketcher::ConstraintPy::Type))) {
        Sketcher::Constraint *constr = static_cast<Sketcher::ConstraintPy*>(pcObj)->getConstraintPtr();
        return Py::new_reference_to(Py::Int(this->getSketchObjectPtr()->addConstraints(constr)));
    }
    Py_Return; 
}

PyObject* SketchObjectPy::setDatum(PyObject *args)
{
    double Datum;
    int    Index;
    if (!PyArg_ParseTuple(args, "di", &Datum,&Index))
        return 0;

    this->getSketchObjectPtr()->setDatum(Datum,Index);

    Py_Return; 
}

PyObject* SketchObjectPy::movePoint(PyObject *args)
{
    PyObject *pcObj;
    int GeoId,PointType;

    if (!PyArg_ParseTuple(args, "iiO!", &GeoId, &PointType, &(Base::VectorPy::Type), &pcObj))
        return 0;

    Base::Vector3d v1 = static_cast<Base::VectorPy*>(pcObj)->value();

    const std::vector< Part::Geometry * > &vals = this->getSketchObjectPtr()->Geometry.getValues();
    const Part::Geometry * actGeom = vals[GeoId];
    if (actGeom->getTypeId() == Part::GeomLineSegment::getClassTypeId()) {
        const Part::GeomLineSegment * Line = static_cast<const Part::GeomLineSegment*>(actGeom);
        // create a single new line segment
        Part::GeomLineSegment *newLine = new Part::GeomLineSegment();
        // set the right point, leave the other old
        if (PointType == 1)
            newLine->setPoints(v1,Line->getEndPoint());
        else
            newLine->setPoints(Line->getStartPoint(),v1);
        //copy the vector and exchange the changed line segment
        std::vector< Part::Geometry * > newVals(vals);
        newVals[GeoId] = newLine;

        // set the new set to the property (which clone the objects)
        this->getSketchObjectPtr()->Geometry.setValues(newVals);

        // set free the new line
        delete newLine;

    }else
        Py_Error(PyExc_AttributeError,"wrong Geometry");



    Py_Return; 
}


Py::Int SketchObjectPy::getConstraintCount(void) const
{
    //return Py::Int();
    throw Py::AttributeError("Not yet implemented");
}

Py::Int SketchObjectPy::getGeometryCount(void) const
{
    //return Py::Int();
    throw Py::AttributeError("Not yet implemented");
}

PyObject *SketchObjectPy::getCustomAttributes(const char* /*attr*/) const
{
    return 0;
}

int SketchObjectPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0; 
}


