/**
* \file Sketcher_SnapLinePerpendicular.hxx
* \brief Header file for the class Sketcher_SnapLinePerpendicular
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapLinePerpendicular.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_SnapLinePerpendicular_H
#define Sketcher_SnapLinePerpendicular_H

#include "Sketcher_SnapLine.hxx"
#include <AIS_PerpendicularRelation.hxx>

DEFINE_STANDARD_HANDLE(Sketcher_SnapLinePerpendicular,Sketcher_SnapLine)

//snap searching perpendicular lines
class Sketcher_SnapLinePerpendicular : public Sketcher_SnapLine
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_SnapLinePerpendicular)

/**
 * \fn Sketcher_SnapLinePerpendicular() 
 * \brief Constructs a Sketcher_SnapLinePerpendicular
 */
Standard_EXPORT	Sketcher_SnapLinePerpendicular();

/**
 * \fn ~Sketcher_SnapLinePerpendicular() 
 * \brief Destructor
 */
Standard_EXPORT	~Sketcher_SnapLinePerpendicular();

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

/**
 * \fn DrawRelation()
 * \brief draw relation 
 * \return void 
 */
Standard_EXPORT	void DrawRelation();

/**
 * \fn EraseRelation()
 * \brief erase relation 
 * \return void 
 */
Standard_EXPORT	void EraseRelation();

private:

//members

Handle(AIS_PerpendicularRelation) myPerRelation;

};

#endif