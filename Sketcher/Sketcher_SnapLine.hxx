/**
* \file Sketcher_SnapLine.hxx
* \brief Header file for the class Sketcher_SnapLine
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapLine.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_SnapLine_H
#define Sketcher_SnapLine_H

#include "Sketcher_Snap.hxx"

#include <gp_Pnt2d.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Lin2d.hxx>
#include <gp_Lin.hxx>
#include <gce_MakeDir2d.hxx>
#include <Geom2d_Line.hxx>

class gp_Dir2d;
class Geom2d_Line;

DEFINE_STANDARD_HANDLE(Sketcher_SnapLine,Sketcher_Snap)

//base class of snaps searching parallel/perpendicular lines
class Sketcher_SnapLine : public Sketcher_Snap
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_SnapLine)

/**
 * \fn Sketcher_SnapLine() 
 * \brief Constructs a Sketcher_SnapLine
 */
Standard_EXPORT	Sketcher_SnapLine();

/**
 * \fn ~Sketcher_SnapLine() 
 * \brief Destructor
 */
Standard_EXPORT	~Sketcher_SnapLine();

/**
 * \fn SelectEvent() 
 * \brief find new point
 * \return void 
 */
Standard_EXPORT	virtual void SelectEvent() = 0;

/**
 * \fn GetSnapType()
 * \brief get Snap type
 * \return Sketcher_SnapType 
 */
Standard_EXPORT	virtual	Sketcher_SnapType GetSnapType() = 0;

/**
 * \fn DrawRelation()
 * \brief draw relation 
 * \return void 
 */
Standard_EXPORT	virtual void DrawRelation() = 0;

/**
 * \fn EraseRelation()
 * \brief erase relation 
 * \return void 
 */
Standard_EXPORT	virtual void EraseRelation() = 0;

/**
 * \fn setFirstPnt(const gp_Pnt2d& p)
 * \brief set point for exceptions
 * \return void 
 * \param p const gp_Pnt2d&
*/
Standard_EXPORT	void setFirstPnt(const gp_Pnt2d& p);

protected:

//methods

/**
 * \fn SelectNearest()
 * \brief select nearest line
 * \return void
 */
Standard_EXPORT	void SelectNearest();

/**
 * \fn checkparallel()
 * \brief check lines for parallel
 * \return void
 */
Standard_EXPORT	void checkparallel();

/**
 * \fn ProjectPoint()
 * \brief find point projected to line
 * \return Standard_Boolean
 */
Standard_EXPORT Standard_Boolean ProjectPoint();

/**
 * \fn ProjectPoint()
 * \brief set Edges to display relation
 * \return Standard_Boolean
 */
Standard_EXPORT	void setEdges();

//members

Standard_Real			minAngle;
Standard_Real			curAngle;

gp_Pnt2d				nearestPnt2d;
gp_Dir2d				curDir2d;
gp_Dir2d				tempDir2d;
Handle(Geom2d_Line)		curGeom2d_Line;
Handle(Geom2d_Line)		tempGeom2d_Line;
gp_Lin					mygp_Lin1,mygp_Lin2;

};

#endif