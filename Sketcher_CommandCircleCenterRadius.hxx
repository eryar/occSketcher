/**
* \file Sketcher_CommandCircleCenterRadius.hxx
* \brief Header file for the class Sketcher_CommandCircleCenterRadius
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandCircleCenterRadius.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_CommandCIRCLECENTERRADIUS_H
#define Sketcher_CommandCIRCLECENTERRADIUS_H

#include "Sketcher_Command.hxx"

#include <Geom2d_CartesianPoint.hxx>
#include <Geom2d_Circle.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_Circle.hxx>
#include <AIS_Circle.hxx>
#include <gp_Ax2d.hxx>

class Geom_Circle;
class AIS_Circle;
class AIS_Line;

DEFINE_STANDARD_HANDLE(Sketcher_CommandCircleCenterRadius,Sketcher_Command)

//Command entering Circle by Center point and other point
class Sketcher_CommandCircleCenterRadius : public Sketcher_Command
{
public:
// Type management
DEFINE_STANDARD_RTTIEXT(Sketcher_CommandCircleCenterRadius, Sketcher_Command)

enum	CircleCenterRadiusAction { Nothing,Input_CenterPoint,Input_RadiusPoint };

/**
 * \fn Sketcher_CommandCircleCenterRadius() 
 * \brief Constructs a Sketcher_CommandCircleCenterRadius
 */
Standard_EXPORT	Sketcher_CommandCircleCenterRadius();

/**
 * \fn ~Sketcher_CommandCircleCenterRadius() 
 * \brief destructor
 */
Standard_EXPORT	~Sketcher_CommandCircleCenterRadius();

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

//members
CircleCenterRadiusAction		myCircleCenterRadiusAction;
Standard_Real					radius;

Handle(Geom_Circle)				tempGeom_Circle;
Handle(AIS_Circle)				myRubberCircle;
gp_Ax2d							myCircleAx2d;
};

#endif