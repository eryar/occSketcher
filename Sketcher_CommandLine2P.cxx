/**
* \file Sketcher_CommandLine2P.cxx
* \brief Implementation file for the class Sketcher_CommandLine2P
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandLine2P.cxx">Sergei Maslov</a>
*/
#include "Sketcher_CommandLine2P.hxx"

IMPLEMENT_STANDARD_RTTIEXT(Sketcher_CommandLine2P, Sketcher_Command)

/**
 * \fn Sketcher_CommandLine2P() 
 * \brief Constructs a Sketcher_CommandLine2P
 */
Sketcher_CommandLine2P::Sketcher_CommandLine2P()
: Sketcher_Command("Line2P.")
{
	myLine2PAction = Nothing;
}


/**
 * \fn ~Sketcher_CommandLine2P() 
 * \brief destructor
 */
Sketcher_CommandLine2P::~Sketcher_CommandLine2P()
{
}

/**
 * \fn Action() 
 * \brief turn command to active state 
 */
void Sketcher_CommandLine2P::Action()
{
	myLine2PAction = Input_FirstPointLine;
}

/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d )
 * \brief input event handler
 * \return Standard_Boolean 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_Boolean Sketcher_CommandLine2P::MouseInputEvent(const gp_Pnt2d& thePnt2d)
{
 switch (myLine2PAction)
 {
	case Nothing:	break;
 
	case Input_FirstPointLine:
			curPnt2d = myAnalyserSnap->MouseInputException(thePnt2d,thePnt2d,Line_FirstPnt,Standard_True);
			myFirstgp_Pnt2d = curPnt2d;
			myFirstPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
			
//			myRubberLine->SetPoints(myFirstPoint,myFirstPoint);
 
//			myContext->Display(myRubberLine,0,-1);
			myLine2PAction = Input_SecondPointLine;
			firstPoint = true;
			break;

	case Input_SecondPointLine:
			{
			curPnt2d = myAnalyserSnap->MouseInputException(myFirstgp_Pnt2d,thePnt2d,Line_SecondPnt,Standard_False);

			Handle(Geom2d_Edge) newGeom2d_Edge = new Geom2d_Edge();
			if(newGeom2d_Edge->SetPoints(myFirstgp_Pnt2d,curPnt2d))
			{
				Handle (Geom_CartesianPoint) Geom_Point1 = new Geom_CartesianPoint(ElCLib::To3d(curCoordinateSystem.Ax2(),myFirstgp_Pnt2d));
				Handle (Geom_CartesianPoint) Geom_Point2 = new Geom_CartesianPoint(ElCLib::To3d(curCoordinateSystem.Ax2(),curPnt2d));
				Handle(AIS_Line) myAIS_Line = new AIS_Line(Geom_Point1,Geom_Point2);
				AddObject(newGeom2d_Edge,myAIS_Line,LineSketcherObject);

				myContext->Display(myAIS_Line, true);
				if (myPolylineMode)
				{
					myFirstgp_Pnt2d = curPnt2d;
					myFirstPoint->SetPnt(mySecondPoint->Pnt());
					myRubberLine->SetPoints(myFirstPoint,myFirstPoint);
					myContext->Redisplay(myRubberLine, true);
				}
				else
				{
					myContext->Remove(myRubberLine, true);
					myLine2PAction = Input_FirstPointLine;
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
void Sketcher_CommandLine2P::MouseMoveEvent(const gp_Pnt2d& thePnt2d)
{
	
 switch (myLine2PAction)
 {
	case Nothing:break;

	case Input_FirstPointLine: 
			curPnt2d = myAnalyserSnap->MouseMoveException(thePnt2d,thePnt2d,Line_FirstPnt,Standard_True);
			break;
	case Input_SecondPointLine:
			curPnt2d = myAnalyserSnap->MouseMoveException(myFirstgp_Pnt2d,thePnt2d,Line_SecondPnt,Standard_False);
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
void Sketcher_CommandLine2P::CancelEvent()
{
 switch (myLine2PAction)
 {
	case Nothing:	break;

	case Input_FirstPointLine: 	break;

	case Input_SecondPointLine:	myContext->Remove(myRubberLine, true);
								break;
	default: break;
 }
 myLine2PAction = Nothing;
}

/**
 * \fn GetTypeOfMethod()
 * \brief get command Method
 * \return Sketcher_ObjectTypeOfMethod 
 */
Sketcher_ObjectTypeOfMethod Sketcher_CommandLine2P::GetTypeOfMethod()
{
	return Line2P_Method;
}


/**
 * \fn SetPolylineFirstPnt(const gp_Pnt2d& p1)
 * \brief set last entering gp_Pnt2d for polyline
 * \return void 
 * \param p1 const gp_Pnt2d&
 */
void  Sketcher_CommandLine2P::SetPolylineFirstPnt(const gp_Pnt2d& p1)
{
	myFirstgp_Pnt2d = p1;
	myFirstPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),p1));
	myRubberLine->SetPoints(myFirstPoint,myFirstPoint);
	myContext->Display(myRubberLine,0,-1, true);
	myLine2PAction = Input_SecondPointLine;
}


/**
 * \fn GetPolylineFirstPnt(gp_Pnt2d& p1)
 * \brief get last entering gp_Pnt2d for polyline
 * \return Standard_Boolean 
 * \param p1 gp_Pnt2d&
 */
Standard_Boolean  Sketcher_CommandLine2P::GetPolylineFirstPnt(gp_Pnt2d& p1)
{
 if (myLine2PAction == Input_SecondPointLine && myPolylineMode == Standard_True)
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
void Sketcher_CommandLine2P::SetPolylineMode (Standard_Boolean mode) 
{
	myPolylineMode = mode;
}


