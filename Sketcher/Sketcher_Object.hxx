/**
* \file Sketcher_Object.hxx
* \brief Header file for the class Sketcher_Object
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_Object.hxx">Sergei Maslov</a>
*/
#ifndef Sketcher_Object_H
#define Sketcher_Object_H

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#include <MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>

#include <Geom2d_Geometry.hxx>
#include <AIS_InteractiveObject.hxx>
#include <TCollection_ExtendedString.hxx>

#include "Sketcher_ObjectGeometryType.hxx"
#include "Sketcher_ObjectTypeOfMethod.hxx"
#include <Quantity_NameOfColor.hxx>
#include "Sketcher_ObjectType.hxx"
#include <Aspect_TypeOfLine.hxx>

class AIS_InteractiveObject;
class Geom2d_Geometry;

DEFINE_STANDARD_HANDLE(Sketcher_Object,MMgt_TShared)

//store object and all it's attributes
class Sketcher_Object : public MMgt_TShared
{
public:
// Type management
	DEFINE_STANDARD_RTTI(Sketcher_Object)

/**
 * \fn Sketcher_Object(const Handle(Geom2d_Geometry)& theGeom2d_Geometry,const Handle(AIS_InteractiveObject)& theAIS_InteractiveObject, const TCollection_ExtendedString& theName, const Sketcher_ObjectGeometryType theGeometryType, const Sketcher_ObjectTypeOfMethod theTypeOfMethod)
 * \brief Constructs a Sketcher_Object
 * \param theGeom2d_Geometry const Handle(Geom2d_Geometry)&
 * \param theAIS_InteractiveObject const TCollection_ExtendedString&
 * \param theName const TCollection_ExtendedString&
 * \param theGeometryType const Sketcher_ObjectGeometryType
 * \param theTypeOfMethod const Sketcher_ObjectTypeOfMethod
 */
Standard_EXPORT	Sketcher_Object(const Handle(Geom2d_Geometry)& theGeom2d_Geometry,const Handle(AIS_InteractiveObject)& theAIS_InteractiveObject,const TCollection_ExtendedString& theName,const Sketcher_ObjectGeometryType theGeometryType,const Sketcher_ObjectTypeOfMethod theTypeOfMethod);

/**
 * \fn ~Sketcher_Object() 
 * \brief destructor
 */
Standard_EXPORT	~Sketcher_Object();

/**
 * \fn SetGeometry(const Handle(Geom2d_Geometry)& theGeom2d_Geometry)
 * \brief set Geom2d_Geometry
 * \return void 
 * \param theGeom2d_Geometry const Handle(Geom2d_Geometry)&
 */
Standard_EXPORT	void SetGeometry(const Handle(Geom2d_Geometry)& theGeom2d_Geometry);
/**
 * \fn GetGeometry()
 * \brief get Geom2d_Geometry
 * \return Handle(Geom2d_Geometry)
 */
Standard_EXPORT	Handle(Geom2d_Geometry) GetGeometry();

/**
 * \fn SetAIS_Object(const Handle(AIS_InteractiveObject)& theAIS_InteractiveObject)
 * \brief set object representation
 * \return void 
 * \param theAIS_InteractiveObject const Handle(AIS_InteractiveObject)&
 */

Standard_EXPORT	void SetAIS_Object(const Handle(AIS_InteractiveObject)& theAIS_InteractiveObject);
/**
 * \fn GetAIS_Object()
 * \brief get object represent
 * \return Handle(AIS_InteractiveObject)
 */
Standard_EXPORT	Handle(AIS_InteractiveObject) GetAIS_Object();

/**
 * \fn SetObjectName(const Handle(TCollection_ExtendedString)& theName)
 * \brief set object name
 * \return void 
 * \param theName const Handle(TCollection_ExtendedString)&
 */
Standard_EXPORT	void SetObjectName(const TCollection_ExtendedString& theName);

/**
 * \fn GetObjectName()
 * \brief get object name
 * \return TCollection_ExtendedString
 */
Standard_EXPORT	TCollection_ExtendedString GetObjectName();

/**
 * \fn SetGeometryType(const Sketcher_ObjectGeometryTypeHandle(TCollection_ExtendedString)& theGeometryType)
 * \brief set type of geometry
 * \return void 
 * \param theGeometryType const Sketcher_ObjectGeometryType
 */
Standard_EXPORT	void SetGeometryType(const Sketcher_ObjectGeometryType theGeometryType);

/**
 * \fn GetGeometryType()
 * \brief get type of geometry
 * \return Sketcher_ObjectGeometryType
 */
Standard_EXPORT	Sketcher_ObjectGeometryType GetGeometryType();

/**
 * \fn SetTypeOfMethod(const Sketcher_ObjectTypeOfMethod theTypeOfMethod)
 * \brief set object entering method
 * \return void 
 * \param theGeometryType const Sketcher_ObjectTypeOfMethod
 */
Standard_EXPORT	void SetTypeOfMethod(const Sketcher_ObjectTypeOfMethod theTypeOfMethod);

/**
 * \fn GetTypeOfMethod()
 * \brief get object entering method
 * \return Sketcher_ObjectTypeOfMethod
 */
Standard_EXPORT	Sketcher_ObjectTypeOfMethod GetTypeOfMethod();

/**
 * \fn SetColor(const Quantity_NameOfColor theColor)
 * \brief set color
 * \return void 
 * \param theColor const Quantity_NameOfColor
 */
Standard_EXPORT	void SetColor(const Quantity_NameOfColor theColor);

/**
 * \fn GetColor()
 * \brief get color
 * \return Quantity_NameOfColor
 */
Standard_EXPORT	Quantity_NameOfColor GetColor();

/**
 * \fn SetType(const Sketcher_ObjectType theType)
 * \brief set type of object
 * \return void 
 * \param theType const Sketcher_ObjectType
 */
Standard_EXPORT	void SetType(const Sketcher_ObjectType theType);

/**
 * \fn GetType()
 * \brief get object type
 * \return Sketcher_ObjectType
 */
Standard_EXPORT	Sketcher_ObjectType GetType();

/**
 * \fn SetStyle(const Aspect_TypeOfLine theLineStyle)
 * \brief set line style
 * \return void 
 * \param theLineStyle const Aspect_TypeOfLine
 */
Standard_EXPORT	void SetStyle(const Aspect_TypeOfLine theLineStyle);

/**
 * \fn GetStyle()
 * \brief get line style
 * \return Aspect_TypeOfLine
 */
Standard_EXPORT	Aspect_TypeOfLine GetStyle();

/**
 * \fn SetWidth(const Standard_Real theWidth)
 * \brief set line width
 * \return void 
 * \param theWidth const Standard_Real
 */
Standard_EXPORT	void SetWidth(const Standard_Real theWidth);

/**
 * \fn GetWidth()
 * \brief get width
 * \return Standard_Real
 */
Standard_EXPORT	Standard_Real GetWidth();

private:

//members

Handle(Geom2d_Geometry)			myGeometry;
Handle(AIS_InteractiveObject)   myAIS_InteractiveObject;

TCollection_ExtendedString		myName;
Sketcher_ObjectGeometryType		myGeometryType;
Sketcher_ObjectTypeOfMethod		myTypeOfMethod;

Quantity_NameOfColor			myNameOfColor;
Sketcher_ObjectType				myObjectType;
Aspect_TypeOfLine				myLineStyle;
Standard_Real					myWidth;

};

#endif

