/**
* \file Sketcher_QtGUI.hxx
* \brief Header file for the class Sketcher_QtGUI
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_QtGUI.hxx">Sergei Maslov</a>
*/
#ifndef SKETCHER_QTGUI_H
#define SKETCHER_QTGUI_H

#include "Sketcher_GUI.hxx"
#include "Sketcher_Object.hxx"

#include "Sketcher_PropertyArc.hxx"
#include "Sketcher_PropertyCircle.hxx"
#include "Sketcher_PropertyLine.hxx"
#include "Sketcher_PropertyPoint.hxx"

#include <qwidget.h>

class Sketcher_QtGUI : public Sketcher_GUI
{ 
public:

/**
 * \fn Sketcher_QtGUI( QWidget* parent)
 * \brief Constructs a Sketcher_QtGUI
 * \param parent QWidget*
 */
Sketcher_QtGUI(QWidget* parent);

/**
 * \fn ~Sketcher_GUI() 
 * \brief destructor
 */
~Sketcher_QtGUI();

/**
 * \fn SetContext(Handle(AIS_InteractiveContext)& theContext)
 * \brief pass context to property classes
 * \return void 
 * \param theContext Handle(AIS_InteractiveContext)&
 */
void SetContext (Handle(AIS_InteractiveContext)& theContext);

/**
 * \fn SetAx3(const gp_Ax3& theAx3)
 * \brief pass coordinate system to property classes
 * \return void 
 * \param theAx3 const gp_Ax3&
 */
void SetAx3 (const gp_Ax3& theAx3);		

/**
 * \fn SetSketcher_Object(Handle(Sketcher_Object)& CurObject)
 * \brief determine property to show, pass object to this property class
 * \return void 
 * \param CurObject Handle(Sketcher_Object)&
 */
void SetSketcher_Object(Handle(Sketcher_Object)& CurObject);

private:

//members

Sketcher_PropertyArc*		prop_arc;
Sketcher_PropertyCircle*	prop_circle;
Sketcher_PropertyLine*		prop_line;
Sketcher_PropertyPoint*		prop_point;

};

#endif // SKETCHER_QTGUI_H