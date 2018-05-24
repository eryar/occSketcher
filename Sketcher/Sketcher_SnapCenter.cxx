/**
* \file Sketcher_SnapCenter.cxx
* \brief Implementation file for the class Sketcher_SnapCenter
* \author <a href="mailto:sergmaslov@istel.ru?subject=Sketcher_SnapCenter.cxx">Sergei Maslov</a>
*/
#include "Sketcher_SnapCenter.hxx"

/**
 * \fn Sketcher_SnapCenter() 
 * \brief Constructs a Sketcher_SnapCenter
 */
Sketcher_SnapCenter::Sketcher_SnapCenter()
{
}

/**
 * \fn ~Sketcher_SnapCenter() 
 * \brief Destructor
 */
Sketcher_SnapCenter::~Sketcher_SnapCenter()
{
}

/**
 * \fn SelectEvent() 
 * \brief find center point of arc, circle
 * \return void 
 */
void Sketcher_SnapCenter::SelectEvent()
{
  findbestPnt2d = Standard_False;
  minDistance = minimumSnapDistance;

  for( Standard_Integer i=1; i<=data->Length(); i++)
	{
		mySObject = Handle(Sketcher_Object)::DownCast(data->Value(i));
		myGeometryType = mySObject->GetGeometryType();
		switch (myGeometryType)
		{
		case PointSketcherObject:	break;
		case LineSketcherObject:	break;
		case CircleSketcherObject:	
		case ArcSketcherObject: 	curGeom2d_Circle = Handle(Geom2d_Circle)::DownCast(mySObject->GetGeometry());
									ProjectOnCurve.Init(curPnt2d,curGeom2d_Circle);
									if(countProject())
									{
										bestPnt2d = curGeom2d_Circle->Location();
										curHilightedObj = mySObject->GetAIS_Object();
									}
									objectPnt2d = curGeom2d_Circle->Location();
									if(count())
									{
										bestPnt2d = objectPnt2d;
										curHilightedObj = mySObject->GetAIS_Object();
									}
									break;
		default:break;
		}
	}

  if(minDistance == minimumSnapDistance) 
		 bestPnt2d = curPnt2d;
  else   findbestPnt2d = Standard_True;

}

/**
 * \fn GetSnapType()
 * \brief get Snap type
 * \return Sketcher_SnapType 
 */
Sketcher_SnapType Sketcher_SnapCenter::GetSnapType()
{
	return SnapCenter;
}

IMPLEMENT_STANDARD_HANDLE(Sketcher_SnapCenter,Sketcher_Snap)
IMPLEMENT_STANDARD_RTTI(Sketcher_SnapCenter,Sketcher_Snap)

IMPLEMENT_STANDARD_TYPE(Sketcher_SnapCenter)
IMPLEMENT_STANDARD_SUPERTYPE(Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Sketcher_Snap)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (MMgt_TShared)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY (Standard_Transient)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(Sketcher_SnapCenter)

