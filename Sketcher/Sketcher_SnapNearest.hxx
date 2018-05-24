/**
* \file Sketcher_SnapNearest.hxx
* \brief Header file for the class Sketcher_SnapNearest
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapNearest.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_SnapNEAREST_H
#define Sketcher_SnapNEAREST_H

#include "Sketcher_Snap.hxx"
#include <Geom2d_Curve.hxx>

class Geom2d_Curve;

DEFINE_STANDARD_HANDLE(Sketcher_SnapNearest,Sketcher_Snap)

//snap searching nearest point at line/circle/arc
class Sketcher_SnapNearest : public Sketcher_Snap
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_SnapNearest)

/**
 * \fn Sketcher_SnapNearest() 
 * \brief Constructs a Sketcher_SnapNearest
 */
Standard_EXPORT	Sketcher_SnapNearest();

/**
 * \fn ~Sketcher_SnapNearest() 
 * \brief Destructor
 */
Standard_EXPORT	~Sketcher_SnapNearest();

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

Handle(Geom2d_Curve)	curGeom2d_Curve;

};

#endif