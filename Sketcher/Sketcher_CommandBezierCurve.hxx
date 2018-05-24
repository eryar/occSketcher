/**
* \file Sketcher_CommandBezierCurve.hxx
* \brief Header file for the class Sketcher_CommandBezierCurve
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandBezierCurve.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_CommandBezierCurve_H
#define Sketcher_CommandBezierCurve_H

#include "Sketcher_Command.hxx"

#include <TColgp_Array1OfPnt2d.hxx> 
#include <TColgp_Array1OfPnt.hxx> 
#include <Geom2d_BezierCurve.hxx>
#include <Geom_BezierCurve.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx> 
#include <TopoDS_Edge.hxx>
#include <AIS_Shape.hxx>
#define MAXIMUMPOLES 16

class AIS_Line;

DEFINE_STANDARD_HANDLE(Sketcher_CommandBezierCurve,Sketcher_Command)

//Command Managerial entering BezierCurve by points
class Sketcher_CommandBezierCurve : public Sketcher_Command
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_CommandBezierCurve)

enum BezierCurveAction { Nothing,Input_1Point,Input_2Point,Input_OtherPoints };


/**
 * \fn Sketcher_CommandBezierCurve() 
 * \brief Constructs a Sketcher_CommandBezierCurve
 */
Standard_EXPORT	Sketcher_CommandBezierCurve();

/**
 * \fn ~Sketcher_CommandBezierCurve() 
 * \brief destructor
 */
Standard_EXPORT	~Sketcher_CommandBezierCurve();

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
 * \fn closeBezierCurve()
 * \brief display Bezier Curve, add to list of objects
 * \return void 
 */
Standard_EXPORT	void closeBezierCurve();

/**
 * \fn storePoles()
 * \brief display Pole point, add to list of objects
 * \return void 
 */
Standard_EXPORT	void storePoles();

//members

BezierCurveAction			myBezierCurveAction;
Handle(Geom2d_BezierCurve)	myGeom2d_BezierCurve;
Handle(Geom_BezierCurve)	myGeom_BezierCurve;

TopoDS_Edge					curEdge;
Handle(AIS_Shape)			myRubberAIS_Shape;
gp_Pnt2d					tempPnt2d;
gp_Pnt						myFirstgp_Pnt;
gp_Pnt						tempPnt;

Standard_Integer			IndexCounter;
};

#endif