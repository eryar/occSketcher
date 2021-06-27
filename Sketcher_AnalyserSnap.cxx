/**
* \file Sketcher_AnalyserSnap.cxx
* \brief Implementation file for the class Sketcher_AnalyserSnap
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_AnalyserSnap.cxx">Sergei Maslov</a>
*/
#include "Sketcher_AnalyserSnap.hxx"

IMPLEMENT_STANDARD_RTTIEXT(Sketcher_AnalyserSnap, Standard_Transient)

/**
 * \fn Sketcher_AnalyserSnap(Handle(AIS_InteractiveContext)& theContext,Handle(TColStd_HSequenceOfTransient)& thedata,gp_Ax3 &theAx3) 
 * \brief Constructs a Sketcher_AnalyserSnap
 * \param theContext Handle(AIS_InteractiveContext)&
 * \param thedata Handle(TColStd_HSequenceOfTransient)&
 * \param theAx3 gp_Ax3&
 */
Sketcher_AnalyserSnap::Sketcher_AnalyserSnap(Handle(AIS_InteractiveContext)& theContext,
										 Handle(TColStd_HSequenceOfTransient)& thedata,
										 gp_Ax3 &theAx3) 
:  curCoordinateSystem(gp::XOY())
{
	myContext = theContext;
	data = thedata;
	curCoordinateSystem = theAx3;

	myCurrentSnap = SnapNothing;
	mySnapAnType = SnapNothing;
	myPnt2d = gp::Origin2d();

	minimumSnapDistance = MINIMUMSNAP;
	bestDist = 0;
	curDist = 0;

	storedPnt2d = gp::Origin2d();
	storedTangentType = NothingTangent;
	isTangent	= Standard_False;
	isLine		= Standard_False;

	mySnaps	= new TColStd_HSequenceOfTransient();
	addSnap(new Sketcher_SnapEnd());
	addSnap(new Sketcher_SnapMiddle());
	addSnap(new Sketcher_SnapCenter());
	addSnap(new Sketcher_SnapNearest());
	addSnap(new Sketcher_SnapIntersection());
	addSnap(new Sketcher_SnapLineParallel());
	addSnap(new Sketcher_SnapLinePerpendicular());
	addSnap(new Sketcher_SnapTangent());
}


/**
 * \fn ~Sketcher_AnalyserSnap()
 * \brief destructor
 */
Sketcher_AnalyserSnap::~Sketcher_AnalyserSnap()
{
}

/**
 * \fn SetContext(Handle(AIS_InteractiveContext)& theContext)
 * \brief set context
 * \return void 
 * \param theContext Handle(AIS_InteractiveContext)&
 */
void Sketcher_AnalyserSnap::SetContext (Handle(AIS_InteractiveContext)& theContext)
{
	myContext = theContext;
	for( Standard_Integer i=1; i<=mySnaps->Length(); i++)
	{
		CurSnap = Handle(Sketcher_Snap)::DownCast(mySnaps->Value(i));
		CurSnap->SetContext(myContext);
	}
}

/**
 * \fn SetData(Handle(TColStd_HSequenceOfTransient)& thedata)
 * \brief set list of objects
 * \return void 
 * \param thedata TColStd_HSequenceOfTransient)&
 */
void Sketcher_AnalyserSnap::SetData (Handle(TColStd_HSequenceOfTransient)& thedata) 
{
	data = thedata;
	for( Standard_Integer i=1; i<=mySnaps->Length(); i++)
	{
		CurSnap = Handle(Sketcher_Snap)::DownCast(mySnaps->Value(i));
		CurSnap->SetData(data);
	}

}

/**
 * \fn SetAx3(const gp_Ax3& theAx3)
 * \brief set coordinate system
 * \return void 
 * \param theAx3 const gp_Ax3&
 */
void Sketcher_AnalyserSnap::SetAx3(const gp_Ax3& theAx3)
{
	curCoordinateSystem = theAx3;
	for( Standard_Integer i=1; i<=mySnaps->Length(); i++)
	{
		CurSnap = Handle(Sketcher_Snap)::DownCast(mySnaps->Value(i));
		CurSnap->SetAx3(curCoordinateSystem);
	}
}

/**
 * \fn SetMinDistance(const Standard_Real& aPrecise)
 * \brief set precise for snap
 * \return void 
 * \param aPrecise const Standard_Real&
 */
void Sketcher_AnalyserSnap::SetMinDistance(const Standard_Real& aPrecise)
{
	minimumSnapDistance = aPrecise;
	for( Standard_Integer i=1; i<=mySnaps->Length(); i++)
	{
		CurSnap = Handle(Sketcher_Snap)::DownCast(mySnaps->Value(i));
		CurSnap->SetMinDistance(aPrecise);
	}
}

/**
 * \fn SetSnapType(Sketcher_SnapType theSnap)
 * \brief set current snap type
 * \return void 
 * \param theSnap Sketcher_SnapType
 */
void Sketcher_AnalyserSnap::SetSnapType(Sketcher_SnapType theSnap)
{
	Cancel();
	myCurrentSnap = theSnap;
}

/**
 * \fn GetSnapType()
 * \brief get current snap type
 * \return Sketcher_SnapType 
 */
Sketcher_SnapType Sketcher_AnalyserSnap::GetSnapType()
{
	return myCurrentSnap;
}

/**
 * \fn MouseInput(const gp_Pnt2d& thePnt2d)
 * \brief input event handler
 * \return gp_Pnt2d 
 * \param thePnt2d const gp_Pnt2d&
 */
gp_Pnt2d Sketcher_AnalyserSnap::MouseInput(const gp_Pnt2d& thePnt2d)
{
	myPnt2d = thePnt2d;
	switch (myCurrentSnap)
	{
	case SnapEnd:		
	case SnapMiddle:	
	case SnapCenter:	
	case SnapIntersection:		
	case SnapNearest:		
		SelectCurSnap();
		myPnt2d = CurSnap->MouseInputEvent(thePnt2d);
		break;
	case SnapTangent:
		if(isTangent)
		{
			SelectCurSnap();
			CurSnap->setFirstPnt(storedPnt2d,storedTangentType);
			myPnt2d = CurSnap->MouseInputEvent(thePnt2d);
		}
		break;
	case SnapParallel:	
	case SnapPerpendicular: 
		if(isLine)
		{
			SelectCurSnap();
			CurSnap->setFirstPnt(storedPnt2d);
			myPnt2d = CurSnap->MouseInputEvent(thePnt2d);
		}
		break;
	case SnapAnalyse:
		SnapAnalyserEvent();
		CurSnap->EraseSnap();
		mySnapAnType = SnapNothing;
		break;
	default: break;
	}		
	return myPnt2d;
}

/**
 * \fn MouseMove(const gp_Pnt2d& thePnt2d)
 * \brief mouse move handler
 * \return gp_Pnt2d 
 * \param thePnt2d const gp_Pnt2d&
 */
gp_Pnt2d Sketcher_AnalyserSnap::MouseMove(const gp_Pnt2d& thePnt2d)
{
	myPnt2d = thePnt2d;
	switch (myCurrentSnap)
	{
	case SnapEnd:		
	case SnapMiddle:	
	case SnapCenter:	
	case SnapIntersection:		
	case SnapNearest:		
		SelectCurSnap();
		myPnt2d = CurSnap->MouseMoveEvent(thePnt2d);
		break;
	case SnapTangent:		
		if(isTangent)
		{
			SelectCurSnap();
			CurSnap->setFirstPnt(storedPnt2d,storedTangentType);
			myPnt2d = CurSnap->MouseMoveEvent(thePnt2d);
		}
		break;
	case SnapParallel:	
	case SnapPerpendicular: 
		if(isLine)
		{
			SelectCurSnap();
			CurSnap->setFirstPnt(storedPnt2d);
			myPnt2d = CurSnap->MouseMoveEvent(thePnt2d);
		}
		break;
	case SnapAnalyse:	
		{
			SnapAnalyserEvent();
			if(mySnapAnType != curSnapAnType)
			{
				mySnapAnType = curSnapAnType;
				CurSnap->EraseSnap();
			}
			for (Standard_Integer k=1; k<=mySnaps->Length(); k++)
			{
				CurSnap = Handle(Sketcher_Snap)::DownCast(mySnaps->Value(k));
				if(CurSnap->GetSnapType() == mySnapAnType)
				{
					myPnt2d = CurSnap->MouseMoveEvent(thePnt2d);
					break;
				}
			}
		}
		break;
	default:break;
	}		

	return myPnt2d;
}

/**
 * \fn Cancel()
 * \brief cancel event handler
 * \return void 
 */
void Sketcher_AnalyserSnap::Cancel()
{
	SelectCurSnap();
	CurSnap->EraseSnap();
	mySnapAnType = SnapNothing;
	storedTangentType = NothingTangent;
	isTangent	= Standard_False;
	isLine		= Standard_False;
}

/**
 * \fn MouseInputException(const gp_Pnt2d& p1, const gp_Pnt2d& thePnt2d, TangentType CType, Standard_Boolean TangentOnly)
 * \brief input event exception handler
 * \return gp_Pnt2d
 * \param p1 const gp_Pnt2d&
 * \param thePnt2d const gp_Pnt2d&
 * \param CType TangentType
 * \param TangentOnly Standard_Boolean
 */
gp_Pnt2d Sketcher_AnalyserSnap::MouseInputException(const gp_Pnt2d& p1, const gp_Pnt2d& thePnt2d, TangentType CType, Standard_Boolean TangentOnly)
{
	if (myCurrentSnap == SnapAnalyse || myCurrentSnap ==  SnapParallel || myCurrentSnap ==  SnapPerpendicular || myCurrentSnap == SnapTangent) 
	{
		storedPnt2d = p1;
		storedTangentType = CType;
		isTangent = Standard_True;
		if (!TangentOnly) isLine = Standard_True;

		MouseInput(thePnt2d);

		isTangent = Standard_False;
		isLine = Standard_False;
		return myPnt2d;
	}
	else return MouseInput(thePnt2d);
}


/**
 * \fn MouseMoveException(const gp_Pnt2d& p1, const gp_Pnt2d& thePnt2d, TangentType CType, Standard_Boolean TangentOnly)
 * \brief mouse move exception handler
 * \return gp_Pnt2d
 * \param p1 const gp_Pnt2d&
 * \param thePnt2d const gp_Pnt2d&
 * \param CType TangentType
 * \param TangentOnly Standard_Boolean
 */
gp_Pnt2d Sketcher_AnalyserSnap::MouseMoveException(const gp_Pnt2d& p1, const gp_Pnt2d& thePnt2d, TangentType CType, Standard_Boolean TangentOnly)
{
	if (myCurrentSnap == SnapAnalyse || myCurrentSnap ==  SnapParallel || myCurrentSnap ==  SnapPerpendicular || myCurrentSnap == SnapTangent) 
	{
		storedPnt2d = p1;
		storedTangentType = CType;
		isTangent = Standard_True;
		if (!TangentOnly) isLine = Standard_True;

		MouseMove(thePnt2d);

		isTangent = Standard_False;
		isLine = Standard_False;
		return myPnt2d;
	}
	else return MouseMove(thePnt2d);
}

/**
 * \fn SnapAnalyserEvent()
 * \brief search best point among all possible snaps
 * \return void 
*/
void Sketcher_AnalyserSnap::SnapAnalyserEvent()
{
	bestDist = minimumSnapDistance*10;
	curDist = 0;

	mySeqOfPnt2d.Clear();
	mySeqOfDistance.Clear();
	mySeqOfFactor.Clear();
	mySnapType.Clear();
	for (Standard_Integer i=1; i<=mySnaps->Length(); i++)
	{
		CurSnap = Handle(Sketcher_Snap)::DownCast(mySnaps->Value(i));
		switch (CurSnap->GetSnapType())
		{
		case SnapEnd:		AddPoints(1);
							break;
		case SnapMiddle:	AddPoints(3.5);
							break;
		case SnapCenter:	AddPoints(4);
							break;
		case SnapIntersection:
							AddPoints(1);
							break;
		case SnapNearest:	AddPoints(8);
							break;
		case SnapTangent:	if(isTangent)
							{
								CurSnap->setFirstPnt(storedPnt2d,storedTangentType);
								AddPoints(1);
							}
							break;
		case SnapParallel:	
		case SnapPerpendicular: if(isLine)
								{
									CurSnap->setFirstPnt(storedPnt2d);
									AddPoints(8);
								}
								break;
		default:break;
		}
	}

	curSnapAnType = SnapNothing;
	for (Standard_Integer j=1; j<=mySeqOfPnt2d.Length(); j++)
	{
		curDist = mySeqOfDistance(j) * mySeqOfFactor(j);
		if (bestDist > curDist)
		{
			bestDist = curDist;
			myPnt2d = mySeqOfPnt2d(j);
			curSnapAnType = (Sketcher_SnapType)mySnapType(j);
		}
	}

	for (Standard_Integer k=1; k<=mySnaps->Length(); k++)
	{
		CurSnap = Handle(Sketcher_Snap)::DownCast(mySnaps->Value(k));
		if(CurSnap->GetSnapType() == mySnapAnType)
			break;
	}
}

/**
 * \fn AddPoints(const Standard_Real& factor)
 * \brief add point to sequence
 * \return void 
 * \param factor const Standard_Real&
*/
void Sketcher_AnalyserSnap::AddPoints(const Standard_Real& factor)
{
	gp_Pnt2d pt;
	Standard_Real distance = 0;
	Standard_Integer snaptype = 0;
	if(CurSnap->AnalyserEvent(myPnt2d,pt,distance,snaptype))
	{
		mySeqOfPnt2d.Append(pt);
		mySeqOfDistance.Append(distance);
		mySeqOfFactor.Append(factor);
		mySnapType.Append(snaptype);
	}
}

/**
 * \fn addSnap(Handle(Sketcher_Snap) theSnap)
 * \brief add theSnap to snap list
 * \return void
 * \param theSnap andle(Sketcher_Snap)
 */
void Sketcher_AnalyserSnap::addSnap(Handle(Sketcher_Snap) theSnap)
{
	theSnap->SetData(data);
	theSnap->SetContext(myContext);
	theSnap->SetAx3(curCoordinateSystem);
	mySnaps->Append(theSnap);
}

/**
 * \fn SelectCurSnap()
 * \brief get snap from snap list
 * \return void
 */
void Sketcher_AnalyserSnap::SelectCurSnap ()
{
	for (Standard_Integer i=1; i<=mySnaps->Length(); i++)
	{
		CurSnap = Handle(Sketcher_Snap)::DownCast(mySnaps->Value(i));
		if(CurSnap->GetSnapType() == myCurrentSnap)
			break;
	}
}

