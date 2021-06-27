/**
* \file Sketcher_Property.hxx
* \brief Header file for the class Sketcher_Property
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_Property.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_Property_H
#define Sketcher_Property_H

#include <qvariant.h>
#include <qwidget.h>

#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include <AIS_InteractiveObject.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_LineAspect.hxx>

#include <TCollection_ExtendedString.hxx>
#include <ElCLib.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Ax3.hxx>

#include "Sketcher_Object.hxx"


class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;

//base class for editing object classes
class Sketcher_Property : public QWidget
{ 
    Q_OBJECT

public:

//methods

/**
 * \fn Sketcher_Property( QWidget* parent,  const char* name, WFlags fl )
 * \brief Constructs a Sketcher_Property which is a child of 'parent', with the name 'name' and widget flags set to 'f' 
 * \param parent QWidget*
 * \param name const char*
 * \param fl WFlags
 */
Sketcher_Property( QWidget* parent = 0, const char* name = 0);

/**
 * \fn ~Sketcher_Property() 
 * \brief destructor
 */
~Sketcher_Property();


/**
 * \fn SetContext(Handle(AIS_InteractiveContext)& theContext)
 * \brief set context
 * \return void 
 * \param theContext Handle(AIS_InteractiveContext)&
 */
void SetContext (Handle(AIS_InteractiveContext)& theContext);

/**
 * \fn SetAx3(const gp_Ax3& theAx3)
 * \brief set coordinate system
 * \return void 
 * \param theAx3 const gp_Ax3&
 */
void SetAx3 (const gp_Ax3& theAx3);		

/**
 * \fn SetObject(Handle(Sketcher_Object)& CurObject)
 * \brief set Sketcher_Object, show dialog window
 * \return void 
 * \param CurObject Handle(Sketcher_Object)&
 */
void SetObject(Handle(Sketcher_Object)& CurObject);

//members

QLabel* TextLabelID;
QLineEdit* LineEditID;

QGroupBox* GroupBoxGP;
QLabel* TextLabelPoint1;
QLineEdit* LineEditPoint1;

QGroupBox* GroupBoxAttributes;
QLabel* TextLabelColor;
QComboBox* ComboBoxColor;
QLabel* TextLabelType;
QComboBox* ComboBoxType;

QLabel*	TextLabelStyle;
QComboBox* ComboBoxStyle;
QLabel* TextLabelWidth;
QComboBox* ComboBoxWidth;

QPushButton* PushButtonOK;
QPushButton* PushButtonCancel;
QPushButton* PushButtonApply;

private slots:
/**
* \fn onOK()
* \brief clicking on "OK" button handler
* \return void 
*/
void onOK();

/**
 * \fn onApply()
 * \brief clicking on "Apply" button handler
 * \return void 
 */
void onApply();

protected:

//methods

/**
 * \fn Init()
 * \brief Constructs a supplementary elements 
 * \return void 
 */
void Init();

/**
 * \fn SetGeometry()
 * \brief show object geometry in dialog window
 * \return void 
 */
virtual void SetGeometry() = 0;

/**
 * \fn CheckGeometry()
 * \brief check geometry for change
 * \return bool 
 */
virtual bool CheckGeometry() = 0;

/**
 * \fn GetGeometry()
 * \brief create new object
 * \return bool 
 */
virtual bool GetGeometry() = 0;

/**
 * \fn SetID()
 * \brief set object name to LineEditID
 * \return void 
 */
void SetID ();

/**
 * \fn GetID()
 * \brief get text from LineEditID
 * \return TCollection_ExtendedString 
 */
TCollection_ExtendedString GetID ();

/**
 * \fn SetCoord(QLineEdit* le,const gp_Pnt2d& p)
 * \brief set point coordinate to le
 * \return void 
 * \param le QLineEdit*
 * \param p const gp_Pnt2d&
 */
void SetCoord(QLineEdit* le,const gp_Pnt2d& p);

/**
 * \fn CheckCoord(QLineEdit* le,const gp_Pnt2d& p)
 * \brief get point coordinate from le
 * \return bool 
 * \param le QLineEdit*
 * \param p const gp_Pnt2d&
 */
bool CheckCoord(QLineEdit* le, gp_Pnt2d& p);

/**
 * \fn CheckAttributies()
 * \brief check attributes for change
 * \return void 
 */
void CheckAttributies();

/**
 * \fn GetAttributies()
 * \brief set attributes to object
 * \return void 
 */
void GetAttributies();

/**
 * \fn GetName()
 * \brief check name and type of object for change
 * \return void 
 */
void GetName();

/**
 * \fn SetColor()
 * \brief show object color in dialog window
 * \return void 
 */
void SetColor();

/**
 * \fn GetColor()
 * \brief get object color from dialog window
 * \return Quantity_NameOfColor 
 */
Quantity_NameOfColor GetColor();

/**
 * \fn SetObjectType()
 * \brief show object type in dialog window
 * \return void 
 */
void SetObjectType();

/**
 * \fn GetObjectType()
 * \brief get object type from dialog window
 * \return Sketcher_ObjectType 
 */
Sketcher_ObjectType GetObjectType();

/**
 * \fn SetObjectStyle()
 * \brief show object line style in dialog window
 * \return void 
 */
void SetObjectStyle();

/**
 * \fn GetObjectStyle()
 * \brief get object line style from dialog window
 * \return Aspect_TypeOfLine 
 */
Aspect_TypeOfLine GetObjectStyle();

/**
 * \fn SetWidth()
 * \brief show object line width in dialog window
 * \return void 
 */
void SetWidth();

/**
 * \fn GetWidth()
 * \brief get object line width from dialog window
 * \return Standard_Real 
 */
Standard_Real GetWidth();


//members

QGridLayout*					PropertyLayout;
QGridLayout*					GroupBoxGPLayout;
QGridLayout*					GroupBoxAttributesLayout;

bool							isPointWindow;

const QString					StartCoord,MidCoord,EndCoord;
const QRegExp					NumberExpr;
QString							NumName;

Handle(AIS_InteractiveContext)	myContext;
gp_Ax3							myCoordinateSystem;

Handle(Sketcher_Object)			mySObject;
Handle(AIS_InteractiveObject)	myAIS_Object;
TCollection_ExtendedString		myID,tempID;
Quantity_NameOfColor			myNameOfColor;
Sketcher_ObjectType				myObjectType;
Aspect_TypeOfLine				myObjectStyle;
Standard_Real					myWidth;

Handle(Prs3d_Drawer)			myDrawer;
Handle(Prs3d_LineAspect)		myPrs3dAspect;
gp_Pnt2d						firstPnt2d,tempPnt2d;

};

#endif // Sketcher_Property_H
