/**
* \file Sketcher_CommandArc3P.hxx
* \brief Header file for the class Sketcher_CommandArc3P
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandArc3P.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_CommandARC3P_H
#define Sketcher_CommandARC3P_H

#include "Sketcher_Command.hxx"
#include "Geom2d_Arc.hxx"

#include <gp_Circ.hxx>
#include <gce_MakeCirc.hxx>
#include <gce_ErrorType.hxx>
#include <Geom_Circle.hxx>
#include <AIS_Circle.hxx>

#include <Geom2dGcc_Circ2d3Tan.hxx>
#include <Geom2dGcc_QualifiedCurve.hxx>
#include <gp_Circ2d.hxx>
#include <Geom2d_Circle.hxx>

class Geom_Circle;
class AIS_Line;
class AIS_Circle;

DEFINE_STANDARD_HANDLE(Sketcher_CommandArc3P,Sketcher_Command)

//Command entering Arc by 3 points
class Sketcher_CommandArc3P : public Sketcher_Command
{
public:
// Type management
DEFINE_STANDARD_RTTIEXT(Sketcher_CommandArc3P, Sketcher_Command)

enum Arc3PAction { Nothing,Input_1ArcPoint,Input_2ArcPoint,Input_3ArcPoint,Input_PolylineArc};

/**
 * \fn Sketcher_CommandArc3P() 
 * \brief Constructs a Sketcher_CommandArc3P
 */
Standard_EXPORT	Sketcher_CommandArc3P();

/**
 * \fn ~Sketcher_CommandArc3P() 
 * \brief destructor
 */
Standard_EXPORT	~Sketcher_CommandArc3P();

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

/**
 * \fn SetPolylineFirstPnt(const gp_Pnt2d& p1)
 * \brief set last entering gp_Pnt2d for polyline
 * \return void 
 * \param p1 const gp_Pnt2d&
 */
Standard_EXPORT	virtual void SetPolylineFirstPnt(const gp_Pnt2d& p1);

/**
 * \fn GetPolylineFirstPnt(gp_Pnt2d& p1)
 * \brief get last entering gp_Pnt2d for polyline
 * \return Standard_Boolean 
 * \param p1 gp_Pnt2d&
 */
Standard_EXPORT	virtual Standard_Boolean GetPolylineFirstPnt(gp_Pnt2d& p1);

/**
 * \fn SetPolylineMode(Standard_Boolean mode)
 * \brief set polyline mode
 * \return void 
 * \param mode Standard_Boolean 
 */
Standard_EXPORT	void SetPolylineMode(Standard_Boolean mode);

private:
//methods
/**
 * \fn findlastSObject()
 * \brief find last object at list of objects
 * \return void 
 */
Standard_EXPORT	void findlastSObject();

/**
 * \fn setTempLine()
 * \brief set default horisontal line
 * \return void 
 */
Standard_EXPORT	void setTempLine();

//members
Arc3PAction						myArc3PAction;

Handle(Geom_Circle)				tempGeom_Circle;
Handle(AIS_Circle)				myRubberCircle;

gp_Pnt2d						mySecondgp_Pnt2d;
gp_Circ							tempCirc;
gp_Pnt							third_Pnt;

Geom2dAdaptor_Curve				temp2dAdaptor_Curve;
gp_Circ2d						temp2d_Circ;
Handle(Geom2d_Circle)			tempGeom2d_Circle;

gp_Pnt2d						midpoint2d,tempu1_pnt2d,tempu2_pnt2d;
Standard_Real					u1,u2,temp_u1, temp_u2;
Standard_Real					dist1, dist2;

Handle(Geom2d_Line)				tempGeom2d_Line;
Handle(Geom2d_CartesianPoint)	FirstGeom2d_Point;
Handle(Geom2d_CartesianPoint)	TempGeom2d_Point;

};

#endif