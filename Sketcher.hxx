/**
* \file Sketcher.hxx
* \brief Header file for the class Sketcher
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher.hxx">Sergei Maslov</a>
*/
#ifndef SKETCHER_H
#define SKETCHER_H

#include <AIS_InteractiveContext.hxx>
#include <TColStd_HSequenceOfTransient.hxx>

#include <gp_Pnt2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax3.hxx>
#include <GeomAPI_IntCS.hxx>
#include <Geom_Line.hxx>
#include <Geom_Plane.hxx>
#include <Geom2d_CartesianPoint.hxx>
#include <V3d_Coordinate.hxx>

#include "Sketcher_SnapType.hxx"
#include "Sketcher_AnalyserSnap.hxx"

#include "Sketcher_GUI.hxx"

#include "Sketcher_Object.hxx"
#include "Sketcher_CommandPoint.hxx"
#include "Sketcher_CommandLine2P.hxx"
#include "Sketcher_CommandCircleCenterRadius.hxx"
#include "Sketcher_CommandCircle3P.hxx"
#include "Sketcher_CommandCircle2PTan.hxx"
#include "Sketcher_CommandCircleP2Tan.hxx"
#include "Sketcher_CommandCircle3Tan.hxx"
#include "Sketcher_CommandArc3P.hxx"
#include "Sketcher_CommandArcCenter2P.hxx"
#include "Sketcher_CommandBezierCurve.hxx"
#include "Sketcher_CommandTrim.hxx"



class AIS_InteractiveContext;
class AIS_Line;
class Geom_Plane;

//main class, witch process program commands and manage other Sketcher classes
class Sketcher
{

public:

/**
 * \fn Sketcher(Handle(AIS_InteractiveContext)& theContext, Sketcher_GUI* sg)
 * \brief Constructs a Sketcher
 * \param theContext Handle(AIS_InteractiveContext)&
 * \param sg Sketcher_GUI* 
 */
Sketcher(Handle(AIS_InteractiveContext)& theContext,Sketcher_GUI* sg);
/**
 * \fn ~Sketcher()
 * \brief destructor
 */
~Sketcher();

/**
 * \fn SetContext(Handle(AIS_InteractiveContext)& theContext)
 * \brief set context
 * \return void 
 * \param theContext Handle(AIS_InteractiveContext)&
 */
void SetContext (Handle(AIS_InteractiveContext)& theContext);

/**
 * \fn SetData(Handle(TColStd_HSequenceOfTransient)& thedata)
 * \brief set list of objects  
 * \return void 
 * \param thedata TColStd_HSequenceOfTransient)&
 */
void SetData (Handle(TColStd_HSequenceOfTransient)& thedata);

/**
 * \fn GetData()
 * \brief get list of objects
 * \return Handle(TColStd_HSequenceOfTransient)&
 */
Handle(TColStd_HSequenceOfTransient) GetData ();

/**
 * \fn SetCoordinateSystem(const gp_Ax3& theCS)
 * \brief set coordinate system
 * \return void 
 * \param theCS const gp_Ax3&
 */
void SetCoordinateSystem (const gp_Ax3& theCS);	
	
/**
 * \fn GetCoordinateSystem()
 * \brief get coordinate system from Sketcher 
 * \return gp_Ax3 
 */
gp_Ax3 GetCoordinateSystem();
	
/**
 * \fn SetPrecise(const Standard_Real& aPrecise)
 * \brief set precise for snap
 * \return void 
 * \param aPrecise const Standard_Real&
 */
void SetPrecise(const Standard_Real& aPrecise);

/**
 * \fn SetColor(const Quantity_NameOfColor theColor)
 * \brief set color
 * \return void 
 * \param theColor const Quantity_NameOfColor
 */
void SetColor(const Quantity_NameOfColor theColor);

/**
 * \fn SetType(const Sketcher_ObjectType theType)
 * \brief set type of object
 * \return void 
 * \param theType const Sketcher_ObjectType
 */
void SetType(const Sketcher_ObjectType theType);

/**
 * \fn SetStyle(const Aspect_TypeOfLine theLineStyle)
 * \brief set line style
 * \return void 
 * \param theLineStyle const Aspect_TypeOfLine
 */
void SetStyle(const Aspect_TypeOfLine theLineStyle);

/**
 * \fn SetWidth(const Standard_Real& theWidth)
 * \brief set line width
 * \return void 
 * \param theWidth const Standard_Real&
 */
void SetWidth(const Standard_Real& theWidth);

/**
 * \fn ObjectAction(const Sketcher_ObjectTypeOfMethod theMethod)
 * \brief set entering object command by theMethod
 * \return void 
 * \param theMethod const Sketcher_ObjectTypeOfMethod
 */
void ObjectAction(const Sketcher_ObjectTypeOfMethod theMethod);

/**
 * \fn GetStatus()
 * \brief get current object create method
 * \return Sketcher_ObjectTypeOfMethod 
 */
 Sketcher_ObjectTypeOfMethod GetStatus();
	
/**
 * \fn OnMouseInputEvent(const V3d_Coordinate &v3dX,const V3d_Coordinate &v3dY,const V3d_Coordinate &v3dZ,const Standard_Real& projVx,const Standard_Real& projVy,const Standard_Real& projVz)
 * \brief input event handler
 * \return void 
 * \param v3dX const V3d_Coordinate&
 * \param v3dY const V3d_Coordinate&
 * \param v3dZ const V3d_Coordinate&
 * \param projVx const Standard_Real&
 * \param projVy const Standard_Real&
 * \param projVz const Standard_Real&
*/
void OnMouseInputEvent(const V3d_Coordinate &v3dX,const V3d_Coordinate &v3dY,const V3d_Coordinate &v3dZ,const Standard_Real& projVx,const Standard_Real& projVy,const Standard_Real& projVz);

/**
 * \fn OnMouseMoveEvent(const V3d_Coordinate &v3dX,const V3d_Coordinate &v3dY,const V3d_Coordinate &v3dZ,const Standard_Real& projVx,const Standard_Real& projVy,const Standard_Real& projVz)
 * \brief mouse move handler
 * \return void 
 * \param v3dX const V3d_Coordinate&
 * \param v3dY const V3d_Coordinate&
 * \param v3dZ const V3d_Coordinate&
 * \param projVx const Standard_Real&
 * \param projVy const Standard_Real&
 * \param projVz const Standard_Real&
*/
void OnMouseMoveEvent(const V3d_Coordinate &v3dX,const V3d_Coordinate &v3dY,const V3d_Coordinate &v3dZ,const Standard_Real& projVx,const Standard_Real& projVy,const Standard_Real& projVz);

/**
 * \fn OnCancel()
 * \brief cancel event handler, stop entering object
 * \return void 
 */
void OnCancel();
			
/**
 * \fn DeleteSelectedObject()
 * \brief delete objects selected in the context
 * \return void 
 */
void DeleteSelectedObject();

/**
 * \fn ViewProperties()
 * \brief show properties for selected object
 * \return void 
 */
void ViewProperties();

/**
 * \fn RedrawAll()
 * \brief erase and redraw all objects from object list
 * \return void 
 */
void RedrawAll();

/**
 * \fn SetPolylineMode(Standard_Boolean mode)
 * \brief set polyline mode 
 * \return void 
 * \param amode Standard_Boolean
 */
void SetPolylineMode(Standard_Boolean amode);

/**
 * \fn SetSnap(Sketcher_SnapType theSnap)
 * \brief set current snap type
 * \return void 
 * \param theSnap Sketcher_SnapType
 */
void SetSnap(Sketcher_SnapType theSnap);

/**
 * \fn GetSnap()
 * \brief get current snap type
 * \return Sketcher_SnapType 
 */
Sketcher_SnapType GetSnap();		

private:
/**
 * \fn ProjectPointOnPlane(const V3d_Coordinate &v3dX,const V3d_Coordinate &v3dY,const V3d_Coordinate &v3dZ,const Standard_Real& projVx,const Standard_Real& projVy,const Standard_Real& projVz)
 * \brief calculate 2d point on current plane
 * \return Standard_Boolean 
 * \param v3dX const V3d_Coordinate&
 * \param v3dY const V3d_Coordinate&
 * \param v3dZ const V3d_Coordinate&
 * \param projVx const Standard_Real&
 * \param projVy const Standard_Real&
 * \param projVz const Standard_Real&
*/
Standard_Boolean ProjectPointOnPlane(const V3d_Coordinate &v3dX,const V3d_Coordinate &v3dY,const V3d_Coordinate &v3dZ,const Standard_Real& projVx,const Standard_Real& projVy,const Standard_Real& projVz);

/**
 * \fn addCommand(Handle(Sketcher_Command) theCommand)
 * \brief add theCommand to command list
 * \return void 
 * \param theCommand Handle(Sketcher_Command)
 */
void addCommand(Handle(Sketcher_Command) theCommand);

/**
 * \fn SelectCurCommand()
 * \brief get command from command list
 * \return void 
 */
void SelectCurCommand();

private:
Handle(AIS_InteractiveContext)			myContext;
Handle(TColStd_HSequenceOfTransient)	myData;
Sketcher_GUI*							myGUI;

Handle(TColStd_HSequenceOfTransient)	myCommands;
Handle(Sketcher_Command)				CurCommand;
Sketcher_ObjectTypeOfMethod				myCurrentMethod;
	
Handle(Sketcher_AnalyserSnap)			myAnalyserSnap;
Handle(Sketcher_Object)					myCurObject;

Handle(Geom_Line)	myCurrentLine;
Handle(Geom_Plane)	myCurrentPlane;
gp_Ax3				myCoordinateSystem;
gp_Dir				myCurrentDir;
gp_Pnt				myTempPnt;
gp_Pnt2d			myCurrentPnt2d;
GeomAPI_IntCS		myIntCS;

gp_Pnt2d			PolylineFirstPoint;
Standard_Boolean	PolylineFirstPointExist;
};

#endif