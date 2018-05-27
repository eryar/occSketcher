/**
* \file Sketcher_SnapMiddle.cxx
* \brief Implementation file for the class Sketcher_SnapMiddle
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapMiddle.cxx">Sergei Maslov</a>
*/
#include "Sketcher_SnapMiddle.hxx"

/**
 * \fn Sketcher_SnapMiddle() 
 * \brief Constructs a Sketcher_SnapMiddle
 */
Sketcher_SnapMiddle::Sketcher_SnapMiddle()
{
}


/**
 * \fn ~Sketcher_SnapMiddle() 
 * \brief Destructor
 */
Sketcher_SnapMiddle::~Sketcher_SnapMiddle()
{
}

/**
 * \fn SelectEvent() 
 * \brief find point nearest to middle line
 * \return void 
 */
void Sketcher_SnapMiddle::SelectEvent()
{
  findbestPnt2d = Standard_False;
  minDistance = minimumSnapDistance;

  for( Standard_Integer i=1; i<=data->Length(); i++)
	{
		mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
		myGeometryType = mySObject->GetGeometryType();
		switch (myGeometryType)
		{
		case PointSketcherObject:	curGeom2d_Point = Handle(Geom2d_CartesianPoint)::DownCast(mySObject->GetGeometry());
									objectPnt2d = curGeom2d_Point->Pnt2d();
									if(count())
									{
										bestPnt2d = objectPnt2d;
										curHilightedObj = mySObject->GetAIS_Object();
									}
									break;
		case LineSketcherObject:	curGeom2d_Edge = Handle(Geom2d_Edge)::DownCast(mySObject->GetGeometry());
									ProjectOnCurve.Init(curPnt2d,curGeom2d_Edge,curGeom2d_Edge->StartParameter(),curGeom2d_Edge->EndParameter());
									if(countProject())
									{
										bestPnt2d = curGeom2d_Edge->MiddlePnt();
										curHilightedObj = mySObject->GetAIS_Object();
									}
									break;
		case CircleSketcherObject:	break;
		case ArcSketcherObject: 	curGeom2d_Arc = Handle(Geom2d_Arc)::DownCast(mySObject->GetGeometry());
									ProjectOnCurve.Init(curPnt2d,curGeom2d_Arc);
									if(countProject())
									{
										bestPnt2d = curGeom2d_Arc->MiddlePnt();
										curHilightedObj = mySObject->GetAIS_Object();
									}
									break;
		default:break;
		}
	}

  if(minDistance == minimumSnapDistance) 
		 bestPnt2d = curPnt2d;
  else   findbestPnt2d = Standard_True;

  minDistance = bestPnt2d.Distance(curPnt2d);
}


/**
 * \fn GetSnapType()
 * \brief get Snap type
 * \return Sketcher_SnapType 
 */
Sketcher_SnapType Sketcher_SnapMiddle::GetSnapType()
{
	return SnapMiddle;
}

IMPLEMENT_STANDARD_HANDLE(Sketcher_SnapMiddle,Sketcher_Snap)
IMPLEMENT_STANDARD_RTTI(Sketcher_SnapMiddle)

IMPLEMENT_STANDARD_TYPE(Sketcher_SnapMiddle)
IMPLEMENT_STANDARD_SUPERTYPE(Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_SnapMiddle)
