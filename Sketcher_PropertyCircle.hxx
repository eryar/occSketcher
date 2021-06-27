/**
* \file Sketcher_PropertyCircle.hxx
* \brief Header file for the class Sketcher_PropertyCircle
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_PropertyCircle.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_PropertyCircle_H
#define Sketcher_PropertyCircle_H

#include "Sketcher_Property.hxx"

#include <Geom2d_Circle.hxx>
#include <Geom_Circle.hxx>
#include <AIS_Circle.hxx>

//class editing circle
class Sketcher_PropertyCircle : public Sketcher_Property
{ 
    Q_OBJECT

public:

//methods

/**
 * \fn Sketcher_PropertyCircle( QWidget* parent,  const char* name, WFlags fl )
 * \brief Constructs a Sketcher_PropertyCircle which is a child of 'parent', with the name 'name' and widget flags set to 'f' 
 * \param parent QWidget*
 * \param name const char*
 * \param fl WFlags
 */
Sketcher_PropertyCircle( QWidget* parent = 0, const char* name = 0);

/**
 * \fn ~Sketcher_PropertyCircle() 
 * \brief destructor
 */
~Sketcher_PropertyCircle();

//members

QLabel* TextLabelRadius;
QLineEdit* LineEditRadius;

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

Handle(Geom2d_Circle)	curGeom2d_Circle;
double					myRadius,tempRadius;

};

#endif // Sketcher_PropertyCircle_H
