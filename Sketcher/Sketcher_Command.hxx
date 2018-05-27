/**
* \file Sketcher_Command.hxx
* \brief Header file for the class Sketcher_Command
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_Command.hxx">Sergei Maslov</a>
*/
#ifndef SKETCHER_COMMAND_H
#define SKETCHER_COMMAND_H

#include <Standard_Macro.hxx>
#include <MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>

#include "Sketcher_Object.hxx"
#include "Sketcher_SnapType.hxx"
#include "Sketcher_AnalyserSnap.hxx"

#include <AIS_InteractiveContext.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Ax3.hxx>
#include <Geom_CartesianPoint.hxx>
#include <AIS_Line.hxx>

//#include <AIS_Drawer.hxx>
#include <Prs3d_LineAspect.hxx>

#define SKETCHER_RADIUS 0


class AIS_InteractiveContext;
class gp_Pnt2d;

DEFINE_STANDARD_HANDLE(Sketcher_Command,MMgt_TShared)

//base class of commands entering objects
class Sketcher_Command : public MMgt_TShared 
{
public:
// Type management
DEFINE_STANDARD_RTTI(Sketcher_Command)

/**
 * \fn Sketcher_Command(const Standard_CString name) 
 * \brief Constructs a Sketcher_Command
 * \param name const Standard_CString
 */
Standard_EXPORT	Sketcher_Command(const Standard_CString name);

/**
 * \fn ~Sketcher_Command() 
 * \brief destructor
 */
Standard_EXPORT	~Sketcher_Command();

/**
 * \fn SetContext(Handle(AIS_InteractiveContext)& theContext)
 * \brief set context
 * \return void 
 * \param theContext Handle(AIS_InteractiveContext)&
 */
Standard_EXPORT	void SetContext(Handle(AIS_InteractiveContext)& theContext );

/**
 * \fn SetData(Handle(TColStd_HSequenceOfTransient)& thedata)
 * \brief set list of objects
 * \return void 
 * \param thedata TColStd_HSequenceOfTransient)&
 */
Standard_EXPORT	void SetData (Handle(TColStd_HSequenceOfTransient)& thedata); 

/**
 * \fn SetAx3(const gp_Ax3& theAx3)
 * \brief set coordinate system
 * \return void 
 * \param theAx3 const gp_Ax3&
 */
Standard_EXPORT	void SetAx3(const gp_Ax3 & theAx3);

/**
 * \fn SetAnalyserSnap(Handle(Sketcher_AnalyserSnap)& theAnalyserSnap)
 * \brief set snap manager
 * \return void 
 * \param theAnalyserSnap Handle(Sketcher_AnalyserSnap)&
 */
Standard_EXPORT	void SetAnalyserSnap(Handle(Sketcher_AnalyserSnap)& theAnalyserSnap);

/**
 * \fn SetColor(const Quantity_NameOfColor theColor)
 * \brief set color
 * \return void 
 * \param theColor const Quantity_NameOfColor
 */
Standard_EXPORT	void SetColor(const Quantity_NameOfColor theColor);

/**
 * \fn SetType(const Sketcher_ObjectType theType)
 * \brief set type of object
 * \return void 
 * \param theType const Sketcher_ObjectType
 */
Standard_EXPORT	void SetType(const Sketcher_ObjectType theType);

/**
 * \fn SetStyle(const Aspect_TypeOfLine theLineStyle)
 * \brief set line style
 * \return void 
 * \param theLineStyle const Aspect_TypeOfLine
 */
Standard_EXPORT	void SetStyle(const Aspect_TypeOfLine theLineStyle);

/**
 * \fn SetWidth(const Standard_Real theWidth)
 * \brief set line width
 * \return void 
 * \param theWidth const Standard_Real
 */
Standard_EXPORT	void SetWidth(const Standard_Real theWidth);

/**
 * \fn AddObject(const Handle(Geom2d_Geometry)& theGeom2d_Geometry,const Handle(AIS_InteractiveObject)& theAIS_InteractiveObject,const Sketcher_ObjectGeometryType theGeometryType)
 * \brief add new object to object list
 * \return void 
 * \param theGeom2d_Geometry const Handle(Geom2d_Geometry)&
 * \param theAIS_InteractiveObject const Handle(AIS_InteractiveObject)&
 * \param theGeometryType const Sketcher_ObjectGeometryType
 */
Standard_EXPORT void AddObject(const Handle(Geom2d_Geometry)& theGeom2d_Geometry,const Handle(AIS_InteractiveObject)& theAIS_InteractiveObject,const Sketcher_ObjectGeometryType theGeometryType);

/**
 * \fn Action() 
 * \brief turn command to active state 
 */
Standard_EXPORT	virtual void Action() = 0;

/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d )
 * \brief input event handler
 * \return Standard_Boolean 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_EXPORT	virtual	Standard_Boolean MouseInputEvent(const gp_Pnt2d& thePnt2d) = 0;

/**
 * \fn MouseMoveEvent(const gp_Pnt2d& thePnt2d )
 * \brief mouse move handler
 * \return void 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_EXPORT	virtual	void MouseMoveEvent(const gp_Pnt2d& thePnt2d) = 0;

/**
 * \fn CancelEvent()
 * \brief cancel event handler, stop entering object
 * \return void 
 */
Standard_EXPORT	virtual	void CancelEvent() = 0;

/**
 * \fn GetTypeOfMethod()
 * \brief get command Method
 * \return Sketcher_ObjectTypeOfMethod 
 */
Standard_EXPORT	virtual Sketcher_ObjectTypeOfMethod GetTypeOfMethod() = 0;

/**
 * \fn SetPolylineFirstPnt(const gp_Pnt2d& p1)
 * \brief set last entering gp_Pnt2d for polyline
 * \return void 
 * \param p1 const gp_Pnt2d&
 */
Standard_EXPORT	virtual void SetPolylineFirstPnt(const gp_Pnt2d& p1);

/**
 * \fn GetPolylineFirstPnt(gp_Pnt2d& p1)
 * \brief get last entering gp_Pnt2d for polyline
 * \return Standard_Boolean 
 * \param p1 gp_Pnt2d&
 */
Standard_EXPORT	virtual Standard_Boolean GetPolylineFirstPnt(gp_Pnt2d& p1);

/**
 * \fn SetPolylineMode(Standard_Boolean mode)
 * \brief set polyline mode
 * \return void 
 * \param mode Standard_Boolean 
 */
Standard_EXPORT	virtual void SetPolylineMode(Standard_Boolean mode);


protected:

Handle(AIS_InteractiveContext)			myContext;
Handle(TColStd_HSequenceOfTransient)	data;
Handle(Sketcher_AnalyserSnap)			myAnalyserSnap;

Quantity_NameOfColor					myColor;
Sketcher_ObjectType						myType;
Aspect_TypeOfLine						myStyle;
Standard_Real							myWidth;
//Handle(AIS_Drawer)						myDrawer;
Handle(Prs3d_LineAspect)				myPrs3dAspect;

Standard_Boolean						myPolylineMode;

gp_Pnt2d								curPnt2d;
gp_Ax3									curCoordinateSystem;
gp_Pnt2d								myFirstgp_Pnt2d;
Handle(Geom_CartesianPoint)				myFirstPoint;
Handle(Geom_CartesianPoint)				mySecondPoint;
Handle(AIS_Line)						myRubberLine;
TCollection_ExtendedString				ObjectName;
Standard_Integer						ObjectCounter;
};

#endif
