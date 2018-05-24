/**
* \file Sketcher_SnapMiddle.hxx
* \brief Header file for the class Sketcher_SnapMiddle
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapMiddle.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_SnapMIDDLE_H
#define Sketcher_SnapMIDDLE_H

#include "Sketcher_Snap.hxx"
#include "Geom2d_Edge.hxx"
#include "Geom2d_Arc.hxx"

DEFINE_STANDARD_HANDLE(Sketcher_SnapMiddle,Sketcher_Snap)

//snap searching middle points of line/arc
class Sketcher_SnapMiddle : public Sketcher_Snap
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_SnapMiddle)

/**
 * \fn Sketcher_SnapMiddle() 
 * \brief Constructs a Sketcher_SnapMiddle
 */
Standard_EXPORT	Sketcher_SnapMiddle();

/**
 * \fn ~Sketcher_SnapMiddle() 
 * \brief Destructor
 */
Standard_EXPORT	~Sketcher_SnapMiddle();

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

Handle(Geom2d_Edge)	curGeom2d_Edge;
Handle(Geom2d_Arc)	curGeom2d_Arc;

};

#endif