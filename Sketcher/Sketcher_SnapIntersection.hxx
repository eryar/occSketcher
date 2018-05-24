/**
* \file Sketcher_SnapIntersection.hxx
* \brief Header file for the class Sketcher_SnapIntersection
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapIntersection.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_SNAPIntersection_H
#define Sketcher_SNAPIntersection_H

#include "Sketcher_Snap.hxx"
#include "Geom2d_Edge.hxx"

#include <Geom2d_Line.hxx>
#include <Geom2d_Curve.hxx>
#include <Geom2dAPI_InterCurveCurve.hxx>
#include <GeomAPI.hxx>

class Geom2d_Curve;

DEFINE_STANDARD_HANDLE(Sketcher_SnapIntersection,Sketcher_Snap)

//snap searching intersection point of objects
class Sketcher_SnapIntersection : public Sketcher_Snap
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_SnapIntersection)

/**
 * \fn Sketcher_SnapIntersection() 
 * \brief Constructs a Sketcher_SnapIntersection
 */
Standard_EXPORT	Sketcher_SnapIntersection();

/**
 * \fn ~Sketcher_SnapIntersection() 
 * \brief Destructor
 */
Standard_EXPORT	~Sketcher_SnapIntersection();

/**
 * \fn SelectEvent() 
 * \brief find new point
 * \return void 
 */
Standard_EXPORT	void SelectEvent();

/**
 * \fn GetSnapType()
 * \brief get Snap type
 * \return Sketcher_SnapType 
 */
Standard_EXPORT	Sketcher_SnapType GetSnapType();

/**
 * \fn DrawRelation()
 * \brief draw relation 
 * \return void 
 */
Standard_EXPORT	virtual void DrawRelation();

private:

//method

/**
 * \fn findSecondObject(Standard_Integer start)
 * \brief find intersection points for object with start index
 * \return void 
 * \param start Standard_Integer
 */
Standard_EXPORT	void findSecondObject(Standard_Integer start);

//members

Handle(Sketcher_Object)		secondSObject;
Handle(Geom2d_Curve)		cur1Geom2d_Curve;
Handle(Geom2d_Curve)		cur2Geom2d_Curve;
Handle(Geom2d_Edge)			cur1Geom2d_Edge;
Handle(Geom2d_Edge)			cur2Geom2d_Edge;
Handle(Geom2d_Line)			cur1Geom2d_Line;
Handle(Geom2d_Line)			cur2Geom2d_Line;

Standard_Real				bpu;
Geom2dAPI_InterCurveCurve	my2dAPI_InterCurveCurve;

Handle(AIS_InteractiveObject)	curHilightedObj2;
};

#endif