/**
* \file Sketcher_SnapLineParallel.hxx
* \brief Header file for the class Sketcher_SnapLineParallel
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapLineParallel.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_SnapLineParallel_H
#define Sketcher_SnapLineParallel_H

#include "Sketcher_SnapLine.hxx"
#include <AIS_ParallelRelation.hxx>
 

DEFINE_STANDARD_HANDLE(Sketcher_SnapLineParallel,Sketcher_SnapLine)

//snap searching parallel lines
class Sketcher_SnapLineParallel : public Sketcher_SnapLine
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_SnapLineParallel)

/**
 * \fn Sketcher_SnapLineParallel() 
 * \brief Constructs a Sketcher_SnapLineParallel
 */
Standard_EXPORT	Sketcher_SnapLineParallel();

/**
 * \fn ~Sketcher_SnapLineParallel() 
 * \brief Destructor
 */
Standard_EXPORT	~Sketcher_SnapLineParallel();

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

Handle(AIS_ParallelRelation) myParRelation;

};

#endif