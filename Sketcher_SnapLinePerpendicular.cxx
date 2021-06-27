/**
* \file Sketcher_SnapLinePerpendicular.cxx
* \brief Implementation file for the class Sketcher_SnapLinePerpendicular
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapLinePerpendicular.cxx">Sergei Maslov</a>
*/
#include "Sketcher_SnapLinePerpendicular.hxx"

IMPLEMENT_STANDARD_RTTIEXT(Sketcher_SnapLinePerpendicular, Sketcher_SnapLine)

/**
 * \fn Sketcher_SnapLinePerpendicular() 
 * \brief Constructs a Sketcher_SnapLinePerpendicular
 */
Sketcher_SnapLinePerpendicular::Sketcher_SnapLinePerpendicular()
{
	myPerRelation = new AIS_PerpendicularRelation(FirstEdge,SecondEdge,myPlane);
}


/**
 * \fn ~Sketcher_SnapLinePerpendicular() 
 * \brief Destructor
 */
Sketcher_SnapLinePerpendicular::~Sketcher_SnapLinePerpendicular()
{
}

/**
 * \fn SelectEvent() 
 * \brief construct line perpendicular to existing line
 * \return void 
 */
void Sketcher_SnapLinePerpendicular::SelectEvent()
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
		case PointSketcherObject:	
				break;
		case LineSketcherObject:	
				curGeom2d_Line = Handle(Geom2d_Line)::DownCast(mySObject->GetGeometry());
				ProjectOnCurve.Init(curPnt2d,curGeom2d_Line);
				if(ProjectOnCurve.NbPoints() > 0)
				{
					objectPnt2d = ProjectOnCurve.NearestPoint();
					gce_MakeDir2d MD2d(curPnt2d,objectPnt2d);
					tempDir2d = MD2d.Value();
					checkparallel();
				}
				break;
		case CircleSketcherObject:	
				break;
		case ArcSketcherObject: 	
				break;
		default:break;	
		}
	}

	if(minAngle == MINANGLE) 
		 bestPnt2d = curPnt2d;
	else  setEdges();
  }
  else 
  {
	gce_MakeDir2d MD2d(curPnt2d,nearestPnt2d);
	tempDir2d = MD2d.Value();
	ProjectPoint();
	setEdges();
  }

  minDistance = bestPnt2d.Distance(curPnt2d);
}


/**
 * \fn GetSnapType()
 * \brief get Snap type
 * \return Sketcher_SnapType 
 */
Sketcher_SnapType Sketcher_SnapLinePerpendicular::GetSnapType()
{
	return SnapPerpendicular;
}

/**
 * \fn DrawRelation()
 * \brief draw relation 
 * \return void 
 */
void Sketcher_SnapLinePerpendicular::DrawRelation()
{
	myPerRelation->SetFirstShape(FirstEdge);
	myPerRelation->SetSecondShape(SecondEdge);
	if(firstDisplay) myContext->Display(myPerRelation,0,-1, true);
	else myContext->Redisplay(myPerRelation, true);
}

/**
 * \fn EraseRelation()
 * \brief erase relation 
 * \return void 
 */
void Sketcher_SnapLinePerpendicular::EraseRelation()
{
	myContext->Remove(myPerRelation, true);
}

