/**
* \file Sketcher_SnapTangent.cxx
* \brief Implementation file for the class Sketcher_SnapTangent
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapTangent.cxx">Sergei Maslov</a>
*/
#include "Sketcher_SnapTangent.hxx"

/**
 * \fn Sketcher_SnapTangent() 
 * \brief Constructs a Sketcher_SnapTangent
 */
Sketcher_SnapTangent::Sketcher_SnapTangent()
{
	myTangentType = NothingTangent;
	FixFirstPnt = Standard_False;
	minAngle = 0;
	curAngle = 0;
	radiusDistance = 0;

	myFirstPnt2d = gp::Origin2d();
	tempDir2d = gp::DX2d();
	tempGeom2d_Line = new Geom2d_Line(myFirstPnt2d,tempDir2d);
	tempGeom2d_Circle = new Geom2d_Circle(gp::OX2d(),0);

	myTanRelation = new AIS_TangentRelation(FirstEdge,SecondEdge,myPlane);
}


/**
 * \fn ~Sketcher_SnapTangent() 
 * \brief Destructor
 */
Sketcher_SnapTangent::~Sketcher_SnapTangent()
{
}


/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d)
 * \brief input event handler
 * \return gp_Pnt2d 
 * \param thePnt2d const gp_Pnt2d&
 */
gp_Pnt2d Sketcher_SnapTangent::MouseInputEvent(const gp_Pnt2d& tempPnt2d)
{
  curPnt2d = tempPnt2d;
  switch (myTangentType)
  {
	case NothingTangent: 	bestPnt2d = curPnt2d;
							break;
	case Line_FirstPnt:		SelectLine_FirstPntEvent();
							if(findbestPnt2d)
							{
								Geom2dAdaptor_Curve curAdaptor_Curve(tempGeom2d_Curve);
								Geom2dGcc_QualifiedCurve curQualifiedCurve (curAdaptor_Curve,GccEnt_unqualified);
								Geom2dGcc_Lin2d2Tan curLin2d2Tan(curQualifiedCurve,bestPnt2d,1.0e-10);
								if (curLin2d2Tan.NbSolutions() > 0)
								{
									mylin2d = curLin2d2Tan.ThisSolution(1);
									FixFirstPnt = Standard_True;
								}
								else bestPnt2d = tempPnt2d;
							}
							break;
	case Line_SecondPnt:	if (FixFirstPnt)
								 SelectLine_FixEvent();
							else SelectLine_CheckFirstPntEvent();
							FixFirstPnt = Standard_False;
							break;
	case Circle_CenterPnt:	SelectCurveCenterEvent();
							break;
	default:				bestPnt2d = curPnt2d;
							break;
  }

  EraseSnap();
  return bestPnt2d;
}

/**
 * \fn SelectEvent() 
 * \brief find nearest intersection point
 * \return void 
 */
void Sketcher_SnapTangent::SelectEvent()
{
  switch (myTangentType)
  {
	case Line_FirstPnt:		SelectLine_FirstPntEvent();
							break;
	case Line_SecondPnt:	if (FixFirstPnt)
								 SelectLine_FixEvent();
							else SelectLine_CheckFirstPntEvent();
							break;
	case Circle_CenterPnt:	SelectCurveCenterEvent();
							break;
	default:				bestPnt2d = curPnt2d;
							break;
  }
}


/**
 * \fn GetSnapType()
 * \brief get Snap type
 * \return Sketcher_SnapType 
 */
Sketcher_SnapType Sketcher_SnapTangent::GetSnapType()
{
	return SnapTangent;
}

/**
 * \fn EraseSnap()
 * \brief cancel event handler
 * \return void 
 */
void Sketcher_SnapTangent::EraseSnap()
{
    myContext->Remove(myAIS_Point);
	myTangentType = NothingTangent;
	FixFirstPnt = Standard_False;
}


/**
 * \fn DrawRelation()
 * \brief draw relation 
 * \return void 
 */
void Sketcher_SnapTangent::DrawRelation()
{
	myTanRelation->SetFirstShape(FirstEdge);
	myTanRelation->SetSecondShape(SecondEdge);
//	if(firstDisplay)	myContext->Display(myTanRelation,0,-1);
//	else		myContext->Redisplay(myTanRelation);
}

/**
 * \fn EraseRelation()
 * \brief erase relation 
 * \return void 
 */
void Sketcher_SnapTangent::EraseRelation()
{
//	myContext->Remove(myTanRelation);
}


/**
 * \fn setFirstPnt(const gp_Pnt2d& p)
 * \brief set point for exceptions
 * \return void 
 * \param p const gp_Pnt2d&
 * \param ttype TangentType
*/
void Sketcher_SnapTangent::setFirstPnt(const gp_Pnt2d& p,TangentType ttype)
{
	myFirstPnt2d = p;
	myTangentType = ttype;
}


/**
 * \fn SelectLine_FirstPntEvent()
 * \brief Find nearest point on curve and store it
 * \return void 
*/
void Sketcher_SnapTangent::SelectLine_FirstPntEvent()
{
  findbestPnt2d = Standard_False;
  minDistance = minimumSnapDistance;
  for( Standard_Integer i=1; i<=data->Length(); i++)
	{
		mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
		myGeometryType = mySObject->GetGeometryType();
		switch (myGeometryType)
		{
		case PointSketcherObject:	break;
		case LineSketcherObject:	break;
		case CircleSketcherObject:	curGeom2d_Circle = Handle(Geom2d_Circle)::DownCast(mySObject->GetGeometry());
									ProjectOnCurve.Init(curPnt2d,curGeom2d_Circle);
									if(countProject())
									{
										bestPnt2d = objectPnt2d;
										tempGeom2d_Curve = curGeom2d_Circle;
									}
									break;
		case ArcSketcherObject: 	curGeom2d_Arc = Handle(Geom2d_Arc)::DownCast(mySObject->GetGeometry());
									objectPnt2d = curGeom2d_Arc->FirstPnt();
									if(count())
									{
										bestPnt2d = objectPnt2d;
										tempGeom2d_Circle->SetCirc2d(curGeom2d_Arc->Circ2d());
										tempGeom2d_Curve = tempGeom2d_Circle;
									}
									objectPnt2d = curGeom2d_Arc->LastPnt();
									if(count())
									{
										bestPnt2d = objectPnt2d;
										tempGeom2d_Circle->SetCirc2d(curGeom2d_Arc->Circ2d());
										tempGeom2d_Curve = tempGeom2d_Circle;
									}
									ProjectOnCurve.Init(curPnt2d,curGeom2d_Arc);
									if(ProjectOnCurve.NbPoints() > 0)
									{
										objectPnt2d = ProjectOnCurve.NearestPoint();
										curDistance = objectPnt2d.Distance(curPnt2d) + minimumSnapDistance/2;
										if(minDistance > curDistance)
										{
											minDistance = curDistance;
											bestPnt2d = objectPnt2d;
											tempGeom2d_Circle->SetCirc2d(curGeom2d_Arc->Circ2d());
											tempGeom2d_Curve = tempGeom2d_Circle;
										}
									}
									break;
		default:break;
		}
	}
  if(minDistance == minimumSnapDistance) 
		 bestPnt2d = curPnt2d;
  else   findbestPnt2d = Standard_True;
}

/**
 * \fn SelectLine_CheckFirstPntEvent()
 * \brief check first point of line belonging to circle/arc
 * \return void 
*/
void Sketcher_SnapTangent::SelectLine_CheckFirstPntEvent()
{
  findbestPnt2d = Standard_False;
  minDistance = 0.0001;
  objectPnt2d = myFirstPnt2d;

  for( Standard_Integer i=1; i<=data->Length(); i++)
	{
		mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
		myGeometryType = mySObject->GetGeometryType();
		switch (myGeometryType)
		{
		case PointSketcherObject:	break;
		case LineSketcherObject:	break;
		case CircleSketcherObject:	curGeom2d_Circle = Handle(Geom2d_Circle)::DownCast(mySObject->GetGeometry());
									ProjectOnCurve.Init(myFirstPnt2d,curGeom2d_Circle);
									if(ProjectOnCurve.NbPoints() > 0)
									{
										objectPnt2d = ProjectOnCurve.NearestPoint();
										curDistance = myFirstPnt2d.Distance(objectPnt2d);
										if(minDistance > curDistance)
										{
											minDistance = curDistance;
											bestPnt2d = objectPnt2d;
											tempGeom2d_Curve = curGeom2d_Circle;
										}
									}
									break;
		case ArcSketcherObject: 	curGeom2d_Arc = Handle(Geom2d_Arc)::DownCast(mySObject->GetGeometry());
									ProjectOnCurve.Init(myFirstPnt2d,curGeom2d_Arc);
									if(ProjectOnCurve.NbPoints() > 0)
									{
										objectPnt2d = ProjectOnCurve.NearestPoint();
										curDistance = myFirstPnt2d.Distance(objectPnt2d);
										if(minDistance > curDistance)
										{
											minDistance = curDistance;
											bestPnt2d = objectPnt2d;
											tempGeom2d_Circle->SetCirc2d(curGeom2d_Arc->Circ2d());
											tempGeom2d_Curve = tempGeom2d_Circle;
										}
									}
									break;
		default:break;
		}
	}
  if(minDistance != 0.0001)   
	{
		Geom2dAdaptor_Curve curAdaptor_Curve(tempGeom2d_Curve);
		Geom2dGcc_QualifiedCurve curQualifiedCurve (curAdaptor_Curve,GccEnt_unqualified);
		Geom2dGcc_Lin2d2Tan curLin2d2Tan(curQualifiedCurve,bestPnt2d,1.0e-6);

		if (curLin2d2Tan.NbSolutions() > 0)
		{
			mylin2d = curLin2d2Tan.ThisSolution(1);
			FixFirstPnt = Standard_True;
			SelectLine_FixEvent();
		}
		else bestPnt2d = curPnt2d;

	}
  else SelectLine_SecondPntEvent();

}

/**
 * \fn SelectLine_FixEvent()
 * \brief find tangent line at first point
 * \return void 
*/
void Sketcher_SnapTangent::SelectLine_FixEvent()
{
	minAngle = MINANGLE*2;
	gce_MakeDir2d MD(myFirstPnt2d,curPnt2d);
	if(MD.IsDone())
	{
		tempDir2d = MD.Value();
		if(mylin2d.Direction().IsParallel(tempDir2d,minAngle))
		{
			curAngle = mylin2d.Direction().Angle(tempDir2d);
			if (minAngle > curAngle) 
				minAngle = curAngle;
			else minAngle = M_PI - curAngle;

			tempGeom2d_Line->SetLin2d(mylin2d);
			ProjectOnCurve.Init(curPnt2d,tempGeom2d_Line);
			if(ProjectOnCurve.NbPoints() > 0)
				bestPnt2d = ProjectOnCurve.NearestPoint();
		}
	}

	if (minAngle == MINANGLE*2)
		SelectLine_SecondPntEvent();
	else findbestPnt2d = Standard_True;

}


/**
 * \fn SelectLine_SecondPntEvent()
 * \brief find tangent line at second point
 * \return void 
*/
void Sketcher_SnapTangent::SelectLine_SecondPntEvent()
{
  findbestPnt2d = Standard_False;
  minDistance = minimumSnapDistance;
  for( Standard_Integer i=1; i<=data->Length(); i++)
	{
		mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
		myGeometryType = mySObject->GetGeometryType();
		switch (myGeometryType)
		{
		case PointSketcherObject:	break;
		case LineSketcherObject:	break;
		case CircleSketcherObject:
		case ArcSketcherObject: 	curGeom2d_Circle = Handle(Geom2d_Circle)::DownCast(mySObject->GetGeometry());
									ProjectOnCurve.Init(curPnt2d,curGeom2d_Circle);
									if(countProject())
									{
										radiusDistance = curGeom2d_Circle->Radius();
										tempGeom2d_Curve = curGeom2d_Circle;
									}
									break;
		default:break;
		}
	}

  if(minDistance == minimumSnapDistance) 
		 bestPnt2d = curPnt2d;
  else   
  {
	Geom2dAdaptor_Curve curAdaptor_Curve(tempGeom2d_Curve);
	Geom2dGcc_QualifiedCurve curQualifiedCurve (curAdaptor_Curve,GccEnt_unqualified);
	Geom2dGcc_Lin2d2Tan curLin2d2Tan(curQualifiedCurve,myFirstPnt2d,1.0e-10);

    minDistance = radiusDistance;
	for(Standard_Integer k=1; k<=curLin2d2Tan.NbSolutions(); k++)
	{
		tempGeom2d_Line->SetLin2d(curLin2d2Tan.ThisSolution(k));
		my2dAPI_InterCurveCurve.Init(tempGeom2d_Line,tempGeom2d_Curve);
		if (my2dAPI_InterCurveCurve.NbPoints() > 0)
		{
			objectPnt2d = my2dAPI_InterCurveCurve.Point(1);
			if(count())
				bestPnt2d = objectPnt2d;
		}
	}
	if(minDistance == radiusDistance) 
		bestPnt2d = curPnt2d;
	else findbestPnt2d = Standard_True;
  }
}

/**
 * \fn SelectCurveCenterEvent()
 * \brief find tangent circle to curve
 * \return void 
*/
void Sketcher_SnapTangent::SelectCurveCenterEvent()
{
  findbestPnt2d = Standard_False;
  minDistance = minimumSnapDistance;
  for( Standard_Integer i=1; i<=data->Length(); i++)
	{
		mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
		myGeometryType = mySObject->GetGeometryType();
		switch (myGeometryType)
		{
		case PointSketcherObject:	break;
		case LineSketcherObject:	
		case CircleSketcherObject:
		case ArcSketcherObject: 	
			{
				Handle(Geom2d_Curve) curGeom2d_Curve = Handle(Geom2d_Curve)::DownCast(mySObject->GetGeometry());
				ProjectOnCurve.Init(curPnt2d,curGeom2d_Curve);
				if(countProject())
					tempGeom2d_Curve = curGeom2d_Curve;
			}
			break;
		default:break;
		}
	}

  if(minDistance == minimumSnapDistance) 
		 bestPnt2d = curPnt2d;
  else 
  {
	ProjectOnCurve.Init(myFirstPnt2d,tempGeom2d_Curve);
	if (ProjectOnCurve.NbPoints() > 0)
	{
		objectPnt2d = ProjectOnCurve.Point(1);
		minDistance = curPnt2d.Distance(objectPnt2d);
		bestPnt2d = objectPnt2d;
		findbestPnt2d = Standard_True;
		for( Standard_Integer i=2; i<=ProjectOnCurve.NbPoints(); i++)
		{
			objectPnt2d = ProjectOnCurve.Point(i);
			if(count())
				bestPnt2d = objectPnt2d;
		}
	}
	else  bestPnt2d = curPnt2d;
  }
}

IMPLEMENT_STANDARD_HANDLE(Sketcher_SnapTangent,Sketcher_Snap)
IMPLEMENT_STANDARD_RTTI(Sketcher_SnapTangent)

IMPLEMENT_STANDARD_TYPE(Sketcher_SnapTangent)
IMPLEMENT_STANDARD_SUPERTYPE(Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_SnapTangent)
