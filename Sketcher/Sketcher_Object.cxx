/**
* \file Sketcher_Object.cxx
* \brief Implementation file for the class Sketcher_Object
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_Object.cxx">Sergei Maslov</a>
*/
#include "Sketcher_Object.hxx"


/**
 * \fn Sketcher_Object(const Handle(Geom2d_Geometry)& theGeom2d_Geometry,const Handle(AIS_InteractiveObject)& theAIS_InteractiveObject, const TCollection_ExtendedString& theName, const Sketcher_ObjectGeometryType theGeometryType, const Sketcher_ObjectTypeOfMethod theTypeOfMethod)
 * \brief Constructs a Sketcher_Object
 * \param theGeom2d_Geometry const Handle(Geom2d_Geometry)&
 * \param theAIS_InteractiveObject const TCollection_ExtendedString&
 * \param theName const TCollection_ExtendedString&
 * \param theGeometryType const Sketcher_ObjectGeometryType
 * \param theTypeOfMethod const Sketcher_ObjectTypeOfMethod
 */
Sketcher_Object::Sketcher_Object(const Handle(Geom2d_Geometry)& theGeom2d_Geometry,
							    const Handle(AIS_InteractiveObject)& theAIS_InteractiveObject,
							    const TCollection_ExtendedString& theName,
							    const Sketcher_ObjectGeometryType theGeometryType,
							    const Sketcher_ObjectTypeOfMethod theTypeOfMethod)
{
    myGeometry = theGeom2d_Geometry;
    myAIS_InteractiveObject = theAIS_InteractiveObject;
	myName = theName;
	myGeometryType = theGeometryType;
	myTypeOfMethod = theTypeOfMethod;

	myNameOfColor = Quantity_NOC_YELLOW;
	myObjectType = MainSketcherType;
	myLineStyle = Aspect_TOL_SOLID;
	myWidth = 1.0;
}


/**
 * \fn ~Sketcher_Object() 
 * \brief destructor
 */
Sketcher_Object::~Sketcher_Object()
{
}

/**
 * \fn SetGeometry(const Handle(Geom2d_Geometry)& theGeom2d_Geometry)
 * \brief set Geom2d_Geometry
 * \return void 
 * \param theGeom2d_Geometry const Handle(Geom2d_Geometry)&
 */
void Sketcher_Object::SetGeometry(const Handle(Geom2d_Geometry)& theGeom2d_Geometry)
{
	myGeometry = theGeom2d_Geometry;
}

/**
 * \fn GetGeometry()
 * \brief get Geom2d_Geometry
 * \return Handle(Geom2d_Geometry)
 */
Handle(Geom2d_Geometry)	Sketcher_Object::GetGeometry()
{
	return myGeometry;
}

/**
 * \fn SetAIS_Object(const Handle(AIS_InteractiveObject)& theAIS_InteractiveObject)
 * \brief set object representation
 * \return void 
 * \param theAIS_InteractiveObject const Handle(AIS_InteractiveObject)&
 */
void Sketcher_Object::SetAIS_Object(const Handle(AIS_InteractiveObject)& theAIS_InteractiveObject)
{
	myAIS_InteractiveObject = theAIS_InteractiveObject;
}

/**
 * \fn GetAIS_Object()
 * \brief get object represent
 * \return Handle(AIS_InteractiveObject)
 */
Handle(AIS_InteractiveObject) Sketcher_Object::GetAIS_Object()
{
	return myAIS_InteractiveObject;
}


/**
 * \fn SetObjectName(const Handle(TCollection_ExtendedString)& theName)
 * \brief set object name
 * \return void 
 * \param theName const Handle(TCollection_ExtendedString)&
 */
void Sketcher_Object::SetObjectName(const TCollection_ExtendedString& theName)
{
	myName = theName;
}

/**
 * \fn GetObjectName()
 * \brief get object name
 * \return TCollection_ExtendedString
 */
TCollection_ExtendedString Sketcher_Object::GetObjectName()
{
	return myName;
}

/**
 * \fn SetGeometryType(const Sketcher_ObjectGeometryTypeHandle(TCollection_ExtendedString)& theGeometryType)
 * \brief set type of geometry
 * \return void 
 * \param theGeometryType const Sketcher_ObjectGeometryType
 */
void Sketcher_Object::SetGeometryType(const Sketcher_ObjectGeometryType theGeometryType)
{
    myGeometryType = theGeometryType;
}

/**
 * \fn GetGeometryType()
 * \brief get type of geometry
 * \return Sketcher_ObjectGeometryType
 */
Sketcher_ObjectGeometryType Sketcher_Object::GetGeometryType()
{
	return myGeometryType;
}

/**
 * \fn SetTypeOfMethod(const Sketcher_ObjectTypeOfMethod theTypeOfMethod)
 * \brief set object entering method
 * \return void 
 * \param theGeometryType const Sketcher_ObjectTypeOfMethod
 */
void Sketcher_Object::SetTypeOfMethod(const Sketcher_ObjectTypeOfMethod theTypeOfMethod)
{
	myTypeOfMethod = theTypeOfMethod;
}

/**
 * \fn GetTypeOfMethod()
 * \brief get object entering method
 * \return Sketcher_ObjectTypeOfMethod
 */
Sketcher_ObjectTypeOfMethod Sketcher_Object::GetTypeOfMethod()
{
	return myTypeOfMethod;
}

/**
 * \fn SetColor(const Quantity_NameOfColor theColor)
 * \brief set color
 * \return void 
 * \param theColor const Quantity_NameOfColor
 */
void Sketcher_Object::SetColor(const Quantity_NameOfColor theColor)
{
	myNameOfColor = theColor;
}

/**
 * \fn GetColor()
 * \brief get color
 * \return Quantity_NameOfColor
 */
Quantity_NameOfColor Sketcher_Object::GetColor()
{
	return myNameOfColor;
}

/**
 * \fn SetType(const Sketcher_ObjectType theType)
 * \brief set type of object
 * \return void 
 * \param theType const Sketcher_ObjectType
 */
void Sketcher_Object::SetType(const Sketcher_ObjectType theType)
{
	myObjectType = theType;
}

/**
 * \fn GetType()
 * \brief get object type
 * \return Sketcher_ObjectType
 */
Sketcher_ObjectType Sketcher_Object::GetType()
{
	return myObjectType;
}

/**
 * \fn SetStyle(const Aspect_TypeOfLine theLineStyle)
 * \brief set line style
 * \return void 
 * \param theLineStyle const Aspect_TypeOfLine
 */
void Sketcher_Object::SetStyle(const Aspect_TypeOfLine theLineStyle)
{
	myLineStyle = theLineStyle;
}

/**
 * \fn GetStyle()
 * \brief get line style
 * \return Aspect_TypeOfLine
 */
Aspect_TypeOfLine Sketcher_Object::GetStyle()
{
	return myLineStyle;
}

/**
 * \fn SetWidth(const Standard_Real theWidth)
 * \brief set line width
 * \return void 
 * \param theWidth const Standard_Real
 */
void Sketcher_Object::SetWidth(const Standard_Real theWidth)
{
	myWidth = theWidth;
}

/**
 * \fn GetWidth()
 * \brief get width
 * \return Standard_Real
 */
Standard_Real Sketcher_Object::GetWidth()
{
	return myWidth;
}


IMPLEMENT_STANDARD_HANDLE(Sketcher_Object,MMgt_TShared)
IMPLEMENT_STANDARD_RTTI(Sketcher_Object,MMgt_TShared)

IMPLEMENT_STANDARD_TYPE(Sketcher_Object)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_Object)

