/**
* \file Sketcher_CommandCircleCenterRadius.cxx
* \brief Implementation file for the class Sketcher_CommandCircleCenterRadius
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandCircleCenterRadius.cxx">Sergei Maslov</a>
*/
#include "Sketcher_CommandCircleCenterRadius.hxx"

IMPLEMENT_STANDARD_RTTIEXT(Sketcher_CommandCircleCenterRadius, Sketcher_Command)

/**
 * \fn Sketcher_CommandCircleCenterRadius() 
 * \brief Constructs a Sketcher_CommandCircleCenterRadius
 */
Sketcher_CommandCircleCenterRadius::Sketcher_CommandCircleCenterRadius()
: Sketcher_Command("CircleCR."),
  myCircleAx2d(gp::Origin2d(),gp::DX2d())
{
	myCircleCenterRadiusAction = Nothing;
	radius = 0;

	tempGeom_Circle = new Geom_Circle(curCoordinateSystem.Ax2(),SKETCHER_RADIUS);
	myRubberCircle = new AIS_Circle(tempGeom_Circle);
	myRubberCircle->SetColor(Quantity_NOC_LIGHTPINK1);
}


/**
 * \fn ~Sketcher_CommandCircleCenterRadius() 
 * \brief destructor
 */
Sketcher_CommandCircleCenterRadius::~Sketcher_CommandCircleCenterRadius()
{
}

/**
 * \fn Action() 
 * \brief turn command to active state 
 */
void Sketcher_CommandCircleCenterRadius::Action()
{
	myCircleCenterRadiusAction = Input_CenterPoint;
	tempGeom_Circle->SetAxis(curCoordinateSystem.Axis());
	myCircleAx2d.SetDirection(gp_Dir2d (curCoordinateSystem.XDirection().X(),curCoordinateSystem.XDirection().Y()));
}


/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d )
 * \brief input event handler
 * \return Standard_Boolean 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_Boolean Sketcher_CommandCircleCenterRadius::MouseInputEvent(const gp_Pnt2d& thePnt2d)
{
 switch (myCircleCenterRadiusAction)
 {
	case Nothing:	break;

	case Input_CenterPoint:
			curPnt2d = myAnalyserSnap->MouseInput(thePnt2d);
			myFirstgp_Pnt2d = curPnt2d;
			myFirstPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));///////

			myCircleAx2d.SetLocation(myFirstgp_Pnt2d);
			tempGeom_Circle->SetLocation(myFirstPoint->Pnt());
			tempGeom_Circle->SetRadius(SKETCHER_RADIUS);
			myRubberCircle->SetCircle(tempGeom_Circle);
			myContext->Display(myRubberCircle,0,-1, true);

//			myRubberLine->SetPoints(myFirstPoint,myFirstPoint);
//			myContext->Display(myRubberLine,0,-1);
			firstPoint=true;

			myCircleCenterRadiusAction = Input_RadiusPoint;
			break;

	case Input_RadiusPoint:
			{
				curPnt2d = myAnalyserSnap->MouseInputException(myFirstgp_Pnt2d,thePnt2d,Circle_CenterPnt,Standard_True);
				radius = myFirstgp_Pnt2d.Distance(curPnt2d);
				Handle(Geom2d_Circle)	myGeom2d_Circle = new Geom2d_Circle(myCircleAx2d,radius);

				Handle(Geom_Circle) Geom_Circle1 = new Geom_Circle(ElCLib::To3d(curCoordinateSystem.Ax2(),myGeom2d_Circle->Circ2d()));
				Handle(AIS_Circle) myAIS_Circle = new AIS_Circle(Geom_Circle1);
				AddObject(myGeom2d_Circle,myAIS_Circle,CircleSketcherObject);

				myContext->Remove(myRubberCircle, false);
				myContext->Remove(myRubberLine, false);
				myContext->Display(myAIS_Circle, true);

				myCircleCenterRadiusAction = Input_CenterPoint;
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
void Sketcher_CommandCircleCenterRadius::MouseMoveEvent(const gp_Pnt2d& thePnt2d)
{
 switch (myCircleCenterRadiusAction)
 {
	case Nothing:break;

	case Input_CenterPoint: 
			curPnt2d = myAnalyserSnap->MouseMove(thePnt2d);
			break;

	case Input_RadiusPoint:
			curPnt2d = myAnalyserSnap->MouseMoveException(myFirstgp_Pnt2d,thePnt2d,Circle_CenterPnt,Standard_True);
			radius = myFirstgp_Pnt2d.Distance(curPnt2d);
			tempGeom_Circle->SetRadius(radius);
//			myRubberCircle->SetCircle (tempGeom_Circle);
			myContext->Redisplay(myRubberCircle, false);

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
	default:break;
 }
}


/**
 * \fn CancelEvent()
 * \brief cancel event handler, stop entering object
 * \return void 
 */
void Sketcher_CommandCircleCenterRadius::CancelEvent()
{
 switch (myCircleCenterRadiusAction)
 {
	case Nothing:	break;

	case Input_CenterPoint: break;

	case Input_RadiusPoint:	myContext->Remove(myRubberCircle, false);
							myContext->Remove(myRubberLine, true);////////////////////////
							break;
	default: break;
 }
 myCircleCenterRadiusAction = Nothing;
}


/**
 * \fn GetTypeOfMethod()
 * \brief get command Method
 * \return Sketcher_ObjectTypeOfMethod 
 */
Sketcher_ObjectTypeOfMethod Sketcher_CommandCircleCenterRadius::GetTypeOfMethod()
{
	return CircleCenterRadius_Method;
}

