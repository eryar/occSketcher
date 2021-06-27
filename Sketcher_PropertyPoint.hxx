/**
* \file Sketcher_PropertyPoint.hxx
* \brief Header file for the class Sketcher_PropertyPoint
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_PropertyPoint.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_PropertyPoint_H
#define Sketcher_PropertyPoint_H

#include "Sketcher_Property.hxx"

#include <Geom2d_CartesianPoint.hxx>
#include <Geom_CartesianPoint.hxx>
#include <AIS_Point.hxx>

//class editing point
class Sketcher_PropertyPoint : public Sketcher_Property
{ 
    Q_OBJECT

public:

//methods

/**
 * \fn Sketcher_PropertyPoint( QWidget* parent,  const char* name, WFlags fl )
 * \brief Constructs a Sketcher_PropertyPoint which is a child of 'parent', with the name 'name' and widget flags set to 'f' 
 * \param parent QWidget*
 * \param name const char*
 * \param fl WFlags
 */
Sketcher_PropertyPoint( QWidget* parent = 0, const char* name = 0);

/**
 * \fn ~Sketcher_PropertyPoint() 
 * \brief destructor
 */
~Sketcher_PropertyPoint();
	

private:

//method

/**
 * \fn SetGeometry()
 * \brief show object geometry in dialog window
 * \return void 
 */
void SetGeometry();

/**
 * \fn CheckGeometry()
 * \brief check geometry for change
 * \return bool 
 */
bool CheckGeometry();

/**
 * \fn GetGeometry()
 * \brief create new object
 * \return bool 
 */
bool GetGeometry();

//members

Handle(Geom2d_CartesianPoint) curGeom2d_Point;

};

#endif // Sketcher_PropertyPoint_H
