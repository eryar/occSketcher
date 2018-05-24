/**
* \file Sketcher_GUI.hxx
* \brief Header file for the class Sketcher_GUI
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_GUI.hxx">Sergei Maslov</a>
*/
#ifndef SKETCHER_GUI_H
#define SKETCHER_GUI_H

#include "Sketcher_Object.hxx"
#include <gp_Ax3.hxx>


class Sketcher_GUI
{ 
public:

/**
 * \fn Sketcher_GUI() 
 * \brief Constructs a Sketcher_GUI
 */
Sketcher_GUI();
/**
 * \fn ~Sketcher_GUI() 
 * \brief destructor
 */
~Sketcher_GUI();

/**
 * \fn SetContext(Handle(AIS_InteractiveContext)& theContext)
 * \brief pass context to property classes
 * \return void 
 * \param theContext Handle(AIS_InteractiveContext)&
 */
virtual void SetContext (Handle(AIS_InteractiveContext)& theContext) = 0;

/**
 * \fn SetAx3(const gp_Ax3& theAx3)
 * \brief pass coordinate system to property classes
 * \return void 
 * \param theAx3 const gp_Ax3&
 */
virtual void SetAx3 (const gp_Ax3& theAx3) = 0;		

/**
 * \fn SetSketcher_Object(Handle(Sketcher_Object)& CurObject)
 * \brief determine property to show, pass object to this property class
 * \return void 
 * \param CurObject Handle(Sketcher_Object)&
 */
virtual void SetSketcher_Object(Handle(Sketcher_Object)& CurObject) = 0;

};

#endif