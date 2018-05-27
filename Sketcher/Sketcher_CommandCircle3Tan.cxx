/**
* \file Sketcher_CommandCircle3Tan.cxx
* \brief Implementation file for the class Sketcher_CommandCircle3Tan
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_CommandCircle3Tan.cxx">Sergei Maslov</a>
*/
#include "Sketcher_CommandCircle3Tan.hxx"

/**
 * \fn Sketcher_CommandCircle3Tan() 
 * \brief Constructs a Sketcher_CommandCircle3Tan
 */
Sketcher_CommandCircle3Tan::Sketcher_CommandCircle3Tan()
: Sketcher_Command("Circle3Tan."),
  temp_Circ2d(gp::OX2d(),SKETCHER_RADIUS),
  best_Circ(gp::XOY(),SKETCHER_RADIUS)
{
	myCircle3TanAction = Nothing;

	minDistance = 0;
	curDistance = 0;
	u1 = u2 = u_temp = 0;
	mySecondgp_Pnt2d = gp::Origin2d();

	tempGeom2d_Circle = new Geom2d_Circle(temp_Circ2d);
	tempGeom_Circle = new Geom_Circle(best_Circ);
	myRubberCircle = new AIS_Circle(tempGeom_Circle);
	myRubberCircle->SetColor(Quantity_NOC_LIGHTPINK1);

	gp_lin1Exist = gp_lin2Exist = Standard_False;
	gp_circ1Exist = gp_circ2Exist = Standard_False;
}


/**
 * \fn ~Sketcher_CommandCircle3Tan() 
 * \brief destructor
 */
Sketcher_CommandCircle3Tan::~Sketcher_CommandCircle3Tan()
{
}

/**
 * \fn Action() 
 * \brief turn command to active state 
 */
void Sketcher_CommandCircle3Tan::Action()
{
	myCircle3TanAction = Input_1CircleTan;
}

/**
 * \fn MouseInputEvent(const gp_Pnt2d& thePnt2d )
 * \brief input event handler
 * \return Standard_Boolean 
 * \param thePnt2d const gp_Pnt2d&
 */
Standard_Boolean Sketcher_CommandCircle3Tan::MouseInputEvent(const gp_Pnt2d& thePnt2d)
{
 curPnt2d = myAnalyserSnap->MouseInput(thePnt2d);
 switch (myCircle3TanAction)
 {
	case Nothing:	break;

	case Input_1CircleTan:
			SelectCurve();
			if (StoregpObject()) 
			{
				u1 = u_temp;
				myFirstgp_Pnt2d = tempPnt2d;
				myFirst2d_Curve = tempGeom2d_Curve;
				myFirstPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),tempPnt2d));

				myRubberLine->SetPoints(myFirstPoint,myFirstPoint);
				myContext->Display(myRubberLine,0,-1);

				myCircle3TanAction = Input_2CircleTan;
			}
			break;

	case Input_2CircleTan:
			SelectCurve();
			if (StoregpObject()) 
			{
				u2 = u_temp;
				mySecondgp_Pnt2d = tempPnt2d;
				mySecond2d_Curve = tempGeom2d_Curve;
				mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),tempPnt2d));

				tempGeom_Circle->SetRadius(SKETCHER_RADIUS);
				myRubberCircle->SetCircle(tempGeom_Circle);

				myContext->Display(myRubberCircle,0,-1);
				myContext->Redisplay(myRubberCircle);

				myCircle3TanAction = Input_3CircleTan;
			}
			break;
	case Input_3CircleTan:
			SelectCurve();
			if (CheckgpObject()) 
			{
			Geom2dAdaptor_Curve FirstAdaptor_Curve(myFirst2d_Curve);
			Geom2dAdaptor_Curve SecondAdaptor_Curve(mySecond2d_Curve);
			Geom2dAdaptor_Curve ThirdAdaptor_Curve(tempGeom2d_Curve);

			Geom2dGcc_QualifiedCurve FirstQualifiedCurve (FirstAdaptor_Curve,GccEnt_unqualified);
			Geom2dGcc_QualifiedCurve SecondQualifiedCurve (SecondAdaptor_Curve,GccEnt_unqualified);
			Geom2dGcc_QualifiedCurve ThirdQualifiedCurve (ThirdAdaptor_Curve,GccEnt_unqualified);
				
			Geom2dGcc_Circ2d3Tan myGcc_Circ2d3Tan(FirstQualifiedCurve,SecondQualifiedCurve,ThirdQualifiedCurve,1.0e-12,u1,u2,u_temp);
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

					gp_lin1Exist = gp_lin2Exist = Standard_False;
					gp_circ1Exist = gp_circ2Exist = Standard_False;
					myCircle3TanAction = Input_1CircleTan;
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
void Sketcher_CommandCircle3Tan::MouseMoveEvent (const gp_Pnt2d& thePnt2d)
{
 curPnt2d = myAnalyserSnap->MouseMove(thePnt2d);
 switch (myCircle3TanAction)
 {
	case Nothing:break;

	case Input_1CircleTan:  
			curPnt2d = myAnalyserSnap->MouseMove(thePnt2d);
			break;

	case Input_2CircleTan:	
			SelectCurve();
			if (minDistance != MINIMUMDISTANCETOCURVE) 
			{
				mySecondPoint->SetPnt(ElCLib::To3d(curCoordinateSystem.Ax2(),tempPnt2d));
				myRubberLine->SetPoints(myFirstPoint,mySecondPoint);
				myContext->Redisplay(myRubberLine);
			}
			break;
	case Input_3CircleTan:	
			SelectCurve();
			if (CheckgpObject()) 
			{
				Geom2dAdaptor_Curve FirstAdaptor_Curve(myFirst2d_Curve);
				Geom2dAdaptor_Curve SecondAdaptor_Curve(mySecond2d_Curve);
				Geom2dAdaptor_Curve ThirdAdaptor_Curve(tempGeom2d_Curve);

				Geom2dGcc_QualifiedCurve FirstQualifiedCurve (FirstAdaptor_Curve,GccEnt_unqualified);
				Geom2dGcc_QualifiedCurve SecondQualifiedCurve (SecondAdaptor_Curve,GccEnt_unqualified);
				Geom2dGcc_QualifiedCurve ThirdQualifiedCurve (ThirdAdaptor_Curve,GccEnt_unqualified);
				
				Geom2dGcc_Circ2d3Tan myGcc_Circ2d3Tan(FirstQualifiedCurve,SecondQualifiedCurve,ThirdQualifiedCurve,1.0e-6,u1,u2,u_temp);
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
						myRubberCircle->SetCircle(tempGeom_Circle);
						myContext->Redisplay(myRubberCircle);
					}
				}
				else 
				{
					tempGeom_Circle->SetRadius(0);
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
void Sketcher_CommandCircle3Tan::CancelEvent()
{
 switch (myCircle3TanAction)
 {
	case Nothing:	break;

	case Input_1CircleTan:	break;

	case Input_2CircleTan: 	myContext->Remove(myRubberLine);
							break;
	case Input_3CircleTan: 	myContext->Remove(myRubberLine);
							myContext->Remove(myRubberCircle);
							break;
	default: break;
 }
 gp_lin1Exist = gp_lin2Exist = Standard_False;
 gp_circ1Exist = gp_circ2Exist = Standard_False;
 myCircle3TanAction = Nothing;
}

/**
 * \fn GetTypeOfMethod()
 * \brief get command Method
 * \return Sketcher_ObjectTypeOfMethod 
 */
Sketcher_ObjectTypeOfMethod Sketcher_CommandCircle3Tan::GetTypeOfMethod()
{
	return Circle3Tan_Method;
}

/**
 * \fn SelectCurve()
 * \brief select nearest curve
 * \return void
 */
void Sketcher_CommandCircle3Tan::SelectCurve()
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
					Handle (Geom2d_Line) temp_Line = new Geom2d_Line(curGeom2d_Edge->Lin2d());
					tempGeom2d_Curve = temp_Line;
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
					Handle (Geom2d_Circle) temp_Circle = new Geom2d_Circle(curGeom2d_Arc->Circ2d());
					tempGeom2d_Curve = temp_Circle;
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
Standard_Boolean Sketcher_CommandCircle3Tan::SumDistanceToCircle ()
{
	curDistance = 0;
	tempGeom2d_Circle->SetCirc2d(temp_Circ2d);
	myProjectOnCurve.Init(myFirstgp_Pnt2d,tempGeom2d_Circle);
	if(myProjectOnCurve.NbPoints() > 0)
		curDistance = myFirstgp_Pnt2d.Distance(myProjectOnCurve.NearestPoint());
	else 
		return Standard_False;

	myProjectOnCurve.Init(mySecondgp_Pnt2d,tempGeom2d_Circle);
	if(myProjectOnCurve.NbPoints() > 0)
		curDistance += mySecondgp_Pnt2d.Distance(myProjectOnCurve.NearestPoint());
	else 
		return Standard_False;

	myProjectOnCurve.Init(tempPnt2d,tempGeom2d_Circle);
	if(myProjectOnCurve.NbPoints() > 0)
		curDistance += tempPnt2d.Distance(myProjectOnCurve.NearestPoint());
	else 
		return Standard_False;

	return Standard_True;
}


/**
 * \fn CheckgpObject()
 * \brief check object, is it possible to create tangent circle
 * \return Standard_Boolean
 */
Standard_Boolean Sketcher_CommandCircle3Tan::CheckgpObject()
{
	if (minDistance != MINIMUMDISTANCETOCURVE) 
	{
		if (tempGeom2d_Curve->IsKind(STANDARD_TYPE(Geom2d_Line)))
		{
			Handle(Geom2d_Line) tnl = Handle(Geom2d_Line)::DownCast(tempGeom2d_Curve);
			if (gp_lin1Exist)
			{
				if (gp_lin1.Position().IsParallel(tnl->Lin2d().Position(),1.0e-12) && gp_lin1.Contains(tnl->Lin2d().Location(),1.0e-12)) 
					return Standard_False;
				else if(gp_lin2Exist) 
					 if (gp_lin2.Position().IsParallel(tnl->Lin2d().Position(),1.0e-12) && gp_lin2.Contains(tnl->Lin2d().Location(),1.0e-12)) 
						 return Standard_False;
			}
		}
		else if(tempGeom2d_Curve->IsKind(STANDARD_TYPE(Geom2d_Circle)))
		{
			Handle(Geom2d_Circle) tnc = Handle(Geom2d_Circle)::DownCast(tempGeom2d_Curve);
			if (gp_circ1Exist)
			{
				if(gp_circ1.Location().IsEqual(tnc->Circ2d().Location(),1.0e-12) && gp_circ1.Radius() == tnc->Circ2d().Radius()) 
					return Standard_False;
				else if(gp_circ2Exist) 
					 if(gp_circ2.Location().IsEqual(tnc->Circ2d().Location(),1.0e-12) && gp_circ2.Radius() == tnc->Circ2d().Radius()) 
						 return Standard_False;
			}
		}
		return Standard_True;
	}
	else return Standard_False;
}


/**
 * \fn StoregpObject()
 * \brief store objects for building tangent circle
 * \return Standard_Boolean
 */
Standard_Boolean Sketcher_CommandCircle3Tan::StoregpObject()
{
	if (minDistance != MINIMUMDISTANCETOCURVE) 
	{
		if (tempGeom2d_Curve->IsKind(STANDARD_TYPE(Geom2d_Line)))
		{
			Handle(Geom2d_Line) tnl = Handle(Geom2d_Line)::DownCast(tempGeom2d_Curve);
			if (!gp_lin1Exist)	
			{
				gp_lin1 = tnl->Lin2d();
				gp_lin1Exist = Standard_True;
			}
			else if (gp_lin1.Direction().IsParallel(tnl->Lin2d().Direction(),1.0e-12) && gp_lin1.Contains(tnl->Lin2d().Location(),1.0e-12)) 
					return Standard_False;
			else 
			{
				gp_lin2 = tnl->Lin2d();
				gp_lin2Exist = Standard_True;
			}
		}
		else if(tempGeom2d_Curve->IsKind(STANDARD_TYPE(Geom2d_Circle)))
		{
			Handle(Geom2d_Circle) tnc = Handle(Geom2d_Circle)::DownCast(tempGeom2d_Curve);
			if (!gp_circ1Exist)	
			{
				gp_circ1 = tnc->Circ2d();
				gp_circ1Exist = Standard_True;
			}
			else if(gp_circ1.Location().IsEqual(tnc->Circ2d().Location(),1.0e-12) && gp_circ1.Radius() == tnc->Circ2d().Radius()) 
					return Standard_False;
			else 
			{
				gp_circ2 = tnc->Circ2d();
				gp_circ2Exist = Standard_True;
			}
		 }
		 return Standard_True;
	}
	else return Standard_False;
}



IMPLEMENT_STANDARD_HANDLE(Sketcher_CommandCircle3Tan,Sketcher_Command)
IMPLEMENT_STANDARD_RTTI(Sketcher_CommandCircle3Tan)

IMPLEMENT_STANDARD_TYPE(Sketcher_CommandCircle3Tan)
IMPLEMENT_STANDARD_SUPERTYPE(Sketcher_Command)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Sketcher_Command)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_CommandCircle3Tan)
