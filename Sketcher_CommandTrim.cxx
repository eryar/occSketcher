/**
* \file Sketcher_CommandTrim.cxx
* \brief Implementation file for the class Sketcher_CommandTrim
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandTrim.cxx">Sergei Maslov</a>
*/
#include "Sketcher_CommandTrim.hxx"

IMPLEMENT_STANDARD_RTTIEXT(Sketcher_CommandTrim, Sketcher_Command)

/**
 * \fn Sketcher_CommandTrim() 
 * \brief Constructs a Sketcher_CommandTrim
 */
Sketcher_CommandTrim::Sketcher_CommandTrim()
: Sketcher_Command(""),
  TrimName(".Trim."),
  nearestgp_Circ2d(gp::OX2d(),SKETCHER_RADIUS)
{
	myTrimAction = NothingAction;

	minDistance = 0;
	curDistance = 0;
	NearestPntU = 0;
	NearestCurveIndex = 0;
	tempU = FirstU = SecondU = 0;
	isInputMouse = Standard_True;
	RedrawFlag = Standard_False;

	temp1Geom2d_Line = new Geom2d_Line(gp::Origin2d(),gp::DX2d());
	temp2Geom2d_Line = new Geom2d_Line(gp::Origin2d(),gp::DX2d());

	tempPnt2d = gp::Origin2d();
	mySecondgp_Pnt2d = gp::Origin2d();

	myRedrawState = NothingRedraw;
}


/**
 * \fn ~Sketcher_CommandTrim() 
 * \brief destructor
 */
Sketcher_CommandTrim::~Sketcher_CommandTrim()
{
}


/**
 * \fn Action() 
 * \brief turn command to active state 
 */
void Sketcher_CommandTrim::Action()
{
	myTrimAction = TrimObject;
	NearestCurveIndex = 0;
	myRedrawState = NothingRedraw;
}


/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d )
 * \brief input event handler
 * \return Standard_Boolean 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_Boolean Sketcher_CommandTrim::MouseInputEvent(const gp_Pnt2d& thePnt2d)
{
	curPnt2d = thePnt2d;
	isInputMouse = Standard_True;
	StandardEvent();
	NearestCurveIndex = 0;
	return Standard_False;
}


/**
 * \fn MouseMoveEvent(const gp_Pnt2d& thePnt2d )
 * \brief mouse move handler
 * \return void 
 * \param thePnt2d const gp_Pnt2d&
 */
void Sketcher_CommandTrim::MouseMoveEvent (const gp_Pnt2d& thePnt2d)
{
	curPnt2d = thePnt2d;
	isInputMouse = Standard_False;
	StandardEvent();
}


/**
 * \fn CancelEvent()
 * \brief cancel event handler, stop entering object
 * \return void 
 */
void Sketcher_CommandTrim::CancelEvent()
{
	RedrawOld();
	myTrimAction = NothingAction;
}

/**
 * \fn GetTypeOfMethod()
 * \brief get command Method
 * \return Sketcher_ObjectTypeOfMethod 
 */
Sketcher_ObjectTypeOfMethod Sketcher_CommandTrim::GetTypeOfMethod()
{
	return Trim_Method;
}

/**
 * \fn StandardEvent()
 * \brief input event and mouse move handler
 * \return void 
 */
void Sketcher_CommandTrim::StandardEvent()
{
	RedrawFlag = Standard_False;
	switch (myTrimAction)
	{
	case NothingAction:	break;

	case TrimObject:SelectNearestCurve();
					if (minDistance != TRIMDISTANCETOCURVE) 
					{
						if (NearestCurveIndex != curCurveIndex)
							FixObjectPoints();
						TrimCurve();
					}
					else RedrawOld();
					break;
	default:break;
	}
}


/**
 * \fn SelectCurve()
 * \brief select nearest curve
 * \return void
 */
void Sketcher_CommandTrim::SelectNearestCurve()
{
  minDistance = TRIMDISTANCETOCURVE;

  for( Standard_Integer i=1; i<=data->Length(); i++)
  {
	mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
	switch (mySObject->GetGeometryType())
	{
	case PointSketcherObject:	break;
	case LineSketcherObject:	curGeom2d_Edge = Handle(Geom2d_Edge)::DownCast(mySObject->GetGeometry());
								myProjectOnCurve.Init(curPnt2d,curGeom2d_Edge,curGeom2d_Edge->StartParameter(),curGeom2d_Edge->EndParameter());
								if(PointProjected(i))
									NearestPntU = ElCLib::Parameter(curGeom2d_Edge->Lin2d(),myProjectOnCurve.NearestPoint());
								break;
	case CircleSketcherObject:	curGeom2d_Circle = Handle(Geom2d_Circle)::DownCast(mySObject->GetGeometry());
								myProjectOnCurve.Init(curPnt2d,curGeom2d_Circle);
								if(PointProjected(i))
									NearestPntU = ElCLib::Parameter(curGeom2d_Circle->Circ2d(),myProjectOnCurve.NearestPoint());
								break;
	case ArcSketcherObject:		curGeom2d_Arc = Handle(Geom2d_Arc)::DownCast(mySObject->GetGeometry());
								myProjectOnCurve.Init(curPnt2d,curGeom2d_Arc,curGeom2d_Arc->FirstParameter(),curGeom2d_Arc->LastParameter());
								if(PointProjected(i))
									NearestPntU = ElCLib::Parameter(curGeom2d_Arc->Circ2d(),myProjectOnCurve.NearestPoint());
								break;
	default:break;
	}
  }
}

/**
 * \fn FixObjectPoints()
 * \brief find intersection points
 * \return void
 */
void Sketcher_CommandTrim::FixObjectPoints()
{
	NearestCurveIndex = curCurveIndex;

	FirstU = 0;
	mySeqOfPntU.Clear();

	mySObject = Handle(Sketcher_Object)::DownCast(data->Value(NearestCurveIndex));
	SetColor(mySObject->GetColor());
	SetType (mySObject->GetType());
	SetStyle(mySObject->GetStyle());
	SetWidth(mySObject->GetWidth());

	myPrs3dAspect->SetColor(myColor);
	myPrs3dAspect->SetTypeOfLine(myStyle);
	myPrs3dAspect->SetWidth(myWidth);
	myDrawer->SetLineAspect(myPrs3dAspect);
	ObjectName = mySObject->GetObjectName() + TrimName;

	TypeofObject = mySObject->GetGeometryType();
	switch (TypeofObject)
	{
	case PointSketcherObject:	break;
	case LineSketcherObject:	curGeom2d_Edge = Handle(Geom2d_Edge)::DownCast(mySObject->GetGeometry());
								mySeqOfPntU.Append(curGeom2d_Edge->StartParameter());
								mySeqOfPntU.Append(curGeom2d_Edge->EndParameter());
								temp1Geom2d_Line->SetLin2d(curGeom2d_Edge->Lin2d());
								nearestGeom2d_Curve = temp1Geom2d_Line;
								break;
	case CircleSketcherObject:	curGeom2d_Circle = Handle(Geom2d_Circle)::DownCast(mySObject->GetGeometry());
								nearestgp_Circ2d = curGeom2d_Circle->Circ2d();
								nearestGeom2d_Curve = curGeom2d_Circle;
								break;
	case ArcSketcherObject:		curGeom2d_Arc = Handle(Geom2d_Arc)::DownCast(mySObject->GetGeometry());
								FirstU = curGeom2d_Arc->FirstParameter();
								mySeqOfPntU.Append(FirstU);
								mySeqOfPntU.Append(curGeom2d_Arc->LastParameter());
								nearestgp_Circ2d = curGeom2d_Arc->Circ2d();
								nearestGeom2d_Curve = curGeom2d_Arc;
								break;
	default:break;
	}
	Standard_Integer k = 0;
	for( Standard_Integer i=1; i<=data->Length() ; i++)
	{
		if ( NearestCurveIndex != i )
		{
			mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
			switch (mySObject->GetGeometryType())
			{
			case PointSketcherObject:
					break;
			case LineSketcherObject:
					curGeom2d_Edge = Handle(Geom2d_Edge)::DownCast(mySObject->GetGeometry());
					temp2Geom2d_Line->SetLin2d(curGeom2d_Edge->Lin2d());
					my2dAPI_InterCurveCurve.Init(nearestGeom2d_Curve,temp2Geom2d_Line,1.0e-12);
					for( k=1; k<=my2dAPI_InterCurveCurve.NbPoints(); k++)
					{
						tempPnt2d = my2dAPI_InterCurveCurve.Point(k);
						tempU = ElCLib::Parameter(curGeom2d_Edge->Lin2d(),tempPnt2d);
						if( tempU > curGeom2d_Edge->StartParameter() &&  tempU < curGeom2d_Edge->EndParameter())
							SetParam();
					}
					break;
			case CircleSketcherObject:	
			case ArcSketcherObject: 
					curGeom2d_Circle = Handle(Geom2d_Circle)::DownCast(mySObject->GetGeometry());
					my2dAPI_InterCurveCurve.Init(nearestGeom2d_Curve,curGeom2d_Circle,1.0e-12);
					for( k=1; k<=my2dAPI_InterCurveCurve.NbPoints(); k++)
					{
						tempPnt2d = my2dAPI_InterCurveCurve.Point(k);
						SetParam();
					}
					break;
			default:break;
			}
		}
	}

}


/**
 * \fn TrimCurve()
 * \brief manager of cutting curve
 * \return void
 */
void Sketcher_CommandTrim::TrimCurve()
{
  	mySObject = Handle(Sketcher_Object)::DownCast(data->Value(NearestCurveIndex));
	newTrimmedObj = mySObject->GetAIS_Object();
	switch (TypeofObject)
	{
	case PointSketcherObject:	break;
	case LineSketcherObject:	if(mySeqOfPntU.Length() <= 2 ) DelTrimmedObject();
								else
								{
									FindSegment();
									if(FirstU == mySeqOfPntU.First())
										EditLine(SecondU,mySeqOfPntU.Last());
									else if(SecondU == mySeqOfPntU.Last())
										EditLine(mySeqOfPntU.First(),FirstU);
									else
										AddNewLine(SecondU,mySeqOfPntU.Last());
								}
								break;
	case CircleSketcherObject:	if(mySeqOfPntU.Length() <= 1 ) DelTrimmedObject();
								else
								{
									if ((mySeqOfPntU.First() + 2*M_PI) != mySeqOfPntU.Last())
										mySeqOfPntU.Append (mySeqOfPntU.First() + 2*M_PI);
									if(mySeqOfPntU(1) > NearestPntU)
									{
										FirstU = mySeqOfPntU(mySeqOfPntU.Length()-1);
										SecondU = mySeqOfPntU.Last();
									}
									else FindSegment();
									EditCircle();
								}
								break;

	case ArcSketcherObject:		if(mySeqOfPntU.Length() <= 2 ) DelTrimmedObject();
								else
								{
									if (NearestPntU < FirstU) NearestPntU += 2*M_PI;
									FindSegment();
									if(FirstU == mySeqOfPntU.First())
										EditArc(SecondU,mySeqOfPntU.Last());
									else if(SecondU == mySeqOfPntU.Last())
										EditArc(mySeqOfPntU.First(),FirstU);
									else 
										AddNewArc(SecondU,mySeqOfPntU.Last());
								}
								break;
	default:break;
	}
}

/**
 * \fn PointProjected(Standard_Integer index )
 * \brief find nearest point
 * \return Standard_Boolean 
 * \param index const Standard_Integer
 */
Standard_Boolean Sketcher_CommandTrim::PointProjected(Standard_Integer index)
{
	if(myProjectOnCurve.NbPoints() > 0)
	{
		curDistance = curPnt2d.Distance(myProjectOnCurve.NearestPoint());
		if(minDistance > curDistance)
		{
			minDistance = curDistance;
			curCurveIndex = index;
			return Standard_True;
		}
	}
	return  Standard_False;
}

/**
 * \fn SetParam()
 * \brief compute intersection point parameter 
 * \return void 
 */
void Sketcher_CommandTrim::SetParam()
{
	if(TypeofObject == LineSketcherObject)
	{
		tempU = ElCLib::Parameter(temp1Geom2d_Line->Lin2d(),tempPnt2d);
		if( tempU > mySeqOfPntU.First() &&  tempU < mySeqOfPntU.Last())
			AddParam();
	}
	else 
	{
		tempU = ElCLib::Parameter(nearestgp_Circ2d,tempPnt2d);
		if (tempU < FirstU) tempU += 2*M_PI;
		AddParam();
	}
}

/**
 * \fn AddParam()
 * \brief add parameter to sequence
 * \return void 
 */
void Sketcher_CommandTrim::AddParam()
{
	if(!mySeqOfPntU.IsEmpty())
	for (Standard_Integer j=1; j<=mySeqOfPntU.Length(); j++)
	{
		if(fabs(tempU - mySeqOfPntU(j)) < RealEpsilon()*10)
			break;
		else if(tempU < mySeqOfPntU(j) )
		{
			mySeqOfPntU.InsertBefore (j,tempU);
			break;
		}
		else if (j == mySeqOfPntU.Length())
		{
			mySeqOfPntU.Append (tempU);
			break;
		}
	}
	else 
		mySeqOfPntU.Append (tempU);
}

/**
 * \fn FindSegment()
 * \brief determine part of curve, that has been cutting
 * \return void 
 */
void Sketcher_CommandTrim::FindSegment()
{
	for (Standard_Integer j=2; j<=mySeqOfPntU.Length(); j++)
	{
		if(mySeqOfPntU(j) > NearestPntU)
		{
			FirstU = mySeqOfPntU(j-1);
			SecondU = mySeqOfPntU(j);
			break;
		}
	}
}

/**
 * \fn Redraw()
 * \brief redraw object, if it's necessary
 * \return Standard_Boolean 
 */
Standard_Boolean Sketcher_CommandTrim::Redraw()
{
	switch (myRedrawState)
	{
	case NothingRedraw:
			curTrimmedObj = newTrimmedObj;
			curFirstObjParam_Start = newFirstObjParam_Start;
			curFirstObjParam_End = newFirstObjParam_End;
			curSecondObjParam_Start = newSecondObjParam_Start;
			curSecondObjParam_End = newSecondObjParam_End;
			return Standard_True;
			break;

	case RedrawTrimmedObject:
			if(curTrimmedObj != newTrimmedObj)
			{
				myContext->Display(curTrimmedObj, true);
				curTrimmedObj = newTrimmedObj;
				return Standard_True;
			}
			break;

	case OneObjectRemove:	
			if(curTrimmedObj != newTrimmedObj || 
				curFirstObjParam_Start != newFirstObjParam_Start ||
				curFirstObjParam_End != newFirstObjParam_End )
			{
				myContext->Display(curTrimmedObj, false);
				myContext->Remove(FirstObj, true);
				curTrimmedObj = newTrimmedObj;
				curFirstObjParam_Start = newFirstObjParam_Start;
				curFirstObjParam_End = newFirstObjParam_End;
				return Standard_True;
			}
			break;

	case TwoObjectRemove:	
			if(curTrimmedObj != newTrimmedObj || 
				curFirstObjParam_Start != newFirstObjParam_Start ||
				curFirstObjParam_End != newFirstObjParam_End  ||
				curSecondObjParam_Start != newSecondObjParam_Start ||
				curSecondObjParam_End != newSecondObjParam_End )
			{
				myContext->Display(curTrimmedObj, false);
				myContext->Remove(FirstObj, false);
				myContext->Remove(SecondObj, true);
				curTrimmedObj = newTrimmedObj;
				curFirstObjParam_Start = newFirstObjParam_Start;
				curFirstObjParam_End = newFirstObjParam_End;
				curSecondObjParam_Start = newSecondObjParam_Start;
				curSecondObjParam_End = newSecondObjParam_End;
				return Standard_True;
			}
			break;
	default:break;
	}
	return Standard_False;
}

/**
 * \fn RedrawOld()
 * \brief draw erased object
 * \return void 
 */
void Sketcher_CommandTrim::RedrawOld()
{
	switch (myRedrawState)
	{
	case NothingRedraw:	
			break;
	case RedrawTrimmedObject:	
			myContext->Display(curTrimmedObj, true);
			break;
	case OneObjectRemove:
			myContext->Display(curTrimmedObj, false);
			myContext->Remove(FirstObj, true);
			break;
	case TwoObjectRemove:
			myContext->Display(curTrimmedObj, false);
			myContext->Remove(FirstObj, false);
			myContext->Remove(SecondObj, true);
			break;
	default:break;
	}
	myRedrawState = NothingRedraw;
}


/**
 * \fn DelTrimmedObject()
 * \brief remove object from list of objects, erase object
 * \return void 
 */
void Sketcher_CommandTrim::DelTrimmedObject()
{
	if (Redraw())
		myContext->Erase(curTrimmedObj,Standard_True);

	if(isInputMouse)
	{
		data->Remove (NearestCurveIndex);
		myRedrawState = NothingRedraw;
	}
	else myRedrawState = RedrawTrimmedObject;
}

/**
 * \fn EditLine(Standard_Real u1,Standard_Real u2)
 * \brief edit line by 2 parameters
 * \return void 
 * \param u1 Standard_Real
 * \param u2 Standard_Real
 */
void Sketcher_CommandTrim::EditLine(Standard_Real u1,Standard_Real u2)
{
	newFirstObjParam_Start = u1;
	newFirstObjParam_End = u2;

	myFirstgp_Pnt2d  = ElCLib::Value(u1,temp1Geom2d_Line->Lin2d());
	mySecondgp_Pnt2d = ElCLib::Value(u2,temp1Geom2d_Line->Lin2d());

	if (Redraw())
	{
		Handle (Geom_CartesianPoint) Geom_Point1 = new Geom_CartesianPoint(ElCLib::To3d(curCoordinateSystem.Ax2(),myFirstgp_Pnt2d));
		Handle (Geom_CartesianPoint) Geom_Point2 = new Geom_CartesianPoint(ElCLib::To3d(curCoordinateSystem.Ax2(),mySecondgp_Pnt2d));

		Handle(AIS_Line) myAIS_Line = new AIS_Line(Geom_Point1,Geom_Point2);
		myAIS_Line->SetAttributes(myDrawer);
		myContext->Erase(curTrimmedObj, false);
		myContext->Display(myAIS_Line, true);

		FirstObj = myAIS_Line;
		RedrawFlag = Standard_True;
	}

	if(isInputMouse) 
	{
		curGeom2d_Edge = Handle(Geom2d_Edge)::DownCast(mySObject->GetGeometry());
		if(curGeom2d_Edge->SetPoints(myFirstgp_Pnt2d,mySecondgp_Pnt2d))
		{
			mySObject->SetAIS_Object(FirstObj);
			myRedrawState = NothingRedraw;
		}
	}
 	else myRedrawState = OneObjectRemove;
}


/**
 * \fn AddNewLine(Standard_Real u1,Standard_Real u2)
 * \brief Constructs a new line by 2 parameters
 * \return void 
 * \param u1 Standard_Real
 * \param u2 Standard_Real
 */
void Sketcher_CommandTrim::AddNewLine(Standard_Real u1,Standard_Real u2)
{
	gp_Pnt2d StartPnt2d(ElCLib::Value(u1,temp1Geom2d_Line->Lin2d()));
	gp_Pnt2d EndPnt2d(ElCLib::Value(u2,temp1Geom2d_Line->Lin2d()));
	newSecondObjParam_Start = u1;
	newSecondObjParam_End = u2;
	
	EditLine(mySeqOfPntU.First(),FirstU);
	if (RedrawFlag) 
	{
		Handle (Geom_CartesianPoint) Geom_Point1 = new Geom_CartesianPoint(ElCLib::To3d(curCoordinateSystem.Ax2(),StartPnt2d));
		Handle (Geom_CartesianPoint) Geom_Point2 = new Geom_CartesianPoint(ElCLib::To3d(curCoordinateSystem.Ax2(),EndPnt2d));
		Handle(AIS_Line) myAIS_Line = new AIS_Line(Geom_Point1,Geom_Point2);
		myAIS_Line->SetAttributes(myDrawer);

		myContext->Display(myAIS_Line, true);
		SecondObj = myAIS_Line;
	}

	if(isInputMouse) 
	{
		Handle(Geom2d_Edge) myGeom2d_Edge = new Geom2d_Edge();
		if(myGeom2d_Edge->SetPoints(StartPnt2d,EndPnt2d))
			AddObject(myGeom2d_Edge,SecondObj,LineSketcherObject);
	}
	else myRedrawState = TwoObjectRemove;

}

/**
 * \fn EditCircle()
 * \brief Constructs a arc from circle by 2 parameters
 * \return void 
 */
void Sketcher_CommandTrim::EditCircle()
{
	myFirstgp_Pnt2d  = ElCLib::Value(FirstU,nearestgp_Circ2d);
	tempPnt2d = ElCLib::Value((FirstU + 2*M_PI + SecondU) / 2 ,nearestgp_Circ2d);
	mySecondgp_Pnt2d = ElCLib::Value(SecondU,nearestgp_Circ2d);

	newFirstObjParam_Start = FirstU;
	newFirstObjParam_End = SecondU;

	if (Redraw())
	{
		Handle(Geom2d_Arc) tempGeom2d_Arc = new Geom2d_Arc(nearestgp_Circ2d);
		tempGeom2d_Arc->SetParam(myFirstgp_Pnt2d,tempPnt2d,mySecondgp_Pnt2d);

		Handle(Geom_Circle) Geom_Circle1 = new Geom_Circle(ElCLib::To3d(curCoordinateSystem.Ax2(),nearestgp_Circ2d));
		Handle(AIS_Circle) myAIS_Circle = new AIS_Circle(Geom_Circle1);
		myAIS_Circle->SetFirstParam(tempGeom2d_Arc->FirstParameter());//(trimmedgp_Circ2d,myFirstgp_Pnt2d));
		myAIS_Circle->SetLastParam (tempGeom2d_Arc->LastParameter());//ElCLib::Parameter(trimmedgp_Circ2d,mySecondgp_Pnt2d));
		myAIS_Circle->SetAttributes(myDrawer);

		myContext->Erase(curTrimmedObj,Standard_False);
		myContext->Display(myAIS_Circle, true);

		FirstObj = myAIS_Circle;
	}

	if(isInputMouse) 
	{
		Handle(Geom2d_Arc) myGeom2d_Arc = new Geom2d_Arc(nearestgp_Circ2d);
		myGeom2d_Arc->SetParam(myFirstgp_Pnt2d,tempPnt2d,mySecondgp_Pnt2d);
		data->Remove (NearestCurveIndex);
		AddObject(myGeom2d_Arc,FirstObj,ArcSketcherObject);
		myRedrawState = NothingRedraw;
	}
	else myRedrawState = OneObjectRemove;

}

/**
 * \fn EditArc(Standard_Real u1,Standard_Real u2)
 * \brief edit arc by 2 parameters
 * \return void 
 * \param u1 Standard_Real
 * \param u2 Standard_Real
 */
void Sketcher_CommandTrim::EditArc(Standard_Real u1,Standard_Real u2)
{
	myFirstgp_Pnt2d  = ElCLib::Value(u1,nearestgp_Circ2d);
	tempPnt2d = ElCLib::Value((u1 + u2) / 2,nearestgp_Circ2d);
	mySecondgp_Pnt2d = ElCLib::Value(u2,nearestgp_Circ2d);
	newFirstObjParam_Start = u1;
	newFirstObjParam_End = u2;

	if (Redraw())
	{
		Handle(Geom2d_Arc) tempGeom2d_Arc = new Geom2d_Arc(nearestgp_Circ2d);
		tempGeom2d_Arc->SetParam(myFirstgp_Pnt2d,tempPnt2d,mySecondgp_Pnt2d);

		Handle(Geom_Circle) Geom_Circle1 = new Geom_Circle(ElCLib::To3d(curCoordinateSystem.Ax2(),nearestgp_Circ2d));
		Handle(AIS_Circle) myAIS_Circle = new AIS_Circle(Geom_Circle1);
		myAIS_Circle->SetFirstParam(tempGeom2d_Arc->FirstParameter());
		myAIS_Circle->SetLastParam (tempGeom2d_Arc->LastParameter());
		myAIS_Circle->SetAttributes(myDrawer);

		myContext->Erase(curTrimmedObj,Standard_False);
		myContext->Display(myAIS_Circle,Standard_True);

		FirstObj = myAIS_Circle;
		RedrawFlag = Standard_True;
	}
	
	if(isInputMouse)
	{
		curGeom2d_Arc = Handle(Geom2d_Arc)::DownCast(mySObject->GetGeometry());
		curGeom2d_Arc->SetFirstParam(u1);
		curGeom2d_Arc->SetLastParam(u2);
		mySObject->SetAIS_Object(FirstObj);
		myRedrawState = NothingRedraw;
	}
	else myRedrawState = OneObjectRemove;
					
}

/**
 * \fn AddNewArc(Standard_Real u1,Standard_Real u2)
 * \brief Constructs a new arc by 2 parameters
 * \return void 
 * \param u1 Standard_Real
 * \param u2 Standard_Real
 */
void Sketcher_CommandTrim::AddNewArc(Standard_Real u1,Standard_Real u2)
{
	gp_Pnt2d StartPnt2d(ElCLib::Value(u1,nearestgp_Circ2d));
	gp_Pnt2d midPnt2d(ElCLib::Value((u1+u2)/2,nearestgp_Circ2d));
	gp_Pnt2d EndPnt2d(ElCLib::Value(u2,nearestgp_Circ2d));
	newSecondObjParam_Start = u1;
	newSecondObjParam_End = u2;

	EditArc(mySeqOfPntU.First(),FirstU);
	if (RedrawFlag) 
	{
		Handle(Geom2d_Arc) tempGeom2d_Arc = new Geom2d_Arc(nearestgp_Circ2d);
		tempGeom2d_Arc->SetParam(StartPnt2d,midPnt2d,EndPnt2d);

		Handle(Geom_Circle) Geom_Circle1 = new Geom_Circle(ElCLib::To3d(curCoordinateSystem.Ax2(),nearestgp_Circ2d));
		Handle(AIS_Circle) myAIS_Circle = new AIS_Circle(Geom_Circle1);
		myAIS_Circle->SetFirstParam(tempGeom2d_Arc->FirstParameter());
		myAIS_Circle->SetLastParam (tempGeom2d_Arc->LastParameter());
		myAIS_Circle->SetAttributes(myDrawer);

		myContext->Display(myAIS_Circle, true);
		SecondObj = myAIS_Circle;
	}

	if(isInputMouse) 
	{
		Handle(Geom2d_Arc) myGeom2d_Arc = new Geom2d_Arc(nearestgp_Circ2d);
		myGeom2d_Arc->SetParam(StartPnt2d,midPnt2d,EndPnt2d);
		AddObject(myGeom2d_Arc,SecondObj,ArcSketcherObject);
	}
	else myRedrawState = TwoObjectRemove;
}

