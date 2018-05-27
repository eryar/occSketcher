/**
* \file Sketcher_SnapIntersection.cxx
* \brief Implementation file for the class Sketcher_SnapIntersection
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapIntersection.cxx">Sergei Maslov</a>
*/
#include "Sketcher_SnapIntersection.hxx"

/**
 * \fn Sketcher_SnapIntersection() 
 * \brief Constructs a Sketcher_SnapIntersection
 */
Sketcher_SnapIntersection::Sketcher_SnapIntersection()
{
	cur1Geom2d_Line = new Geom2d_Line(gp::Origin2d(),gp::DX2d());
	cur2Geom2d_Line = new Geom2d_Line(gp::Origin2d(),gp::DX2d());
	bpu = 0;
}


/**
 * \fn ~Sketcher_SnapIntersection() 
 * \brief Destructor
 */
Sketcher_SnapIntersection::~Sketcher_SnapIntersection()
{
}

/**
 * \fn SelectEvent() 
 * \brief find nearest intersection point
 * \return void 
 */
void Sketcher_SnapIntersection::SelectEvent()
{
  findbestPnt2d = Standard_False;
  minDistance = minimumSnapDistance;
  for( Standard_Integer i=1; i<=data->Length(); i++)
  {
	mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
	myGeometryType = mySObject->GetGeometryType();
	switch (myGeometryType)
	{
	case PointSketcherObject:	
			curGeom2d_Point = Handle(Geom2d_CartesianPoint)::DownCast(mySObject->GetGeometry());
			objectPnt2d = curGeom2d_Point->Pnt2d();
			if(count())
			{
				bestPnt2d = objectPnt2d;
				curHilightedObj = mySObject->GetAIS_Object();
				curHilightedObj2 = curHilightedObj;
			}
			break;
	case LineSketcherObject:	
			cur1Geom2d_Edge = Handle(Geom2d_Edge)::DownCast(mySObject->GetGeometry());
			cur1Geom2d_Line->SetLin2d(cur1Geom2d_Edge->Lin2d());
			cur1Geom2d_Curve = cur1Geom2d_Line;
			findSecondObject(i);
			break;	
	case CircleSketcherObject:
	case ArcSketcherObject: 
			cur1Geom2d_Curve = Handle(Geom2d_Curve)::DownCast(mySObject->GetGeometry());
			findSecondObject(i);
			break;
	default:break;
	}
  }

  if(minDistance == minimumSnapDistance) 
		 bestPnt2d = curPnt2d;
  else   findbestPnt2d = Standard_True;
}

/**
 * \fn findSecondObject(Standard_Integer start)
 * \brief find intersection points for object with start index
 * \return void 
 * \param start Standard_Integer
 */
void Sketcher_SnapIntersection::findSecondObject(Standard_Integer start)
{
  for( Standard_Integer j=start+1; j<=data->Length(); j++)
  {
	secondSObject = Handle(Sketcher_Object)::DownCast(data->Value(j));
	myGeometryType = secondSObject->GetGeometryType();
	switch (myGeometryType)
	{
	case PointSketcherObject:	
			break;
	case LineSketcherObject:
		{
			cur2Geom2d_Edge = Handle(Geom2d_Edge)::DownCast(secondSObject->GetGeometry());
			cur2Geom2d_Line->SetLin2d(cur2Geom2d_Edge->Lin2d());
			my2dAPI_InterCurveCurve.Init(cur1Geom2d_Curve,cur2Geom2d_Line,1.0e-12);
			for( Standard_Integer k=1; k<=my2dAPI_InterCurveCurve.NbPoints(); k++)
			{
				objectPnt2d = my2dAPI_InterCurveCurve.Point(k);
				curDistance = objectPnt2d.Distance(curPnt2d);
				if(minDistance > curDistance)
				{
					bpu = ElCLib::Parameter(cur2Geom2d_Edge->Lin2d(),objectPnt2d);
					if( bpu > cur2Geom2d_Edge->StartParameter() &&  bpu < cur2Geom2d_Edge->EndParameter())
					{
						if (cur1Geom2d_Curve->IsKind(STANDARD_TYPE(Geom2d_Line)))
						{
							bpu = ElCLib::Parameter(cur1Geom2d_Edge->Lin2d(),objectPnt2d);
							if( bpu > cur1Geom2d_Edge->StartParameter() &&  bpu < cur1Geom2d_Edge->EndParameter())
							{
								bestPnt2d = objectPnt2d;
								minDistance = curDistance;
								curHilightedObj = mySObject->GetAIS_Object();
								curHilightedObj2 = secondSObject->GetAIS_Object();
							}
						}
						else 
						{
							bestPnt2d = objectPnt2d;
							minDistance = curDistance;
							curHilightedObj = mySObject->GetAIS_Object();
							curHilightedObj2 = secondSObject->GetAIS_Object();
						}
					}
				}
			}
		}
		break;
	case CircleSketcherObject:	
	case ArcSketcherObject:
		{
			cur2Geom2d_Curve = Handle(Geom2d_Curve)::DownCast(secondSObject->GetGeometry());
			my2dAPI_InterCurveCurve.Init(cur1Geom2d_Curve,cur2Geom2d_Curve,1.0e-12);
			for( Standard_Integer k=1; k<=my2dAPI_InterCurveCurve.NbPoints(); k++)
			{
				objectPnt2d = my2dAPI_InterCurveCurve.Point(k);
				curDistance = objectPnt2d.Distance(curPnt2d);
				if(minDistance > curDistance)
				{
					if (cur1Geom2d_Curve->IsKind(STANDARD_TYPE(Geom2d_Line)))
					{
						bpu = ElCLib::Parameter(cur1Geom2d_Edge->Lin2d(),objectPnt2d);
						if( bpu > cur1Geom2d_Edge->StartParameter() &&  bpu < cur1Geom2d_Edge->EndParameter())
						{
							bestPnt2d = objectPnt2d;
							minDistance = curDistance;
							curHilightedObj = mySObject->GetAIS_Object();
							curHilightedObj2 = secondSObject->GetAIS_Object();
						}
					}
					else
					{
						bestPnt2d = objectPnt2d;
						minDistance = curDistance;
						curHilightedObj = mySObject->GetAIS_Object();
						curHilightedObj2 = secondSObject->GetAIS_Object();
					}
				}
			}
		}
		break;
	default:break;
	}
  }
}


/**
 * \fn GetSnapType()
 * \brief get Snap type
 * \return Sketcher_SnapType 
 */
Sketcher_SnapType Sketcher_SnapIntersection::GetSnapType()
{
	return SnapIntersection;
}


/**
 * \fn DrawRelation()
 * \brief draw relation 
 * \return void 
 */
void Sketcher_SnapIntersection::DrawRelation()
{
	myContext->SetSelected(curHilightedObj);
	myContext->SetSelected(curHilightedObj2);
}


IMPLEMENT_STANDARD_HANDLE(Sketcher_SnapIntersection,Sketcher_Snap)
IMPLEMENT_STANDARD_RTTI(Sketcher_SnapIntersection)

IMPLEMENT_STANDARD_TYPE(Sketcher_SnapIntersection)
IMPLEMENT_STANDARD_SUPERTYPE(Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_SnapIntersection)
