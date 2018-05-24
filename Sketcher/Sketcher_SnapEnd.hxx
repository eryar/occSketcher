/**
* \file Sketcher_SnapEnd.hxx
* \brief Header file for the class Sketcher_SnapEnd
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapEnd.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_SnapEND_H
#define Sketcher_SnapEND_H

#include "Sketcher_Snap.hxx"
#include "Geom2d_Edge.hxx"
#include "Geom2d_Arc.hxx"


DEFINE_STANDARD_HANDLE(Sketcher_SnapEnd,Sketcher_Snap)

//snap searching end point of line/arc
class Sketcher_SnapEnd : public Sketcher_Snap
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_SnapEnd)

/**
 * \fn Sketcher_SnapEnd() 
 * \brief Constructs a Sketcher_SnapEnd
 */
Standard_EXPORT	Sketcher_SnapEnd();

/**
 * \fn ~Sketcher_SnapEnd() 
 * \brief Destructor
 */
Standard_EXPORT	~Sketcher_SnapEnd();

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