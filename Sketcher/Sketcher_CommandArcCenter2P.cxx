/**
* \file Sketcher_CommandArcCenter2P.cxx
* \brief Implementation file for the class Sketcher_CommandArcCenter2P
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandArcCenter2P.cxx">Sergei Maslov</a>
*/
#include "Sketcher_CommandArcCenter2P.hxx"

/**
 * \fn Sketcher_CommandArcCenter2P() 
 * \brief Constructs a Sketcher_CommandArcCenter2P
 */
Sketcher_CommandArcCenter2P::Sketcher_CommandArcCenter2P()
: Sketcher_Command("ArcCenter2P."),
  mySecondgp_Pnt2d(gp::Origin2d())
{
	myArcCenter2PAction = Nothing;

	myRubberCenterPoint = new AIS_Point(myFirstPoint);
	myRubberCenterPoint->SetColor(Quantity_NOC_LIGHTPINK1);

	tempGeom_Circle = new Geom_Circle(curCoordinateSystem.Ax2(),SKETCHER_RADIUS);
	myRubberCircle = new AIS_Circle(tempGeom_Circle);
	myRubberCircle->SetColor(Quantity_NOC_LIGHTPINK1);
}
    
/**
 * \fn ~Sketcher_CommandArcCenter2P() 
 * \brief destructor
 */
Sketcher_CommandArcCenter2P::~Sketcher_CommandArcCenter2P()
{
}

/**
 * \fn Action() 
 * \brief turn command to active state 
 */
void Sketcher_CommandArcCenter2P::Action()
{
	myArcCenter2PAction = Input_CenterArc;
	tempGeom_Circle->SetAxis(curCoordinateSystem.Axis());
}
			
			
/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d )
 * \brief input event handler
 * \return Standard_Boolean 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_Boolean Sketcher_CommandArcCenter2P::MouseInputEvent(const gp_Pnt2d& thePnt2d )
{
 switch (myArcCenter2PAction)
 {
	case Nothing:	break;

	case Input_CenterArc:
		{
			myFirstgp_Pnt2d = myAnalyserSnap->MouseInput(thePnt2d);
			myFirstPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),myFirstgp_Pnt2d));

			tempGeom_Circle->SetLocation(myFirstPoint->Pnt());
			tempGeom_Circle->SetRadius(SKETCHER_RADIUS);
		
			myRubberCircle->SetCircle(tempGeom_Circle);
			myRubberCircle->SetFirstParam(0);
			myRubberCircle->SetLastParam (Standard_PI * 2);
			myContext->Display(myRubberCircle,0,-1);

			myRubberCenterPoint->SetComponent(myFirstPoint);
			myRubberLine->SetPoints(myFirstPoint,myFirstPoint);
			myContext->Display(myRubberCenterPoint,0,-1);
			myContext->Redisplay(myRubberCenterPoint);
			myContext->Display(myRubberLine,0,-1);

			myArcCenter2PAction = Input_1ArcPoint;
			break;
		}
	case Input_1ArcPoint:
			{
				curPnt2d = myAnalyserSnap->MouseInputException(myFirstgp_Pnt2d,thePnt2d,Circle_CenterPnt,Standard_True);
				mySecondgp_Pnt2d = curPnt2d;
				radius = myFirstgp_Pnt2d.Distance(mySecondgp_Pnt2d);
				mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
			
				tempGeom_Circle->SetRadius(radius);
				myRubberCircle->SetCircle(tempGeom_Circle);
				tempGeom2d_Circle = Handle(Geom2d_Circle)::DownCast(GeomAPI::To2d(tempGeom_Circle,gp_Pln(curCoordinateSystem)));

				myContext->Remove(myRubberLine);
				myContext->Redisplay(myRubberCircle);
			
				myArcCenter2PAction = Input_MidPoint;
			}
			break;
	case Input_2ArcPoint:
			curPnt2d = myAnalyserSnap->MouseMove(thePnt2d);
			if (ProjectOnCircle(curPnt2d))
			{
				Handle (Geom2d_CartesianPoint) Geom2d_Point1 = new Geom2d_CartesianPoint(mySecondgp_Pnt2d);
				Handle (Geom2d_CartesianPoint) Geom2d_Point2 = new Geom2d_CartesianPoint(myMidgp_Pnt2d);
				Handle (Geom2d_CartesianPoint) Geom2d_Point3 = new Geom2d_CartesianPoint(curPnt2d);

				Geom2dGcc_Circ2d3Tan tempGcc_Circ2d3Tan(Geom2d_Point1,Geom2d_Point2,Geom2d_Point3,1.0e-10);
				if (tempGcc_Circ2d3Tan.IsDone() && tempGcc_Circ2d3Tan.NbSolutions()>0)
				{
					Handle(Geom2d_Arc) myGeom2d_Arc = new Geom2d_Arc(tempGcc_Circ2d3Tan.ThisSolution(1));
					myGeom2d_Arc->SetParam(mySecondgp_Pnt2d,myMidgp_Pnt2d,curPnt2d);
	
					Handle(Geom_Circle) Geom_Circle1 = new Geom_Circle(ElCLib::To3d(curCoordinateSystem.Ax2(),myGeom2d_Arc->Circ2d()));
					Handle(AIS_Circle) myAIS_Circle = new AIS_Circle(Geom_Circle1);
	
					myAIS_Circle->SetFirstParam(myGeom2d_Arc->FirstParameter());
					myAIS_Circle->SetLastParam (myGeom2d_Arc->LastParameter());
			
					AddObject(myGeom2d_Arc,myAIS_Circle,ArcSketcherObject);
	
					myContext->Remove(myRubberCircle);
					myContext->Remove(myRubberCenterPoint);
					myContext->Display(myAIS_Circle);

					myArcCenter2PAction = Input_CenterArc;
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
void Sketcher_CommandArcCenter2P::MouseMoveEvent (const gp_Pnt2d& thePnt2d)
{
 curPnt2d = myAnalyserSnap->MouseMove(thePnt2d);
 switch (myArcCenter2PAction)
 {
	case Nothing:break;

	case Input_CenterArc: 
			curPnt2d = myAnalyserSnap->MouseMove(thePnt2d);
			break;

	case Input_1ArcPoint:
			curPnt2d = myAnalyserSnap->MouseMoveException(myFirstgp_Pnt2d,thePnt2d,Circle_CenterPnt,Standard_True);
			radius = myFirstgp_Pnt2d.Distance(curPnt2d);
			tempGeom_Circle->SetRadius(radius);
			myContext->Redisplay(myRubberCircle);

			mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
			myRubberLine->SetPoints(myFirstPoint,mySecondPoint);
			myContext->Redisplay(myRubberLine);
			break;
	case Input_MidPoint:
			if (ProjectOnCircle(thePnt2d))
			{
				if(!curPnt2d.IsEqual(myFirstgp_Pnt2d,0))
				{
					myMidgp_Pnt2d = curPnt2d;
					myFirstPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
					myArcCenter2PAction = Input_2ArcPoint;
				}
			}
			break;
	case Input_2ArcPoint:
			curPnt2d = myAnalyserSnap->MouseMove(thePnt2d);
			if (ProjectOnCircle(curPnt2d))
			{
				third_Pnt =	ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d);
				gce_MakeCirc tempMakeCirc = gce_MakeCirc(mySecondPoint->Pnt(),myFirstPoint->Pnt(),third_Pnt);
				if (tempMakeCirc.Status() == gce_Done) 
				{
					tempGeom_Circle->SetCirc(tempMakeCirc.Value());
					myRubberCircle->SetCircle(tempGeom_Circle);
					myRubberCircle->SetFirstParam(ElCLib::Parameter(tempGeom_Circle->Circ(),mySecondPoint->Pnt()));
					myRubberCircle->SetLastParam (ElCLib::Parameter(tempGeom_Circle->Circ(),third_Pnt));
					myContext->Redisplay(myRubberCircle);
				}
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
void Sketcher_CommandArcCenter2P::CancelEvent()
{
 switch (myArcCenter2PAction)
 {
	case Nothing:	break;

	case Input_CenterArc: break;

	case Input_1ArcPoint:	
	case Input_MidPoint:	myContext->Remove(myRubberLine);

	case Input_2ArcPoint:	myContext->Remove(myRubberCircle);
							myContext->Remove(myRubberCenterPoint);
							break;
	default: break;
 }
 myArcCenter2PAction = Nothing;
}

/**
 * \fn GetTypeOfMethod()
 * \brief get command Method
 * \return Sketcher_ObjectTypeOfMethod 
 */
Sketcher_ObjectTypeOfMethod Sketcher_CommandArcCenter2P::GetTypeOfMethod()
{
	return ArcCenter2P_Method;
}

/**
 * \fn ProjectOnCircle()
 * \brief project point on circle
 * \return Standard_Boolean 
 */
Standard_Boolean Sketcher_CommandArcCenter2P::ProjectOnCircle(const gp_Pnt2d& thePnt2d)
{
	ProjectOnCurve.Init(thePnt2d,tempGeom2d_Circle);
	if(ProjectOnCurve.NbPoints() > 0)
	{
		curPnt2d = ProjectOnCurve.NearestPoint();
		return Standard_True;
	}
	return Standard_False;
}

IMPLEMENT_STANDARD_HANDLE(Sketcher_CommandArcCenter2P,Sketcher_Command)
IMPLEMENT_STANDARD_RTTI(Sketcher_CommandArcCenter2P,Sketcher_Command)

IMPLEMENT_STANDARD_TYPE(Sketcher_CommandArcCenter2P)
IMPLEMENT_STANDARD_SUPERTYPE(Sketcher_Command)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Sketcher_Command)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_CommandArcCenter2P)
