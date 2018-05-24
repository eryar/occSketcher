/**
* \file Sketcher_CommandCircle3P.hxx
* \brief Header file for the class Sketcher_CommandCircle3P
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandCircle3P.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_CommandCIRCLE3P_H
#define Sketcher_CommandCIRCLE3P_H

#include "Sketcher_Command.hxx"

#include <gp_Circ.hxx>
#include <gce_MakeCirc2d.hxx>
#include <gce_MakeCirc.hxx>
#include <gce_ErrorType.hxx>
#include <Geom_Circle.hxx>
#include <AIS_Circle.hxx>

class Geom_Circle;
class AIS_Line;
class AIS_Circle;

DEFINE_STANDARD_HANDLE(Sketcher_CommandCircle3P,Sketcher_Command)

//Command entering Circle by 3 points
class Sketcher_CommandCircle3P : public Sketcher_Command
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_CommandCircle3P)

enum	Circle3PAction { Nothing,Input_1CirclePoint,Input_2CirclePoint,Input_3CirclePoint };

/**
 * \fn Sketcher_CommandCircle3P() 
 * \brief Constructs a Sketcher_CommandCircle3P
 */
Standard_EXPORT	Sketcher_CommandCircle3P();

/**
 * \fn ~Sketcher_CommandCircle3P() 
 * \brief destructor
 */
Standard_EXPORT	~Sketcher_CommandCircle3P();

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
Circle3PAction				myCircle3PAction;

Handle(Geom_Circle)			tempGeom_Circle;
Handle(AIS_Circle)			myRubberCircle;

gp_Pnt2d					mySecondgp_Pnt2d;
gp_Circ						tempCirc;
gp_Pnt						third_Pnt;

};

#endif