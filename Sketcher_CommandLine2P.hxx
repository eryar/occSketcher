/**
* \file Sketcher_CommandLine2P.hxx
* \brief Header file for the class Sketcher_CommandLine2P
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandLine2P.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_CommandLine2P_H
#define Sketcher_CommandLine2P_H

#include "Sketcher_Command.hxx"
#include "Geom2d_Edge.hxx"

class Geom2d_CartesianPoint;
class AIS_Line;

DEFINE_STANDARD_HANDLE(Sketcher_CommandLine2P,Sketcher_Command)

//Command entering Line by 2 Points
class Sketcher_CommandLine2P : public Sketcher_Command
{
public:
// Type management
DEFINE_STANDARD_RTTIEXT(Sketcher_CommandLine2P, Sketcher_Command)

enum	Line2PAction { Nothing,Input_FirstPointLine,Input_SecondPointLine };


/**
 * \fn Sketcher_CommandLine2P() 
 * \brief Constructs a Sketcher_CommandLine2P
 */
Standard_EXPORT	Sketcher_CommandLine2P();

/**
 * \fn ~Sketcher_CommandLine2P() 
 * \brief destructor
 */
Standard_EXPORT	~Sketcher_CommandLine2P();

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
//members

Line2PAction myLine2PAction;

};

#endif