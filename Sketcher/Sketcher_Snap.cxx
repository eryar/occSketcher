/**
* \file Sketcher_Snap.cxx
* \brief Implementation file for the class Sketcher_Snap
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_Snap.cxx">Sergei Maslov</a>
*/
#include "Sketcher_Snap.hxx"

/**
 * \fn Sketcher_Snap() 
 * \brief Constructs a Sketcher_Snap
 */
Sketcher_Snap::Sketcher_Snap()
:   curCoordinateSystem(gp::XOY()),
	FirstEdge(),SecondEdge()
{
	curPnt2d = gp::Origin2d();
	objectPnt2d = gp::Origin2d();
	bestPnt2d = gp::Origin2d();
	findbestPnt2d = Standard_False;

	firstDisplay  = Standard_True;
	myGeom_Point = new Geom_CartesianPoint(gp::Origin());
	myAIS_Point = new AIS_Point(myGeom_Point);
	myAIS_Point->SetColor(Quantity_NOC_CYAN1);

	minimumSnapDistance = MINIMUMSNAP;
	minDistance = 0;
	curDistance = 0;
	curGeom2d_Point = new Geom2d_CartesianPoint(curPnt2d);
	myPlane = new Geom_Plane(curCoordinateSystem);
}

/**
 * \fn ~Sketcher_Snap() 
 * \brief Destructor
 */
Sketcher_Snap::~Sketcher_Snap()
{
}

/**
 * \fn SetContext(Handle(AIS_InteractiveContext)& theContext)
 * \brief set context
 * \return void 
 * \param theContext Handle(AIS_InteractiveContext)&
 */
void Sketcher_Snap::SetContext (Handle(AIS_InteractiveContext)& theContext)
{
	myContext = theContext;
}

/**
 * \fn SetData(Handle(TColStd_HSequenceOfTransient)& thedata)
 * \brief set list of objects
 * \return void 
 * \param thedata TColStd_HSequenceOfTransient)&
 */
void Sketcher_Snap::SetData (Handle(TColStd_HSequenceOfTransient)& thedata) 
{
	data = thedata;
}

/**
 * \fn SetAx3(const gp_Ax3& theAx3)
 * \brief set coordinate system
 * \return void 
 * \param theAx3 const gp_Ax3&
 */
void Sketcher_Snap::SetAx3(const gp_Ax3 &theAx3)
{
	curCoordinateSystem = theAx3;
}

/**
 * \fn SetMinDistance(const Standard_Real& aPrecise)
 * \brief set precise 
 * \return void 
 * \param aPrecise const Standard_Real&
 */
void Sketcher_Snap::SetMinDistance(const Standard_Real& aPrecise)
{
	minimumSnapDistance = aPrecise;
}

/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d)
 * \brief input event handler
 * \return gp_Pnt2d 
 * \param thePnt2d const gp_Pnt2d&
 */
gp_Pnt2d Sketcher_Snap::MouseInputEvent(const gp_Pnt2d& tempPnt2d)
{
	curPnt2d = tempPnt2d;
	SelectEvent();
	EraseSnap();
	return bestPnt2d;
}

/**
 * \fn MouseMoveEvent(const gp_Pnt2d& thePnt2d)
 * \brief mouse move handler
 * \return gp_Pnt2d 
 * \param thePnt2d const gp_Pnt2d&
 */
gp_Pnt2d Sketcher_Snap::MouseMoveEvent(const gp_Pnt2d& tempPnt2d)
{
  curPnt2d = tempPnt2d;
  SelectEvent();
  if(findbestPnt2d)
  {
	myGeom_Point->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),bestPnt2d));
	myAIS_Point->SetComponent(myGeom_Point);
	if (firstDisplay)	
	{
		myContext->Display(myAIS_Point,0,-1);
		DrawRelation();
		firstDisplay = Standard_False;
	}
	else 
	{
		myContext->Redisplay(myAIS_Point);
 		DrawRelation();
	}
  }
  else
  {
	myContext->Remove(myAIS_Point);
	EraseRelation();
	firstDisplay = Standard_True;
  }

  return bestPnt2d;
}

/**
 * \fn EraseSnap()
 * \brief cancel event handler
 * \return void 
 */
void Sketcher_Snap::EraseSnap()
{
	firstDisplay = Standard_True;
	myContext->Remove(myAIS_Point);
	EraseRelation();
}

/**
 * \fn AnalyserEvent(const gp_Pnt2d& tempPnt2d, gp_Pnt2d& newPnt2d,Standard_Real& dist,Standard_Integer& type)
 * \brief get new point, minimum distance, snap type
 * \return Standard_Boolean 
 * \param tempPnt2d const gp_Pnt2d&
 * \param newPnt2d const gp_Pnt2d&
 * \param dist Standard_Real& 
 * \param type Standard_Integer& 
*/
Standard_Boolean Sketcher_Snap::AnalyserEvent(const gp_Pnt2d& tempPnt2d, gp_Pnt2d& newPnt2d,Standard_Real& dist,Standard_Integer& type)
{
  curPnt2d = tempPnt2d;
  SelectEvent();
  newPnt2d = bestPnt2d;
  dist = minDistance;
  type = (Standard_Integer)GetSnapType();
  return findbestPnt2d;
}

/**
 * \fn DrawRelation()
 * \brief draw relation 
 * \return void 
 */
void Sketcher_Snap::DrawRelation()
{
	myContext->SetSelected(curHilightedObj);
}

/**
 * \fn EraseRelation()
 * \brief erase relation 
 * \return void 
 */
void Sketcher_Snap::EraseRelation()
{
	myContext->ClearSelected();
}

/**
 * \fn setFirstPnt(const gp_Pnt2d& p)
 * \brief set point for exceptions
 * \return void 
 * \param p const gp_Pnt2d&
*/
void Sketcher_Snap::setFirstPnt(const gp_Pnt2d& p)
{
}

/**
 * \fn setFirstPnt(const gp_Pnt2d& p)
 * \brief set point and tangent type for exceptions
 * \return void 
 * \param p const gp_Pnt2d&
 * \param ttype TangentType
*/
void Sketcher_Snap::setFirstPnt(const gp_Pnt2d& p,TangentType ttype)
{
}

/**
 * \fn countProject()
 * \brief find point projected on curve
 * \return Standard_Boolean 
 */
Standard_Boolean Sketcher_Snap::countProject()
{
	if(ProjectOnCurve.NbPoints() > 0)
	{
		objectPnt2d = ProjectOnCurve.NearestPoint();
		return count();
	}
	else return Standard_False;
}

/**
 * \fn count()
 * \brief check distance to point
 * \return Standard_Boolean 
 */
Standard_Boolean Sketcher_Snap::count()
{
	curDistance = objectPnt2d.Distance(curPnt2d);
	if(minDistance > curDistance)
	{
		minDistance = curDistance;
		return Standard_True;
	}
	else return Standard_False;
}


IMPLEMENT_STANDARD_HANDLE(Sketcher_Snap,MMgt_TShared)
IMPLEMENT_STANDARD_RTTI(Sketcher_Snap)

IMPLEMENT_STANDARD_TYPE(Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_Snap)

