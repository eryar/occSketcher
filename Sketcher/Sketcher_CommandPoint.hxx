/**
* \file Sketcher_CommandPoint.hxx
* \brief Header file for the class Sketcher_CommandPoint
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandPoint.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_CommandPOINT_H
#define Sketcher_CommandPOINT_H

#include "Sketcher_Command.hxx"
#include <AIS_Point.hxx>

DEFINE_STANDARD_HANDLE(Sketcher_CommandPoint,Sketcher_Command)

//Command entering Point
class Sketcher_CommandPoint : public Sketcher_Command
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_CommandPoint)

enum	PointAction { Nothing,Input_Point};

/**
 * \fn Sketcher_CommandPoint() 
 * \brief Constructs a Sketcher_CommandPoint
 */
Standard_EXPORT	Sketcher_CommandPoint();

/**
 * \fn ~Sketcher_CommandPoint() 
 * \brief destructor
 */
Standard_EXPORT	~Sketcher_CommandPoint();

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
PointAction		myPointAction;

};

#endif