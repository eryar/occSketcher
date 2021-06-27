/**
* \file Sketcher_CommandBezierCurve.cxx
* \brief Implementation file for the class Sketcher_CommandBezierCurve
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandBezierCurve.cxx">Sergei Maslov</a>
*/
#include "Sketcher_CommandBezierCurve.hxx"

IMPLEMENT_STANDARD_RTTIEXT(Sketcher_CommandBezierCurve, Sketcher_Command)

/**
 * \fn Sketcher_CommandBezierCurve() 
 * \brief Constructs a Sketcher_CommandBezierCurve
 */
Sketcher_CommandBezierCurve::Sketcher_CommandBezierCurve()
: Sketcher_Command("BezierCurve."),
	tempPnt2d(gp::Origin2d()),
	myFirstgp_Pnt(gp::Origin()),
	tempPnt(gp::Origin()),
	curEdge()
{
	myBezierCurveAction = Nothing;

	TColgp_Array1OfPnt2d curgp_Array1CurvePoles2d(1,2);
	curgp_Array1CurvePoles2d.SetValue (1, myFirstgp_Pnt2d);
	curgp_Array1CurvePoles2d.SetValue (2, tempPnt2d);
	myGeom2d_BezierCurve = new Geom2d_BezierCurve(curgp_Array1CurvePoles2d);

	TColgp_Array1OfPnt curgp_Array1CurvePoles(1,2);
	curgp_Array1CurvePoles.SetValue (1, myFirstgp_Pnt);
	curgp_Array1CurvePoles.SetValue (2, tempPnt);
	myGeom_BezierCurve = new Geom_BezierCurve(curgp_Array1CurvePoles);

	myRubberAIS_Shape = new AIS_Shape(curEdge);
}

/**
 * \fn ~Sketcher_CommandBezierCurve() 
 * \brief destructor
 */
Sketcher_CommandBezierCurve::~Sketcher_CommandBezierCurve()
{
}

/**
 * \fn Action() 
 * \brief turn command to active state 
 */
void Sketcher_CommandBezierCurve::Action()
{
	myBezierCurveAction = Input_1Point;
}


/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d )
 * \brief input event handler
 * \return Standard_Boolean 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_Boolean Sketcher_CommandBezierCurve::MouseInputEvent(const gp_Pnt2d& thePnt2d)
{
 curPnt2d = myAnalyserSnap->MouseInput(thePnt2d);
 switch (myBezierCurveAction)
 {
	case Nothing:	break;
	case Input_1Point:
		{
			myFirstgp_Pnt2d = curPnt2d;
			myFirstgp_Pnt = ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d);
			myFirstPoint->SetPnt(myFirstgp_Pnt);
//			myRubberLine->SetPoints(myFirstPoint,myFirstPoint);

			Handle (Geom2d_CartesianPoint) myGeom2d_Point = new Geom2d_CartesianPoint(curPnt2d);
			Handle(AIS_Point) myAIS_Point = new AIS_Point(myFirstPoint);
			myContext->Display(myAIS_Point, true);
			AddObject(myGeom2d_Point,myAIS_Point,PointSketcherObject);

//			myContext->Display(myRubberLine,0,-1);
			firstPoint = true;
			myBezierCurveAction = Input_2Point;
			IndexCounter = 2;
		}
		break;

	case Input_2Point:
		{
			myGeom2d_BezierCurve->SetPole (1,myFirstgp_Pnt2d);
			myGeom2d_BezierCurve->SetPole (IndexCounter,curPnt2d);

			tempPnt = ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d);
			myGeom_BezierCurve->SetPole (1,myFirstgp_Pnt);
			myGeom_BezierCurve->SetPole (IndexCounter,tempPnt);
 
			mySecondPoint->SetPnt(tempPnt);
			BRepBuilderAPI_MakeEdge ME (myGeom_BezierCurve,myFirstgp_Pnt,tempPnt);
			if (ME.IsDone ())
			{
				storePoles();
				curEdge = ME.Edge();
				myRubberAIS_Shape->Set(curEdge);
				myContext->Remove(myRubberLine, false);
				myContext->Display(myRubberAIS_Shape,0,-1, true);

				myGeom2d_BezierCurve->InsertPoleAfter (IndexCounter,curPnt2d);
				myGeom_BezierCurve->InsertPoleAfter (IndexCounter,tempPnt);
				IndexCounter++;
				myBezierCurveAction = Input_OtherPoints;
			}
		}
		break;
	case Input_OtherPoints:
		{
			myGeom2d_BezierCurve->SetPole (IndexCounter,curPnt2d);
			tempPnt = ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d);
//			mySecondPoint->SetPnt(tempPnt);
			myGeom_BezierCurve->SetPole (IndexCounter,tempPnt);
 
			BRepBuilderAPI_MakeEdge ME (myGeom_BezierCurve,myFirstgp_Pnt,tempPnt);
			if (ME.IsDone ())
			{
				storePoles();
				curEdge = ME.Edge();
				if(	IndexCounter > MAXIMUMPOLES) closeBezierCurve();
				else 
				{
					myRubberAIS_Shape->Set(curEdge);
					myContext->Redisplay(myRubberAIS_Shape, true);

					myGeom2d_BezierCurve->InsertPoleAfter (IndexCounter,curPnt2d);
					myGeom_BezierCurve->InsertPoleAfter (IndexCounter,tempPnt);
					tempPnt2d = curPnt2d;
					IndexCounter++;
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
void Sketcher_CommandBezierCurve::MouseMoveEvent (const gp_Pnt2d& thePnt2d)
{
 curPnt2d = myAnalyserSnap->MouseMove(thePnt2d);
 switch (myBezierCurveAction)
 {
	case Nothing:		break;
	case Input_1Point:	break;
	case Input_2Point:	
			mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
			myRubberLine->SetPoints(myFirstPoint,mySecondPoint);
			if(firstPoint)
			{
				myContext->Display(myRubberLine,0,-1, true);
				firstPoint = false;
			}else
			{
				myContext->Redisplay(myRubberLine, true);
			}

			break;

	case Input_OtherPoints:
		{
			myGeom2d_BezierCurve->SetPole (IndexCounter,curPnt2d);
			mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
			myGeom_BezierCurve->SetPole (IndexCounter,mySecondPoint->Pnt());

			BRepBuilderAPI_MakeEdge ME (myGeom_BezierCurve,myFirstgp_Pnt,mySecondPoint->Pnt());
			if (ME.IsDone ())
			{
				curEdge = ME.Edge();
				myRubberAIS_Shape->Set(curEdge);
				myContext->Redisplay(myRubberAIS_Shape, true);
			}
			else
			IndexCounter--;
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
void Sketcher_CommandBezierCurve::CancelEvent()
{
 switch (myBezierCurveAction)
 {
	case Nothing:		break;
	case Input_1Point:	break;
	case Input_2Point:		
		myContext->Remove(myRubberLine, true);
		break;
	case Input_OtherPoints:	
		{
			myGeom2d_BezierCurve->RemovePole (IndexCounter);
			myGeom_BezierCurve->RemovePole (IndexCounter);
			BRepBuilderAPI_MakeEdge ME (myGeom_BezierCurve,myFirstgp_Pnt,tempPnt);
			if (ME.IsDone ())
			{
				curEdge = ME.Edge();
				IndexCounter--;
				closeBezierCurve();
			}
		}
		break;
	default:break;
 }
 myBezierCurveAction = Nothing;
}

/**
 * \fn GetTypeOfMethod()
 * \brief get command Method
 * \return Sketcher_ObjectTypeOfMethod 
 */
Sketcher_ObjectTypeOfMethod Sketcher_CommandBezierCurve::GetTypeOfMethod()
{
	return BezierCurve_Method;
}

/**
 * \fn closeBezierCurve()
 * \brief display Bezier Curve, add to list of objects
 * \return void 
 */
void Sketcher_CommandBezierCurve::closeBezierCurve()
{
	myContext->Remove(myRubberAIS_Shape, false);
	Handle (AIS_Shape) myAIS_Shape = new AIS_Shape(curEdge);
	AddObject(myGeom2d_BezierCurve,myAIS_Shape,CurveSketcherObject);

	myContext->Display(myAIS_Shape, true);

	for(Standard_Integer i = IndexCounter; i>2;i--)
	{
		myGeom2d_BezierCurve->RemovePole (i);
		myGeom_BezierCurve->RemovePole (i);
	}

	myBezierCurveAction = Input_1Point;
}

/**
 * \fn storePoles()
 * \brief display Pole point, add to list of objects
 * \return void 
 */
void Sketcher_CommandBezierCurve::storePoles()
{
	Handle (Geom2d_CartesianPoint) myGeom2d_Point = new Geom2d_CartesianPoint(curPnt2d);

	mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
	Handle(AIS_Point) myAIS_Point = new AIS_Point(mySecondPoint);
	myContext->Display(myAIS_Point, true);

	AddObject(myGeom2d_Point,myAIS_Point,PointSketcherObject);
}


