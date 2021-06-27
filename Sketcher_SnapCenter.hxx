/**
* \file Sketcher_SnapCenter.hxx
* \brief Header file for the class Sketcher_SnapCenter
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapCenter.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_SnapCENTER_H
#define Sketcher_SnapCENTER_H

#include "Sketcher_Snap.hxx"
#include <Geom2d_Circle.hxx>

class Geom2d_Circle;

DEFINE_STANDARD_HANDLE(Sketcher_SnapCenter,Sketcher_Snap)

//snap searching center curve/circle/arc
class Sketcher_SnapCenter : public Sketcher_Snap
{
public:
// Type management
DEFINE_STANDARD_RTTIEXT(Sketcher_SnapCenter, Sketcher_Snap)

/**
 * \fn Sketcher_SnapCenter() 
 * \brief Constructs a Sketcher_SnapCenter
 */
Standard_EXPORT	Sketcher_SnapCenter();

/**
 * \fn ~Sketcher_SnapCenter() 
 * \brief Destructor
 */
Standard_EXPORT	~Sketcher_SnapCenter();

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

private:

//members

Handle(Geom2d_Circle)	curGeom2d_Circle;

};

#endif