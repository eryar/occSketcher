/**
* \file Sketcher_Command.cxx
* \brief Implementation file for the class Sketcher_Command
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_Command.cxx">Sergei Maslov</a>
*/
#include "Sketcher_Command.hxx"

/**
 * \fn Sketcher_Command(const Standard_CString name) 
 * \brief Constructs a Sketcher_Command
 * \param name const Standard_CString
 */
Sketcher_Command::Sketcher_Command(const Standard_CString name)
: ObjectName(name),
  curCoordinateSystem(gp::XOY())
{
	ObjectCounter = 0;
	myColor = Quantity_NOC_YELLOW;
	myType = MainSketcherType;
	myStyle = Aspect_TOL_SOLID;
	myWidth = 1.0;
	myPrs3dAspect = new Prs3d_LineAspect(myColor,myStyle,myWidth);
	//myDrawer = new AIS_Drawer();


	myPolylineMode= Standard_False;
	curPnt2d = gp::Origin2d();
	myFirstgp_Pnt2d = gp::Origin2d();

	myFirstPoint = new Geom_CartesianPoint(gp::Origin());
	mySecondPoint = new Geom_CartesianPoint(gp::Origin());
	myRubberLine = new AIS_Line(myFirstPoint,mySecondPoint);
	myRubberLine->SetColor(Quantity_NOC_LIGHTPINK1);
}

/**
 * \fn ~Sketcher_Command() 
 * \brief destructor
 */

Sketcher_Command::~Sketcher_Command()
{
}

/**
 * \fn SetContext(Handle(AIS_InteractiveContext)& theContext)
 * \brief set context
 * \return void 
 * \param theContext Handle(AIS_InteractiveContext)&
 */
void Sketcher_Command::SetContext (Handle(AIS_InteractiveContext)& theContext)
{
	myContext = theContext;
}

/**
 * \fn SetData(Handle(TColStd_HSequenceOfTransient)& thedata)
 * \brief set list of objects
 * \return void 
 * \param thedata TColStd_HSequenceOfTransient)&
 */
void Sketcher_Command::SetData (Handle(TColStd_HSequenceOfTransient)& thedata) 
{
	data = thedata;
}

/**
 * \fn SetAx3(const gp_Ax3& theAx3)
 * \brief set coordinate system
 * \return void 
 * \param theAx3 const gp_Ax3&
 */
void Sketcher_Command::SetAx3(const gp_Ax3 &theAx3)
{
	curCoordinateSystem = theAx3;
}

/**
 * \fn SetAnalyserSnap(Handle(Sketcher_AnalyserSnap)& theAnalyserSnap)
 * \brief set snap manager
 * \return void 
 * \param theAnalyserSnap Handle(Sketcher_AnalyserSnap)&
 */
void Sketcher_Command::SetAnalyserSnap(Handle(Sketcher_AnalyserSnap)& theAnalyserSnap)
{
	myAnalyserSnap = theAnalyserSnap;
}

/**
 * \fn SetColor(const Quantity_NameOfColor theColor)
 * \brief set color
 * \return void 
 * \param theColor const Quantity_NameOfColor
 */
void Sketcher_Command::SetColor(const Quantity_NameOfColor theColor)
{
	myColor = theColor;
}

/**
 * \fn SetType(const Sketcher_ObjectType theType)
 * \brief set type of object
 * \return void 
 * \param theType const Sketcher_ObjectType
 */
void Sketcher_Command::SetType(const Sketcher_ObjectType theType)
{
	myType = theType;
}

/**
 * \fn SetStyle(const Aspect_TypeOfLine theLineStyle)
 * \brief set line style
 * \return void 
 * \param theLineStyle const Aspect_TypeOfLine
 */
void Sketcher_Command::SetStyle(const Aspect_TypeOfLine theLineStyle)
{
	myStyle = theLineStyle;
}

/**
 * \fn SetWidth(const Standard_Real theWidth)
 * \brief set line width
 * \return void 
 * \param theWidth const Standard_Real
 */
void Sketcher_Command::SetWidth(const Standard_Real theWidth)
{
	myWidth = theWidth;
}

/**
 * \fn AddObject(const Handle(Geom2d_Geometry)& theGeom2d_Geometry,const Handle(AIS_InteractiveObject)& theAIS_InteractiveObject,const Sketcher_ObjectGeometryType theGeometryType)
 * \brief add new object to object list
 * \return void 
 * \param theGeom2d_Geometry const Handle(Geom2d_Geometry)&
 * \param theAIS_InteractiveObject const Handle(AIS_InteractiveObject)&
 * \param theGeometryType const Sketcher_ObjectGeometryType
 */
void Sketcher_Command::AddObject(const Handle(Geom2d_Geometry)& theGeom2d_Geometry,
								 const Handle(AIS_InteractiveObject)& theAIS_InteractiveObject,
								 const Sketcher_ObjectGeometryType theGeometryType)
{
	ObjectCounter++;
	TCollection_ExtendedString numString(ObjectCounter);
	TCollection_ExtendedString currentName(ObjectName);
	currentName += numString;

	if(GetTypeOfMethod() == Point_Method)
		theAIS_InteractiveObject->SetColor(myColor);
	else
	{
		myPrs3dAspect->SetColor(myColor);
		myPrs3dAspect->SetTypeOfLine(myStyle);
		myPrs3dAspect->SetWidth(myWidth);

		//myDrawer->SetLineAspect(myPrs3dAspect);
		//theAIS_InteractiveObject->SetAttributes(myDrawer);
	}

	Handle(Sketcher_Object) so = new Sketcher_Object(theGeom2d_Geometry,theAIS_InteractiveObject,currentName,theGeometryType,GetTypeOfMethod());
	so->SetColor(myColor);
	so->SetType(myType);
	so->SetStyle(myStyle);
	so->SetWidth(myWidth);
	data->Append(so);
}


/**
 * \fn SetPolylineFirstPnt(const gp_Pnt2d& p1)
 * \brief set last entering gp_Pnt2d for polyline
 * \return void 
 * \param p1 const gp_Pnt2d&
 */
void  Sketcher_Command::SetPolylineFirstPnt(const gp_Pnt2d& p1)
{
}

/**
 * \fn GetPolylineFirstPnt(gp_Pnt2d& p1)
 * \brief get last entering gp_Pnt2d for polyline
 * \return Standard_Boolean 
 * \param p1 gp_Pnt2d&
 */
Standard_Boolean Sketcher_Command::GetPolylineFirstPnt(gp_Pnt2d& p1)
{
	return Standard_False;
}

/**
 * \fn SetPolylineMode(Standard_Boolean mode)
 * \brief set polyline mode
 * \return void 
 * \param mode Standard_Boolean 
 */
void Sketcher_Command::SetPolylineMode (Standard_Boolean mode) 
{
}


IMPLEMENT_STANDARD_HANDLE(Sketcher_Command,MMgt_TShared)
IMPLEMENT_STANDARD_RTTI(Sketcher_Command)

IMPLEMENT_STANDARD_TYPE(Sketcher_Command)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_Command)

