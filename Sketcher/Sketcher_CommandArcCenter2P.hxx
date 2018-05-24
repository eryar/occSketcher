/**
* \file Sketcher_CommandArcCenter2P.hxx
* \brief Header file for the class Sketcher_CommandArcCenter2P
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandArcCenter2P.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_CommandArcCenter2P_H
#define Sketcher_CommandArcCenter2P_H

#include "Sketcher_Command.hxx"
#include "Geom2d_Arc.hxx"

#include <AIS_Point.hxx>
#include <gp_Circ2d.hxx>
#include <Geom2d_Circle.hxx>
#include <Geom2dGcc_Circ2d3Tan.hxx>
#include <Geom2dGcc_QualifiedCurve.hxx>
#include <Geom2dAPI_ProjectPointOnCurve.hxx>

#include <gp_Circ.hxx>
#include <Geom_Circle.hxx>
#include <AIS_Circle.hxx>
#include <gce_MakeCirc.hxx>
#include <gce_ErrorType.hxx>
#include <GeomAPI.hxx>

class Geom_Circle;
class AIS_Line;
class AIS_Circle;

DEFINE_STANDARD_HANDLE(Sketcher_CommandArcCenter2P,Sketcher_Command)

//Command entering Arc by center & 2 points
class Sketcher_CommandArcCenter2P : public Sketcher_Command
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_CommandArcCenter2P)

enum ArcCenter2PAction { Nothing,Input_CenterArc,Input_1ArcPoint,Input_MidPoint,Input_2ArcPoint};

/**
 * \fn Sketcher_CommandArcCenter2P() 
 * \brief Constructs a Sketcher_CommandArcCenter2P
 */
Standard_EXPORT	Sketcher_CommandArcCenter2P();

/**
 * \fn ~Sketcher_CommandArcCenter2P() 
 * \brief destructor
 */
Standard_EXPORT	~Sketcher_CommandArcCenter2P();

/**
 * \fn Action() 
 * \brief turn command to active state 
 */
Standard_EXPORT	void Action();

/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d )
 * \brief input event handler
 * \return Standard_Boolean 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_EXPORT	Standard_Boolean MouseInputEvent(const gp_Pnt2d& thePnt2d);

/**
 * \fn MouseMoveEvent(const gp_Pnt2d& thePnt2d )
 * \brief mouse move handler
 * \return void 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_EXPORT	void MouseMoveEvent(const gp_Pnt2d& thePnt2d);

/**
 * \fn CancelEvent()
 * \brief cancel event handler, stop entering object
 * \return void 
 */
Standard_EXPORT	void CancelEvent();

/**
 * \fn GetTypeOfMethod()
 * \brief get command Method
 * \return Sketcher_ObjectTypeOfMethod 
 */
Standard_EXPORT	Sketcher_ObjectTypeOfMethod GetTypeOfMethod();

private:

//methods

/**
 * \fn ProjectOnCircle()
 * \brief project point on circle
 * \return Standard_Boolean 
 */
Standard_EXPORT Standard_Boolean ProjectOnCircle(const gp_Pnt2d& thePnt2d);

//members

ArcCenter2PAction		myArcCenter2PAction;

Handle(AIS_Point)		myRubberCenterPoint;
gp_Pnt2d				mySecondgp_Pnt2d;
gp_Pnt2d				myMidgp_Pnt2d;
Handle(Geom2d_Circle)	tempGeom2d_Circle;

gp_Pnt					third_Pnt;
Handle(Geom_Circle)		tempGeom_Circle;
Handle(AIS_Circle)		myRubberCircle;
Standard_Real			radius;

Geom2dAPI_ProjectPointOnCurve ProjectOnCurve;


};

#endif