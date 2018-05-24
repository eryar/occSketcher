/**
* \file Sketcher_CommandCircle2PTan.cxx
* \brief Implementation file for the class Sketcher_CommandCircle2PTan
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandCircle2PTan.cxx">Sergei Maslov</a>
*/
#include "Sketcher_CommandCircle2PTan.hxx"


/**
 * \fn Sketcher_CommandCircle2PTan() 
 * \brief Constructs a Sketcher_CommandCircle2PTan
 */
Sketcher_CommandCircle2PTan::Sketcher_CommandCircle2PTan()
: Sketcher_Command("Circle2PTan."),
  temp_Circ2d(gp::OX2d(),SKETCHER_RADIUS),
  best_Circ(gp::XOY(),SKETCHER_RADIUS)
{
	myCircle2PTanAction = Nothing;

	minDistance = 0;
	curDistance = 0;
	u_temp = 0;

	First2dPoint = new Geom2d_CartesianPoint(myFirstgp_Pnt2d);
	Second2dPoint = new Geom2d_CartesianPoint(myFirstgp_Pnt2d);
	tempGeom2d_Line = new Geom2d_Line(myFirstgp_Pnt2d,gp::DX2d());

	tempGeom2d_Circle = new Geom2d_Circle(temp_Circ2d);
	tempGeom_Circle = new Geom_Circle(best_Circ);
	myRubberCircle = new AIS_Circle(tempGeom_Circle);
	myRubberCircle->SetColor(Quantity_NOC_LIGHTPINK1);
}


/**
 * \fn ~Sketcher_CommandCircle2PTan() 
 * \brief destructor
 */
Sketcher_CommandCircle2PTan::~Sketcher_CommandCircle2PTan()
{
}

/**
 * \fn Action() 
 * \brief turn command to active state 
 */
void Sketcher_CommandCircle2PTan::Action()
{
	myCircle2PTanAction = Input_1CirclePoint;
}


/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d )
 * \brief input event handler
 * \return Standard_Boolean 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_Boolean Sketcher_CommandCircle2PTan::MouseInputEvent(const gp_Pnt2d& thePnt2d)
{
 curPnt2d = myAnalyserSnap->MouseInput(thePnt2d);
 switch (myCircle2PTanAction)
 {
	case Nothing:	break;

	case Input_1CirclePoint:
			First2dPoint->SetPnt2d(curPnt2d);
			myFirstPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));

			myRubberLine->SetPoints(myFirstPoint,myFirstPoint);
			myContext->Display(myRubberLine,0,-1);
			myCircle2PTanAction = Input_2CirclePoint;
			break;
	case Input_2CirclePoint:
			Second2dPoint->SetPnt2d(curPnt2d);
			mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));

			tempGeom_Circle->SetRadius(SKETCHER_RADIUS);
			myRubberCircle->SetCircle(tempGeom_Circle);

			myContext->Display(myRubberCircle,0,-1);

			myCircle2PTanAction = Input_3CircleTan;
			break;
	case Input_3CircleTan:
			SelectCurve();
  			if (minDistance != MINIMUMDISTANCETOCURVE) 
			{
			Geom2dAdaptor_Curve ThirdAdaptor_Curve(tempGeom2d_Curve);
			Geom2dGcc_QualifiedCurve ThirdQualifiedCurve (ThirdAdaptor_Curve,GccEnt_unqualified);
				
			Geom2dGcc_Circ2d3Tan myGcc_Circ2d3Tan(ThirdQualifiedCurve,First2dPoint,Second2dPoint,1.0e-12,u_temp);
			if (myGcc_Circ2d3Tan.IsDone() && myGcc_Circ2d3Tan.NbSolutions() >0)
			{
				temp_Circ2d = myGcc_Circ2d3Tan.ThisSolution(1);
				Standard_Integer bestCircleIndex = 1;
				if (SumDistanceToCircle())
				{
					minDistance = curDistance;
					best_Circ = ElCLib::To3d(curCoordinateSystem.Ax2(),temp_Circ2d);
					for( Standard_Integer i=2; i<=myGcc_Circ2d3Tan.NbSolutions(); i++)
					{
						temp_Circ2d = myGcc_Circ2d3Tan.ThisSolution(i);
						if (SumDistanceToCircle() && minDistance > curDistance)
						{
							minDistance = curDistance;
							bestCircleIndex = i;
							best_Circ = ElCLib::To3d(curCoordinateSystem.Ax2(),temp_Circ2d);
						}
					}
					Handle(Geom2d_Circle) myGeom2d_Circle = new Geom2d_Circle(myGcc_Circ2d3Tan.ThisSolution(bestCircleIndex));

					Handle(Geom_Circle) Geom_Circle1 = new Geom_Circle(best_Circ);
					Handle(AIS_Circle) myAIS_Circle = new AIS_Circle(Geom_Circle1);
					AddObject(myGeom2d_Circle,myAIS_Circle,CircleSketcherObject);

					myContext->Remove(myRubberLine);
					myContext->Remove(myRubberCircle);
					myContext->Display(myAIS_Circle);

					myCircle2PTanAction = Input_1CirclePoint;
				}
			}
			}
			break;
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
void Sketcher_CommandCircle2PTan::MouseMoveEvent (const gp_Pnt2d& thePnt2d)
{
 curPnt2d = myAnalyserSnap->MouseMove(thePnt2d);
 switch (myCircle2PTanAction)
 {
	case Nothing:break;

	case Input_1CirclePoint:  break;

	case Input_2CirclePoint:	
			mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
			myRubberLine->SetPoints(myFirstPoint,mySecondPoint);
			myContext->Redisplay(myRubberLine);
			break;
	case Input_3CircleTan:	
			SelectCurve();
  			if (minDistance != MINIMUMDISTANCETOCURVE) 
			{
				Geom2dAdaptor_Curve ThirdAdaptor_Curve(tempGeom2d_Curve);
				Geom2dGcc_QualifiedCurve ThirdQualifiedCurve (ThirdAdaptor_Curve,GccEnt_unqualified);
				
				Geom2dGcc_Circ2d3Tan myGcc_Circ2d3Tan(ThirdQualifiedCurve,First2dPoint,Second2dPoint,1.0e-6,u_temp);
				if (myGcc_Circ2d3Tan.IsDone() && myGcc_Circ2d3Tan.NbSolutions() >0)
				{
					temp_Circ2d = myGcc_Circ2d3Tan.ThisSolution(1);
					if (SumDistanceToCircle())
					{
						minDistance = curDistance;
						best_Circ = ElCLib::To3d(curCoordinateSystem.Ax2(),temp_Circ2d);
						for( Standard_Integer i=2; i<=myGcc_Circ2d3Tan.NbSolutions(); i++)
						{
							temp_Circ2d = myGcc_Circ2d3Tan.ThisSolution(i);
							if (SumDistanceToCircle() && minDistance > curDistance)
							{
								minDistance = curDistance;
								best_Circ = ElCLib::To3d(curCoordinateSystem.Ax2(),temp_Circ2d);
							}
						}
						tempGeom_Circle->SetCirc(best_Circ);
						myContext->Redisplay(myRubberCircle);
					}
				}
				else 
				{
					tempGeom_Circle->SetRadius(SKETCHER_RADIUS);
					myContext->Redisplay(myRubberCircle);
				}
			}
			else 
			{
				tempGeom_Circle->SetRadius(SKETCHER_RADIUS);
				myContext->Redisplay(myRubberCircle);
			}
			break;
	default:break;
 }
}


/**
 * \fn CancelEvent()
 * \brief cancel event handler, stop entering object
 * \return void 
 */
void Sketcher_CommandCircle2PTan::CancelEvent()
{
 switch (myCircle2PTanAction)
 {
	case Nothing:	break;

	case Input_1CirclePoint:	break;

	case Input_2CirclePoint: 	myContext->Remove(myRubberLine);
								break;
	case Input_3CircleTan: 		myContext->Remove(myRubberLine);
								myContext->Remove(myRubberCircle);
								break;
	default: break;
 }
 myCircle2PTanAction = Nothing;
}

/**
 * \fn GetTypeOfMethod()
 * \brief get command Method
 * \return Sketcher_ObjectTypeOfMethod 
 */
Sketcher_ObjectTypeOfMethod Sketcher_CommandCircle2PTan::GetTypeOfMethod()
{
	return Circle2PTan_Method;
}


/**
 * \fn SelectCurve()
 * \brief select nearest curve
 * \return void
 */
void Sketcher_CommandCircle2PTan::SelectCurve()
{
  minDistance = MINIMUMDISTANCETOCURVE;

  for( Standard_Integer i=1; i<=data->Length(); i++)
  {
	mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
	switch (mySObject->GetGeometryType())
	{
	case PointSketcherObject:	break;
	case LineSketcherObject:
			{
				Handle(Geom2d_Edge) curGeom2d_Edge = Handle(Geom2d_Edge)::DownCast(mySObject->GetGeometry());
				myProjectOnCurve.Init(curPnt2d,curGeom2d_Edge,curGeom2d_Edge->StartParameter(),curGeom2d_Edge->EndParameter());
				if(myProjectOnCurve.NbPoints() > 0)
				{
					curDistance = curPnt2d.Distance(myProjectOnCurve.NearestPoint());
					if(minDistance > curDistance)
					{
						minDistance = curDistance;
						tempPnt2d = myProjectOnCurve.NearestPoint();
						u_temp = ElCLib::Parameter(curGeom2d_Edge->Lin2d(),tempPnt2d);
						tempGeom2d_Line->SetLin2d(curGeom2d_Edge->Lin2d());
						tempGeom2d_Curve = tempGeom2d_Line;
					}
				}
			}
			break;
	case CircleSketcherObject:	
			{
				Handle(Geom2d_Circle) curGeom2d_Circle = Handle(Geom2d_Circle)::DownCast(mySObject->GetGeometry());
				myProjectOnCurve.Init(curPnt2d,curGeom2d_Circle);
				if(myProjectOnCurve.NbPoints() > 0)
				{
					curDistance = curPnt2d.Distance(myProjectOnCurve.NearestPoint());
					if(minDistance > curDistance)
					{
						minDistance = curDistance;
						tempPnt2d = myProjectOnCurve.NearestPoint();
						u_temp = ElCLib::Parameter(curGeom2d_Circle->Circ2d(),tempPnt2d);
						tempGeom2d_Curve = curGeom2d_Circle;
					}
				}
			}
			break;
	case ArcSketcherObject: 	
			{
				Handle(Geom2d_Arc) curGeom2d_Arc = Handle(Geom2d_Arc)::DownCast(mySObject->GetGeometry());
				myProjectOnCurve.Init(curPnt2d,curGeom2d_Arc);
				if(myProjectOnCurve.NbPoints() > 0)
				{
					curDistance = curPnt2d.Distance(myProjectOnCurve.NearestPoint());
					if(minDistance > curDistance)
					{
						minDistance = curDistance;
						tempPnt2d = myProjectOnCurve.NearestPoint();
						u_temp = ElCLib::Parameter(curGeom2d_Arc->Circ2d(),tempPnt2d);
						tempGeom2d_Circle->SetCirc2d(curGeom2d_Arc->Circ2d());
						tempGeom2d_Curve = tempGeom2d_Circle;
					}
				}
			}
			break;
	default:break;
	}
  }
}

/**
 * \fn SumDistanceToCircle()
 * \brief select circle orientation 
 * \return Standard_Boolean
 */
Standard_Boolean Sketcher_CommandCircle2PTan::SumDistanceToCircle ()
{
	tempGeom2d_Circle->SetCirc2d(temp_Circ2d);
	myProjectOnCurve.Init(tempPnt2d,tempGeom2d_Circle);
	if(myProjectOnCurve.NbPoints() > 0)
	{
		curDistance = tempPnt2d.Distance(myProjectOnCurve.NearestPoint());
		return Standard_True;
	}
	else return Standard_False;
}



IMPLEMENT_STANDARD_HANDLE(Sketcher_CommandCircle2PTan,Sketcher_Command)
IMPLEMENT_STANDARD_RTTI(Sketcher_CommandCircle2PTan,Sketcher_Command)

IMPLEMENT_STANDARD_TYPE(Sketcher_CommandCircle2PTan)
IMPLEMENT_STANDARD_SUPERTYPE(Sketcher_Command)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Sketcher_Command)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_CommandCircle2PTan)
