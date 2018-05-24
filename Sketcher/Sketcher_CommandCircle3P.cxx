/**
* \file Sketcher_CommandCircle3P.cxx
* \brief Implementation file for the class Sketcher_CommandCircle3P
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandCircle3P.cxx">Sergei Maslov</a>
*/
#include "Sketcher_CommandCircle3P.hxx"

/**
 * \fn Sketcher_CommandCircle3P() 
 * \brief Constructs a Sketcher_CommandCircle3P
 */
Sketcher_CommandCircle3P::Sketcher_CommandCircle3P()
: Sketcher_Command("Circle3P."),
  tempCirc(curCoordinateSystem.Ax2(),SKETCHER_RADIUS)
{
	myCircle3PAction = Nothing;

	tempGeom_Circle = new Geom_Circle(curCoordinateSystem.Ax2(),SKETCHER_RADIUS);
	myRubberCircle = new AIS_Circle(tempGeom_Circle);
	myRubberCircle->SetColor(Quantity_NOC_LIGHTPINK1);

	mySecondgp_Pnt2d = gp::Origin2d();
	third_Pnt = gp::Origin();
}


/**
 * \fn ~Sketcher_CommandCircle3P() 
 * \brief destructor
 */
Sketcher_CommandCircle3P::~Sketcher_CommandCircle3P()
{
}

/**
 * \fn Action() 
 * \brief turn command to active state 
 */
void Sketcher_CommandCircle3P::Action()
{
	myCircle3PAction = Input_1CirclePoint;
	tempCirc.SetPosition (curCoordinateSystem.Ax2());
}
			
			
/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d )
 * \brief input event handler
 * \return Standard_Boolean 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_Boolean Sketcher_CommandCircle3P::MouseInputEvent(const gp_Pnt2d& thePnt2d)
{
 curPnt2d = myAnalyserSnap->MouseInput(thePnt2d);
 switch (myCircle3PAction)
 {
	case Nothing:	break;

	case Input_1CirclePoint:
		myFirstgp_Pnt2d = curPnt2d;
		myFirstPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));

		myRubberLine->SetPoints(myFirstPoint,myFirstPoint);
		myContext->Display(myRubberLine,0,-1);

		myCircle3PAction = Input_2CirclePoint;
		break;

	case Input_2CirclePoint:
		{
			mySecondgp_Pnt2d = curPnt2d;
			mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
			tempCirc.SetLocation(myFirstPoint->Pnt().Scaled(mySecondPoint->Pnt(),0.5));
			tempCirc.SetRadius(myFirstgp_Pnt2d.Distance(curPnt2d) / 2);

			tempGeom_Circle->SetCirc(tempCirc);
			myRubberCircle->SetCircle(tempGeom_Circle);

			myContext->Remove(myRubberLine);
			myContext->Display(myRubberCircle,0,-1);
			myContext->Redisplay(myRubberCircle);

			myCircle3PAction = Input_3CirclePoint;
		}
		break;
	case Input_3CirclePoint:
		{
			gce_MakeCirc2d tempMakeCirc2d(myFirstgp_Pnt2d,mySecondgp_Pnt2d,curPnt2d);
			if (tempMakeCirc2d.Status() == gce_Done) 
			{
				Handle(Geom2d_Circle) myGeom2d_Circle = new Geom2d_Circle(tempMakeCirc2d.Value());

				Handle(Geom_Circle) Geom_Circle1 = new Geom_Circle(ElCLib::To3d(curCoordinateSystem.Ax2(),myGeom2d_Circle->Circ2d()));
				Handle(AIS_Circle) myAIS_Circle = new AIS_Circle(Geom_Circle1);
				AddObject(myGeom2d_Circle,myAIS_Circle,CircleSketcherObject);

				myContext->Remove(myRubberCircle);
				myContext->Display(myAIS_Circle);
				myCircle3PAction = Input_1CirclePoint;
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
void Sketcher_CommandCircle3P::MouseMoveEvent(const gp_Pnt2d& thePnt2d)
{
 curPnt2d = myAnalyserSnap->MouseMove(thePnt2d);
 switch (myCircle3PAction)
 {
	case Nothing:break;

	case Input_1CirclePoint: break;

	case Input_2CirclePoint:
		mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
		myRubberLine->SetPoints(myFirstPoint,mySecondPoint);
		myContext->Redisplay(myRubberLine);
		break;
	case Input_3CirclePoint:
		{
			third_Pnt =	ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d);
			gce_MakeCirc tempMakeCirc = gce_MakeCirc(myFirstPoint->Pnt(),mySecondPoint->Pnt(),third_Pnt);
			if (tempMakeCirc.Status() == gce_Done) 
			{
				tempGeom_Circle->SetCirc(tempMakeCirc.Value());
//				myRubberCircle->SetCircle(tempGeom_Circle);
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
void Sketcher_CommandCircle3P::CancelEvent()
{
 switch (myCircle3PAction)
 {
	case Nothing:	break;

	case Input_1CirclePoint: break;

	case Input_2CirclePoint: myContext->Remove(myRubberLine);
							 break;
	case Input_3CirclePoint: myContext->Remove(myRubberCircle);
							 break;
	default: break;
 }
 myCircle3PAction = Nothing;
}

/**
 * \fn GetTypeOfMethod()
 * \brief get command Method
 * \return Sketcher_ObjectTypeOfMethod 
 */
Sketcher_ObjectTypeOfMethod Sketcher_CommandCircle3P::GetTypeOfMethod()
{
	return Circle3P_Method;
}


IMPLEMENT_STANDARD_HANDLE(Sketcher_CommandCircle3P,Sketcher_Command)
IMPLEMENT_STANDARD_RTTI(Sketcher_CommandCircle3P,Sketcher_Command)

IMPLEMENT_STANDARD_TYPE(Sketcher_CommandCircle3P)
IMPLEMENT_STANDARD_SUPERTYPE(Sketcher_Command)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Sketcher_Command)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_CommandCircle3P)
