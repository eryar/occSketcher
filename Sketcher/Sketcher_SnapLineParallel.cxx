/**
* \file Sketcher_SnapLineParallel.cxx
* \brief Implementation file for the class Sketcher_SnapLineParallel
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapLineParallel.cxx">Sergei Maslov</a>
*/
#include "Sketcher_SnapLineParallel.hxx"

/**
 * \fn Sketcher_SnapLineParallel() 
 * \brief Constructs a Sketcher_SnapLineParallel
 */
Sketcher_SnapLineParallel::Sketcher_SnapLineParallel()
{
	myParRelation = new AIS_ParallelRelation(FirstEdge,SecondEdge,myPlane);
}


/**
 * \fn ~Sketcher_SnapLineParallel() 
 * \brief Destructor
 */
Sketcher_SnapLineParallel::~Sketcher_SnapLineParallel()
{
}


/**
 * \fn SelectEvent() 
 * \brief construct line parallel to existing line
 * \return void 
 */
void Sketcher_SnapLineParallel::SelectEvent()
{
  SelectNearest();
  if(minDistance == minimumSnapDistance/2) 
  {
	nearestPnt2d = curPnt2d;
	curDir2d = gp_Dir2d (gp_Vec2d(curGeom2d_Point->Pnt2d(),curPnt2d));
	minAngle = MINANGLE;

	for( Standard_Integer i=1; i<=data->Length(); i++)
	{
		mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
		myGeometryType = mySObject->GetGeometryType();
		switch (myGeometryType)
		{
		case PointSketcherObject:	break;
		case LineSketcherObject:	curGeom2d_Line = Handle(Geom2d_Line)::DownCast(mySObject->GetGeometry());
									tempDir2d = curGeom2d_Line->Lin2d().Direction();
									checkparallel();
									break;
		case CircleSketcherObject:	break;
		case ArcSketcherObject: 	break;
		default:break;
		}
	}
	
	if(minAngle == MINANGLE) 
		 bestPnt2d = curPnt2d;
	else setEdges();

  }
  else 
  {
	ProjectPoint();
	setEdges();
  }
}


/**
 * \fn GetSnapType()
 * \brief get Snap type
 * \return Sketcher_SnapType 
 */
Sketcher_SnapType Sketcher_SnapLineParallel::GetSnapType()
{
	return SnapParallel;
}

/**
 * \fn DrawRelation()
 * \brief draw relation 
 * \return void 
 */
void Sketcher_SnapLineParallel::DrawRelation()
{
	myParRelation->SetFirstShape(FirstEdge);
	myParRelation->SetSecondShape(SecondEdge);
	if(firstDisplay) myContext->Display(myParRelation,0,-1);
	else myContext->Redisplay(myParRelation);
}

/**
 * \fn EraseRelation()
 * \brief erase relation 
 * \return void 
 */
void Sketcher_SnapLineParallel::EraseRelation()
{
	myContext->Remove(myParRelation);
}


IMPLEMENT_STANDARD_HANDLE(Sketcher_SnapLineParallel,Sketcher_SnapLine)
IMPLEMENT_STANDARD_RTTI(Sketcher_SnapLineParallel)

IMPLEMENT_STANDARD_TYPE(Sketcher_SnapLineParallel)
IMPLEMENT_STANDARD_SUPERTYPE(Sketcher_SnapLine)
IMPLEMENT_STANDARD_SUPERTYPE(Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Sketcher_SnapLine)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_SnapLineParallel)
