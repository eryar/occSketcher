/**
* \file Sketcher_PropertyLine.hxx
* \brief Header file for the class Sketcher_PropertyLine
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_PropertyLine.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_PropertyLine_H
#define Sketcher_PropertyLine_H

#include "Sketcher_Property.hxx"
#include "Geom2d_Edge.hxx"

#include <Geom_CartesianPoint.hxx>
#include <AIS_Line.hxx>

//class editing line (edge)
class Sketcher_PropertyLine : public Sketcher_Property
{ 
    Q_OBJECT

public:

//methods

/**
 * \fn Sketcher_PropertyLine( QWidget* parent,  const char* name, WFlags fl )
 * \brief Constructs a Sketcher_PropertyLine which is a child of 'parent', with the name 'name' and widget flags set to 'f' 
 * \param parent QWidget*
 * \param name const char*
 * \param fl WFlags
 */
Sketcher_PropertyLine( QWidget* parent = 0, const char* name = 0);

/**
 * \fn ~Sketcher_PropertyLine() 
 * \brief destructor
 */
~Sketcher_PropertyLine();

//members

QLabel* TextLabelPoint2;
QLineEdit* LineEditPoint2;

QLabel* TextLabelLength;
QLineEdit* LineEditLength;

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

/**
 * \fn SetLineLength()
 * \brief show line length in dialog window
 * \return void 
 */
void SetLineLength();

//members

Handle(Geom2d_Edge)	curGeom2d_Edge;
gp_Pnt2d			secondPnt2d,temp2Pnt2d;

};

#endif // Sketcher_PropertyLine_H
