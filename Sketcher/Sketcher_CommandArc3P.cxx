/**
* \file Sketcher_CommandArc3P.cxx
* \brief Implementation file for the class Sketcher_CommandArc3P
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandArc3P.cxx">Sergei Maslov</a>
*/
#include "Sketcher_CommandArc3P.hxx"

/**
 * \fn Sketcher_CommandArc3P() 
 * \brief Constructs a Sketcher_CommandArc3P
 */
Sketcher_CommandArc3P::Sketcher_CommandArc3P()
: Sketcher_Command("Arc3P."),
  tempCirc(curCoordinateSystem.Ax2(),SKETCHER_RADIUS),
  temp2dAdaptor_Curve(),
  temp2d_Circ()
{
	myArc3PAction = Nothing;

	tempGeom_Circle = new Geom_Circle(curCoordinateSystem.Ax2(),SKETCHER_RADIUS);
	myRubberCircle = new AIS_Circle(tempGeom_Circle);
	myRubberCircle->SetColor(Quantity_NOC_LIGHTPINK1);

	mySecondgp_Pnt2d = gp::Origin2d();
	third_Pnt = gp::Origin();
	midpoint2d = gp::Origin2d();
	tempu1_pnt2d = gp::Origin2d();
	tempu2_pnt2d = gp::Origin2d();

	tempGeom2d_Line = new Geom2d_Line(tempu1_pnt2d,gp::DX2d());
	tempGeom2d_Circle = new Geom2d_Circle(temp2d_Circ);

	FirstGeom2d_Point = new Geom2d_CartesianPoint(tempu1_pnt2d);
	TempGeom2d_Point = new Geom2d_CartesianPoint(tempu2_pnt2d);

	u1=u2=0;
	temp_u1=temp_u2=0;
	dist1 = dist2 = 0;
}


/**
 * \fn ~Sketcher_CommandArc3P() 
 * \brief destructor
 */
Sketcher_CommandArc3P::~Sketcher_CommandArc3P()
{
}

/**
 * \fn Action() 
 * \brief turn command to active state 
 */
void Sketcher_CommandArc3P::Action()
{
	myArc3PAction = Input_1ArcPoint;
	tempCirc.SetPosition (curCoordinateSystem.Ax2());
}
			
			
/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d )
 * \brief input event handler
 * \return Standard_Boolean 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_Boolean Sketcher_CommandArc3P::MouseInputEvent(const gp_Pnt2d& thePnt2d )
{
 curPnt2d = myAnalyserSnap->MouseInput(thePnt2d);
 switch (myArc3PAction)
 {
	case Nothing:	break;

	case Input_1ArcPoint:
			myFirstgp_Pnt2d = curPnt2d;
			myFirstPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
			if (!myPolylineMode)
			{
				myRubberLine->SetPoints(myFirstPoint,myFirstPoint);
				myContext->Display(myRubberLine,0,-1);
				myArc3PAction = Input_2ArcPoint;
			}
			else	
			{
				findlastSObject();
				myContext->Display(myRubberCircle,0,-1);
			}
			break;

	case Input_2ArcPoint:
			{
			mySecondgp_Pnt2d = curPnt2d;
			mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
			tempCirc.SetLocation(myFirstPoint->Pnt().Scaled(mySecondPoint->Pnt(),0.5));
			tempCirc.SetRadius(myFirstgp_Pnt2d.Distance(curPnt2d) / 2);

			tempGeom_Circle->SetCirc(tempCirc);
			myRubberCircle->SetCircle(tempGeom_Circle);

			Standard_Real u1 = ElCLib::Parameter(tempCirc,myFirstPoint->Pnt());
			Standard_Real u2 = ElCLib::Parameter(tempCirc,mySecondPoint->Pnt());

			if ( u1 > u2 )
			{
				myRubberCircle->SetFirstParam(u2);
				myRubberCircle->SetLastParam (u1);
			}
			else
			{
				myRubberCircle->SetFirstParam(u1);
				myRubberCircle->SetLastParam (u2);
			}

			myContext->Remove(myRubberLine);
			myContext->Display(myRubberCircle,0,-1);
			myContext->Redisplay(myRubberCircle);
			
			myArc3PAction = Input_3ArcPoint;
			}
			break;
	case Input_3ArcPoint:
			{
			Handle (Geom2d_CartesianPoint) Geom2d_Point1 = new Geom2d_CartesianPoint(myFirstgp_Pnt2d);
			Handle (Geom2d_CartesianPoint) Geom2d_Point2 = new Geom2d_CartesianPoint(mySecondgp_Pnt2d);
			Handle (Geom2d_CartesianPoint) Geom2d_Point3 = new Geom2d_CartesianPoint(curPnt2d);

			Geom2dGcc_Circ2d3Tan tempGcc_Circ2d3Tan(Geom2d_Point1,Geom2d_Point2,Geom2d_Point3,1.0e-10);
			if (tempGcc_Circ2d3Tan.IsDone() && tempGcc_Circ2d3Tan.NbSolutions()>0)
			{
				Handle(Geom2d_Arc) myGeom2d_Arc = new Geom2d_Arc(tempGcc_Circ2d3Tan.ThisSolution(1));
				myGeom2d_Arc->SetParam(myFirstgp_Pnt2d,mySecondgp_Pnt2d,curPnt2d);

				Handle(Geom_Circle) Geom_Circle1 = new Geom_Circle(ElCLib::To3d(curCoordinateSystem.Ax2(),myGeom2d_Arc->Circ2d()));
				Handle(AIS_Circle) myAIS_Circle = new AIS_Circle(Geom_Circle1);

				myAIS_Circle->SetFirstParam(myGeom2d_Arc->FirstParameter());
				myAIS_Circle->SetLastParam (myGeom2d_Arc->LastParameter());
		
				AddObject(myGeom2d_Arc,myAIS_Circle,ArcSketcherObject);

				myContext->Remove(myRubberCircle);
				myContext->Display(myAIS_Circle);

				myArc3PAction = Input_1ArcPoint;
			}
			}
			break;
	case Input_PolylineArc:
			{
			TempGeom2d_Point->SetPnt2d(curPnt2d);
			Geom2dGcc_QualifiedCurve temp2d_QualifiedCurve(temp2dAdaptor_Curve, GccEnt_unqualified);
			Geom2dGcc_Circ2d3Tan tempGcc_Circ2d3Tan(temp2d_QualifiedCurve,FirstGeom2d_Point,TempGeom2d_Point,1.0e-6,0);
			if (tempGcc_Circ2d3Tan.IsDone() && tempGcc_Circ2d3Tan.NbSolutions()>0)
			{
				temp2d_Circ = tempGcc_Circ2d3Tan.ThisSolution(1);

				u1 = ElCLib::Parameter(temp2d_Circ,myFirstgp_Pnt2d);
				u2 = ElCLib::Parameter(temp2d_Circ,curPnt2d);
				temp_u1 = u1 + (u2 - u1) / 100;
				temp_u2 = u1 - (u2 - u1) / 100;
				
				tempu1_pnt2d = ElCLib::Value(temp_u1, temp2d_Circ);	
				tempu2_pnt2d = ElCLib::Value(temp_u2, temp2d_Circ);	
				dist1 = tempu1_pnt2d.Distance(midpoint2d);
				dist2 = tempu2_pnt2d.Distance(midpoint2d);

				if (dist1 < dist2) tempu1_pnt2d = tempu2_pnt2d;

				Handle(Geom2d_Arc) myGeom2d_Arc = new Geom2d_Arc(temp2d_Circ);
				myGeom2d_Arc->SetParam(myFirstgp_Pnt2d,tempu1_pnt2d,curPnt2d);

				Handle(Geom_Circle) Geom_Circle1 = new Geom_Circle(ElCLib::To3d(curCoordinateSystem.Ax2(),myGeom2d_Arc->Circ2d()));
				Handle(AIS_Circle) myAIS_Circle = new AIS_Circle(Geom_Circle1);
				myAIS_Circle->SetFirstParam(myGeom2d_Arc->FirstParameter());
				myAIS_Circle->SetLastParam (myGeom2d_Arc->LastParameter());
				myContext->Display(myAIS_Circle);

				AddObject(myGeom2d_Arc,myAIS_Circle,ArcSketcherObject);
				
				midpoint2d = myGeom2d_Arc->MiddlePnt();
				tempGeom2d_Circle->SetCirc2d(myGeom2d_Arc->Circ2d());
				temp2dAdaptor_Curve.Load(tempGeom2d_Circle);

				myFirstgp_Pnt2d = curPnt2d;
				FirstGeom2d_Point->SetPnt2d(myFirstgp_Pnt2d);
				myFirstPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));

				tempGeom_Circle->SetRadius(0);
				myRubberCircle->SetCircle(tempGeom_Circle);
				myContext->Redisplay(myRubberCircle);
			}
			}

	default:break;
 }
 return Standard_False;
}

/**
 * \fn MouseMoveEvent(const gp_Pnt2d& thePnt2d )
 * \brief mouse move handler
 * \return void 
 * \param thePnt2d const gp_Pnt2d&
 */
void Sketcher_CommandArc3P::MouseMoveEvent (const gp_Pnt2d& thePnt2d)
{
 curPnt2d = myAnalyserSnap->MouseMove(thePnt2d);
 switch (myArc3PAction)
 {
	case Nothing:break;

	case Input_1ArcPoint: 
			curPnt2d = myAnalyserSnap->MouseMove(thePnt2d);
			break;

	case Input_2ArcPoint:
			mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
			myRubberLine->SetPoints(myFirstPoint,mySecondPoint);
			myContext->Redisplay(myRubberLine);
			break;
	case Input_3ArcPoint:
		{
			third_Pnt =	ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d);
			gce_MakeCirc tempMakeCirc = gce_MakeCirc(myFirstPoint->Pnt(),mySecondPoint->Pnt(),third_Pnt);
			if (tempMakeCirc.Status() == gce_Done) 
				{
					tempGeom_Circle->SetCirc(tempMakeCirc.Value());
					myRubberCircle->SetCircle(tempGeom_Circle);
					myRubberCircle->SetFirstParam(ElCLib::Parameter(tempGeom_Circle->Circ(),myFirstPoint->Pnt()));
					myRubberCircle->SetLastParam (ElCLib::Parameter(tempGeom_Circle->Circ(),third_Pnt));
					myContext->Redisplay(myRubberCircle);
				}
		}
			break;
	case Input_PolylineArc: 
		{
			third_Pnt =	ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d);
			TempGeom2d_Point->SetPnt2d(curPnt2d);
			Geom2dGcc_QualifiedCurve temp2d_QualifiedCurve(temp2dAdaptor_Curve, GccEnt_unqualified);
			Geom2dGcc_Circ2d3Tan tempGcc_Circ2d3Tan(temp2d_QualifiedCurve,FirstGeom2d_Point,TempGeom2d_Point,1.0e-6,0);
			if (tempGcc_Circ2d3Tan.IsDone() && tempGcc_Circ2d3Tan.NbSolutions()>0)
			{
				temp2d_Circ = tempGcc_Circ2d3Tan.ThisSolution(1);

				u1 = ElCLib::Parameter(temp2d_Circ,myFirstgp_Pnt2d);
				u2 = ElCLib::Parameter(temp2d_Circ,curPnt2d);
				temp_u1 = u1 + (u2 - u1) / 100;
				temp_u2 = u1 - (u2 - u1) / 100;
				
				tempu1_pnt2d = ElCLib::Value(temp_u1, temp2d_Circ);	
				tempu2_pnt2d = ElCLib::Value(temp_u2, temp2d_Circ);	
				dist1 = tempu1_pnt2d.Distance(midpoint2d);
				dist2 = tempu2_pnt2d.Distance(midpoint2d);
				if (dist1 < dist2) tempu1_pnt2d = tempu2_pnt2d;

				gce_MakeCirc tempMakeCirc(myFirstPoint->Pnt(),ElCLib::To3d(curCoordinateSystem.Ax2(),tempu1_pnt2d),third_Pnt);
				if (tempMakeCirc.Status() == gce_Done) 
				{
					tempGeom_Circle->SetCirc(tempMakeCirc.Value());
//					myRubberCircle->SetCircle(tempGeom_Circle);
					myRubberCircle->SetFirstParam(ElCLib::Parameter(tempGeom_Circle->Circ(),myFirstPoint->Pnt()));
					myRubberCircle->SetLastParam (ElCLib::Parameter(tempGeom_Circle->Circ(),third_Pnt));
					myContext->Redisplay(myRubberCircle);
				}
			}
			break;
		}
	default:break;
 }
}


/**
 * \fn CancelEvent()
 * \brief cancel event handler, stop entering object
 * \return void 
 */
void Sketcher_CommandArc3P::CancelEvent()
{
 switch (myArc3PAction)
 {
	case Nothing:	break;

	case Input_1ArcPoint: break;

	case Input_2ArcPoint:	myContext->Remove(myRubberLine);
							break;
	case Input_3ArcPoint:	myContext->Remove(myRubberCircle);
							break;
	case Input_PolylineArc: myContext->Remove(myRubberCircle);
							break;

	default: break;
 }
 myArc3PAction = Nothing;
}

/**
 * \fn GetTypeOfMethod()
 * \brief get command Method
 * \return Sketcher_ObjectTypeOfMethod 
 */
Sketcher_ObjectTypeOfMethod Sketcher_CommandArc3P::GetTypeOfMethod()
{
	return Arc3P_Method;
}

/**
 * \fn SetPolylineFirstPnt(const gp_Pnt2d& p1)
 * \brief set last entering gp_Pnt2d for polyline
 * \return void 
 * \param p1 const gp_Pnt2d&
 */
void  Sketcher_CommandArc3P::SetPolylineFirstPnt(const gp_Pnt2d& p1)
{
	myFirstgp_Pnt2d = p1;
	myFirstPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),p1));
	findlastSObject();
	myContext->Display(myRubberCircle,0,-1);
}


/**
 * \fn GetPolylineFirstPnt(gp_Pnt2d& p1)
 * \brief get last entering gp_Pnt2d for polyline
 * \return Standard_Boolean 
 * \param p1 gp_Pnt2d&
 */
Standard_Boolean  Sketcher_CommandArc3P::GetPolylineFirstPnt(gp_Pnt2d& p1)
{
 if (myArc3PAction == Input_PolylineArc && myPolylineMode == Standard_True)
 {
	 p1 = myFirstgp_Pnt2d;
	 return Standard_True;
 }
 else 
	 return Standard_False;
}


/**
 * \fn SetPolylineMode(Standard_Boolean mode)
 * \brief set polyline mode
 * \return void 
 * \param mode Standard_Boolean 
 */
void Sketcher_CommandArc3P::SetPolylineMode (Standard_Boolean mode) 
{
 myPolylineMode = mode;
 switch (myArc3PAction)
 {
	case Nothing:	break;

	case Input_1ArcPoint:	break;
							
	case Input_2ArcPoint:	findlastSObject();
							myContext->Remove(myRubberLine);
							myContext->Display(myRubberCircle,0,-1);
							break;
	case Input_3ArcPoint:	findlastSObject();
							myContext->Redisplay(myRubberCircle);
							break;
	case Input_PolylineArc: mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
							myRubberLine->SetPoints(myFirstPoint,mySecondPoint);

							myContext->Remove(myRubberCircle);
							myContext->Display(myRubberLine,0,-1);
							myArc3PAction = Input_2ArcPoint;
							break;

	default: break;
 }

}


/**
 * \fn findlastSObject()
 * \brief find last object at list of objects
 * \return void 
 */
void Sketcher_CommandArc3P::findlastSObject() 
{
 midpoint2d = gp::Origin2d();
 if (data->Length() > 0)
 {
	Handle(Sketcher_Object)	lastSObject = Handle(Sketcher_Object)::DownCast(data->Value(data->Length()));
	if (lastSObject->GetGeometryType() == LineSketcherObject)
	{
		Handle(Geom2d_Edge)	last2d_Edge = Handle(Geom2d_Edge)::DownCast(lastSObject->GetGeometry());
		if (last2d_Edge->GetStart_Pnt().IsEqual(myFirstgp_Pnt2d,1.0e-6) ||
			last2d_Edge->GetEnd_Pnt().IsEqual(myFirstgp_Pnt2d,1.0e-6))
		{
			midpoint2d = last2d_Edge->MiddlePnt();
			tempGeom2d_Line->SetLin2d(last2d_Edge->Lin2d());
			temp2dAdaptor_Curve.Load(tempGeom2d_Line);
		}
		else setTempLine();
	}
	else if (lastSObject->GetGeometryType() == ArcSketcherObject)
	{
		Handle(Geom2d_Arc)	last2d_Arc = Handle(Geom2d_Arc)::DownCast(lastSObject->GetGeometry());
		if (last2d_Arc->FirstPnt().IsEqual(myFirstgp_Pnt2d,1.0e-6) ||
			last2d_Arc->LastPnt().IsEqual(myFirstgp_Pnt2d,1.0e-6))
		{
			midpoint2d = last2d_Arc->MiddlePnt();
			tempGeom2d_Circle->SetCirc2d(last2d_Arc->Circ2d());
			temp2dAdaptor_Curve.Load(tempGeom2d_Circle);
		}
		else setTempLine();
	}
	else setTempLine();
 }
 else setTempLine();

	FirstGeom2d_Point->SetPnt2d(myFirstgp_Pnt2d);
	tempGeom_Circle->SetRadius(0);
	myRubberCircle->SetCircle(tempGeom_Circle);

	myArc3PAction = Input_PolylineArc;
}


/**
 * \fn setTempLine()
 * \brief set default horisontal line
 * \return void 
 */
void Sketcher_CommandArc3P::setTempLine() 
{
	tempGeom2d_Line->SetLocation(myFirstgp_Pnt2d);
	tempGeom2d_Line->SetDirection (gp::DX2d());
	temp2dAdaptor_Curve.Load(tempGeom2d_Line);
}


IMPLEMENT_STANDARD_HANDLE(Sketcher_CommandArc3P,Sketcher_Command)
IMPLEMENT_STANDARD_RTTI(Sketcher_CommandArc3P,Sketcher_Command)

IMPLEMENT_STANDARD_TYPE(Sketcher_CommandArc3P)
IMPLEMENT_STANDARD_SUPERTYPE(Sketcher_Command)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Sketcher_Command)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_CommandArc3P)
