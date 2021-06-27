/**
* \file Sketcher_SnapTangent.hxx
* \brief Header file for the class Sketcher_SnapTangent
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapTangent.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_SnapTangent_H
#define Sketcher_SnapTangent_H

#include "Sketcher_Snap.hxx"
#include "Geom2d_Edge.hxx"
#include "Geom2d_Arc.hxx"
#include <Geom2d_Line.hxx>
#include <Geom2d_Circle.hxx>
#include <Geom2d_Curve.hxx>

#include <Geom2dAdaptor_Curve.hxx>
#include <GccEnt_Position.hxx>
#include <Geom2dGcc_QualifiedCurve.hxx>
#include <Geom2dGcc_Lin2d2Tan.hxx>
#include <Geom2dGcc_Circ2dTanCen.hxx>
#include <Geom2dAPI_InterCurveCurve.hxx>
#include <gce_MakeDir2d.hxx>

#include <gp_Pnt2d.hxx>
#include <GeomAPI.hxx>
#include <AIS_TangentRelation.hxx>

DEFINE_STANDARD_HANDLE(Sketcher_SnapTangent,Sketcher_Snap)

//snap searching tangent to objects
class Sketcher_SnapTangent : public Sketcher_Snap
{
public:
// Type management
DEFINE_STANDARD_RTTIEXT(Sketcher_SnapTangent, Sketcher_Snap)

/**
 * \fn Sketcher_SnapTangent() 
 * \brief Constructs a Sketcher_SnapTangent
 */
Standard_EXPORT	Sketcher_SnapTangent();

/**
 * \fn ~Sketcher_SnapTangent() 
 * \brief Destructor
 */
Standard_EXPORT	~Sketcher_SnapTangent();

/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d)
 * \brief input event handler
 * \return gp_Pnt2d 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_EXPORT gp_Pnt2d MouseInputEvent(const gp_Pnt2d& tempPnt2d);

/**
 * \fn EraseSnap()
 * \brief cancel event handler
 * \return void 
 */
Standard_EXPORT	void EraseSnap();

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
Standard_EXPORT	void DrawRelation();

/**
 * \fn EraseRelation()
 * \brief erase relation 
 * \return void 
 */
Standard_EXPORT	void EraseRelation();

/**
 * \fn setFirstPnt(const gp_Pnt2d& p)
 * \brief set point and tangent type for exceptions
 * \return void 
 * \param p const gp_Pnt2d&
 * \param ttype TangentType
*/
Standard_EXPORT	void setFirstPnt(const gp_Pnt2d& p,TangentType ttype);

private:

// methods

/**
 * \fn SelectLine_FirstPntEvent()
 * \brief Find nearest point on curve and store it
 * \return void 
*/
Standard_EXPORT	void SelectLine_FirstPntEvent();

/**
 * \fn SelectLine_CheckFirstPntEvent()
 * \brief check first point of line belonging to circle/arc
 * \return void 
*/
Standard_EXPORT	void SelectLine_CheckFirstPntEvent();

/**
 * \fn SelectLine_FixEvent()
 * \brief find tangent line at first point
 * \return void 
*/
Standard_EXPORT	void SelectLine_FixEvent();

/**
 * \fn SelectLine_SecondPntEvent()
 * \brief find tangent line at second point
 * \return void 
*/
Standard_EXPORT	void SelectLine_SecondPntEvent();

/**
 * \fn SelectCurveCenterEvent()
 * \brief find tangent circle to curve
 * \return void 
*/
Standard_EXPORT	void SelectCurveCenterEvent();


//members

TangentType						myTangentType;
Standard_Real					radiusDistance;

gp_Pnt2d						myFirstPnt2d;

//for tangent line first pnt
Standard_Boolean				FixFirstPnt;
Standard_Real					minAngle;
Standard_Real					curAngle;
gp_Lin2d						mylin2d;
gp_Dir2d						tempDir2d;

Handle(Geom2d_Arc)				curGeom2d_Arc;
Handle(Geom2d_Circle)			curGeom2d_Circle;

Handle(Geom2d_Line)				tempGeom2d_Line;
Handle(Geom2d_Circle)			tempGeom2d_Circle;
Handle(Geom2d_Curve)			tempGeom2d_Curve;

Geom2dAPI_InterCurveCurve		my2dAPI_InterCurveCurve;
Handle(AIS_TangentRelation)		myTanRelation;


};

#endif